#include "database.h"
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QMessageBox>
#include <QDebug>
#include <QStringList>
#include <QRegularExpression>
#include <QDate>
#include <QFileInfo>
#include <QStandardPaths>
#include <QDir>

QString DatabaseManager::databasePath() {
    QString dbPath = qgetenv("FOODAPP_DB_PATH");
    if (dbPath.isEmpty()) {
        QString appDataDir = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
        if (appDataDir.isEmpty()) {
            appDataDir = QDir::currentPath();
        }

        QDir dir(appDataDir);
        if (!dir.exists()) {
            dir.mkpath(".");
        }

        dbPath = dir.filePath("food_sharing.db");
    } else {
        QFileInfo fileInfo(dbPath);
        QDir dir = fileInfo.absoluteDir();
        if (!dir.exists()) {
            dir.mkpath(".");
        }
    }

    return dbPath;
}

bool DatabaseManager::initDatabase() {
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");

    QString dbPath = databasePath();
    qDebug() << "Database path:" << dbPath;
    db.setDatabaseName(dbPath);

    if (!db.open()) {
        QMessageBox::critical(nullptr, "Database Error", "Could not open database");
        return false;
    }

    createPeopleSignTable();
    createMealRequestsTable();
    createUsersTable();
    createRestaurantsTable();
    createOrganizationsTable();
    createFoodDonationsTable();
    createProviderSafetyChecksTable();
    createAddressesTable();

    return true;
}

void DatabaseManager::createPeopleSignTable() {
    QSqlQuery query;
    query.exec("CREATE TABLE IF NOT EXISTS people_sign ("
               "id INTEGER PRIMARY KEY AUTOINCREMENT,"
               "name TEXT NOT NULL,"
               "people_id TEXT NOT NULL,"
               "address TEXT NOT NULL,"
               "contact TEXT NOT NULL,"
               "notes TEXT,"
               "signup_date DATETIME DEFAULT CURRENT_TIMESTAMP)");
}

void DatabaseManager::createMealRequestsTable() {
    QSqlQuery query;
    query.exec("CREATE TABLE IF NOT EXISTS meal_requests ("
               "id INTEGER PRIMARY KEY AUTOINCREMENT,"
               "person_id INTEGER,"
               "meal_count INTEGER NOT NULL,"
               "delivered_meals INTEGER DEFAULT 0, "
               "address TEXT NOT NULL,"
               "request_date DATETIME DEFAULT CURRENT_TIMESTAMP,"
               "FOREIGN KEY(person_id) REFERENCES people_sign(id))");
    query.exec("ALTER TABLE meal_requests ADD COLUMN delivered_meals INTEGER DEFAULT 0");
}

void DatabaseManager::createUsersTable() {
    QSqlQuery query;
    query.exec("CREATE TABLE IF NOT EXISTS users ("
               "id INTEGER PRIMARY KEY AUTOINCREMENT, "
               "name TEXT, "
               "national_id TEXT, "
               "age TEXT, "
               "city TEXT, "
               "vehicle TEXT)");
    query.exec("ALTER TABLE users ADD COLUMN personal_id TEXT");
}

void DatabaseManager::createRestaurantsTable() {
    QSqlQuery query;
    query.exec("CREATE TABLE IF NOT EXISTS restaurants ("
               "id INTEGER PRIMARY KEY AUTOINCREMENT, "
               "name TEXT NOT NULL, "
               "location TEXT NOT NULL, "
               "food_type TEXT NOT NULL, "
               "contact_info TEXT NOT NULL, "
               "created_at DATETIME DEFAULT CURRENT_TIMESTAMP)");
}

void DatabaseManager::createOrganizationsTable() {
    QSqlQuery query;
    query.exec("CREATE TABLE IF NOT EXISTS organizations ("
               "id INTEGER PRIMARY KEY AUTOINCREMENT, "
               "name TEXT NOT NULL, "
               "organization_type TEXT NOT NULL, "
               "location TEXT NOT NULL, "
               "purpose TEXT NOT NULL, "
               "contact_info TEXT NOT NULL, "
               "created_at DATETIME DEFAULT CURRENT_TIMESTAMP)");
}

void DatabaseManager::createFoodDonationsTable() {
    QSqlQuery query;
    query.exec("CREATE TABLE IF NOT EXISTS food_donations ("
               "id INTEGER PRIMARY KEY AUTOINCREMENT, "
               "provider_name TEXT NOT NULL, "
               "provider_role TEXT NOT NULL, "
               "food_amount TEXT NOT NULL, "
               "remaining_meals INTEGER DEFAULT 0, "
               "food_type TEXT NOT NULL, "
               "donation_location TEXT NOT NULL, "
               "delivery_method TEXT NOT NULL, "
               "expiry_date TEXT DEFAULT '', "
               "donation_date DATETIME DEFAULT CURRENT_TIMESTAMP)");
    query.exec("ALTER TABLE food_donations ADD COLUMN remaining_meals INTEGER DEFAULT 0");
    query.exec("ALTER TABLE food_donations ADD COLUMN expiry_date TEXT DEFAULT ''");
}

void DatabaseManager::createAddressesTable() {
    QSqlQuery query;
query.exec("CREATE TABLE IF NOT EXISTS all_addresses ("
           "id INTEGER PRIMARY KEY AUTOINCREMENT,"
           "source_type TEXT NOT NULL,"
           "source_id INTEGER NOT NULL,"
           "person_name TEXT,"
           "provider_name TEXT,"
           "address TEXT NOT NULL,"
           "city TEXT,"
           "street TEXT,"
           "address_details TEXT,"
           "details TEXT,"
           "match_status TEXT DEFAULT 'unmatched',"
           "matched_with_id INTEGER DEFAULT -1,"
           "match_score INTEGER DEFAULT 0,"
           "assigned_to TEXT,"
            "delivery_status TEXT DEFAULT 'pending',"
           "created_at DATETIME DEFAULT CURRENT_TIMESTAMP)");

// migrations
query.exec("ALTER TABLE all_addresses ADD COLUMN assigned_to TEXT");
query.exec("ALTER TABLE all_addresses ADD COLUMN city TEXT");
query.exec("ALTER TABLE all_addresses ADD COLUMN street TEXT");
query.exec("ALTER TABLE all_addresses ADD COLUMN address_details TEXT");
query.exec("ALTER TABLE all_addresses ADD COLUMN delivery_status TEXT");
}

void DatabaseManager::matchAddresses() {
    QSqlQuery query;

    QSqlQuery deleteExpiredAddresses;
    deleteExpiredAddresses.prepare("DELETE FROM all_addresses "
                                   "WHERE source_type = 'donation' "
                                   "AND source_id IN ("
                                   "    SELECT id FROM food_donations "
                                   "    WHERE expiry_date != '' AND expiry_date < :today"
                                   ")");
    deleteExpiredAddresses.bindValue(":today", QDate::currentDate().toString("yyyy-MM-dd"));
    deleteExpiredAddresses.exec();

    QSqlQuery deleteExpiredDonations;
    deleteExpiredDonations.prepare("DELETE FROM food_donations "
                                   "WHERE expiry_date != '' AND expiry_date < :today");
    deleteExpiredDonations.bindValue(":today", QDate::currentDate().toString("yyyy-MM-dd"));
    deleteExpiredDonations.exec();

    query.exec("SELECT id, address, person_name, city, street, details FROM all_addresses "
               "WHERE source_type = 'meal_request' AND match_status = 'unmatched' "
               "ORDER BY created_at ASC");

    while (query.next()) {
        int mealId = query.value(0).toInt();
        QString mealAddress = query.value(1).toString();
        QString personName = query.value(2).toString();
        QString mealCity = query.value(3).toString().toLower().trimmed();
        QString mealStreet = query.value(4).toString().toLower().trimmed();
        QString details = query.value(5).toString();
        
        int requestedMeals = extractMealCount(details);
       
        QStringList mealWords = mealStreet
                                    .remove(QRegularExpression("[^a-zA-Z0-9\\s]"))
                                    .split(" ", Qt::SkipEmptyParts);

        QSqlQuery donationQuery;
        donationQuery.exec("SELECT id, address, provider_name, city, street, source_id FROM all_addresses "
                           "WHERE source_type = 'donation' AND match_status = 'unmatched' "
                           "ORDER BY created_at ASC");

        int bestMatchId = -1;
        int bestMatchScore = 0;
        QString bestMatchProvider;
        int bestMatchDonationSourceId = -1;
        int bestMatchAvailableMeals = 0;

        while (donationQuery.next()) {
            int donationId = donationQuery.value(0).toInt();
            QString providerName = donationQuery.value(2).toString();
            QString donationCity = donationQuery.value(3).toString().toLower().trimmed();
            QString donationStreet = donationQuery.value(4).toString().toLower().trimmed();
            int donationSourceId = donationQuery.value(5).toInt();
            
            QSqlQuery mealQuery;
            mealQuery.prepare("SELECT remaining_meals FROM food_donations WHERE id = :id");
            mealQuery.bindValue(":id", donationSourceId);
            int availableMeals = 0;
            if (mealQuery.exec() && mealQuery.next()) {
                availableMeals = mealQuery.value(0).toInt();
            }
            
            if (availableMeals <= 0) {
                continue;
            }
           
            if (mealCity != donationCity) {
                continue;
            }

            int matchScore = 1;
            QStringList donationWords = donationStreet
                                            .remove(QRegularExpression("[^a-zA-Z0-9\\s]"))
                                            .split(" ", Qt::SkipEmptyParts);

            for (const QString& mealWord : mealWords) {
                if (donationWords.contains(mealWord) && mealWord.length() > 2) {
                    matchScore++;
                }
            }

            if (matchScore > bestMatchScore) {
                bestMatchScore = matchScore;
                bestMatchId = donationId;
                bestMatchProvider = providerName;
                bestMatchDonationSourceId = donationSourceId;
                bestMatchAvailableMeals = availableMeals;
            }
        }

        if (bestMatchId != -1 && bestMatchScore > 0) {
            int mealsToTake = qMin(requestedMeals, bestMatchAvailableMeals);
            
            QSqlQuery updateDonationMeals;
            updateDonationMeals.prepare("UPDATE food_donations SET "
                                       "remaining_meals = remaining_meals - :taken "
                                       "WHERE id = :id");
            updateDonationMeals.bindValue(":taken", mealsToTake);
            updateDonationMeals.bindValue(":id", bestMatchDonationSourceId);
            updateDonationMeals.exec();
            
            if (bestMatchAvailableMeals - mealsToTake == 0) {
                QSqlQuery updateDonation;
                updateDonation.prepare("UPDATE all_addresses SET "
                                       "match_status = 'matched', "
                                       "matched_with_id = :meal_id, "
                                       "match_score = :score "
                                       "WHERE id = :donation_id");
                updateDonation.bindValue(":meal_id", mealId);
                updateDonation.bindValue(":score", bestMatchScore);
                updateDonation.bindValue(":donation_id", bestMatchId);
                updateDonation.exec();
            }
            
            QSqlQuery updateMeal;
            updateMeal.prepare("UPDATE all_addresses SET "
                               "match_status = 'matched', "
                               "matched_with_id = :match_id, "
                               "match_score = :score "
                               "WHERE id = :meal_id");
            updateMeal.bindValue(":match_id", bestMatchId);
            updateMeal.bindValue(":score", bestMatchScore);
            updateMeal.bindValue(":meal_id", mealId);
            updateMeal.exec();

            QSqlQuery updateDelivered;
            updateDelivered.prepare("UPDATE meal_requests SET delivered_meals = delivered_meals + :taken "
                                    "WHERE person_id = (SELECT person_id FROM all_addresses WHERE id = :meal_id)");
            updateDelivered.bindValue(":taken", mealsToTake);
            updateDelivered.bindValue(":meal_id", mealId);
            updateDelivered.exec();

            QSqlQuery checkComplete;
            checkComplete.prepare("UPDATE meal_requests SET match_status = 'completed' "
                                  "WHERE person_id = (SELECT person_id FROM all_addresses WHERE id = :meal_id) "
                                  "AND delivered_meals >= meal_count");
            checkComplete.bindValue(":meal_id", mealId);
            checkComplete.exec();

            qDebug() << "Matched - Meal:" << personName 
                     << "with Donation:" << bestMatchProvider
                     << "| City:" << mealCity 
                     << "| Score:" << bestMatchScore
                     << "| Meals taken:" << mealsToTake << "of" << requestedMeals;
        }
    }

    QSqlQuery unmatchedQuery;
    unmatchedQuery.exec("SELECT id, source_type, address, person_name, provider_name, match_status "
                       "FROM all_addresses WHERE match_status = 'unmatched'");
    
    while (unmatchedQuery.next()) {
        int id = unmatchedQuery.value(0).toInt();
        QString type = unmatchedQuery.value(1).toString();
        QString address = unmatchedQuery.value(2).toString();
        QString personName = unmatchedQuery.value(3).toString();
        QString providerName = unmatchedQuery.value(4).toString();
        
        qDebug() << "Unmatched" << type << ":" 
                 << (type == "meal_request" ? personName : providerName)
                 << "-" << address;
    }
}

// Helper function to extract meal count from details string
int DatabaseManager::extractMealCount(const QString& details) {
    QRegularExpression re("(\\d+)\\s*meals? requested");
    QRegularExpressionMatch match = re.match(details);
    if (match.hasMatch()) {
        return match.captured(1).toInt();
    }
    return 0;
}
void DatabaseManager::createProviderSafetyChecksTable() {
    QSqlQuery query;
    query.exec("CREATE TABLE IF NOT EXISTS provider_safety_checks ("
               "id INTEGER PRIMARY KEY AUTOINCREMENT, "
               "provider_name TEXT NOT NULL, "
               "provider_role TEXT NOT NULL, "
               "food_age TEXT NOT NULL, "
               "ingredients TEXT NOT NULL, "
               "expiry_date TEXT NOT NULL DEFAULT '', "
               "storage_location TEXT NOT NULL, "
               "storage_temperature TEXT, "
               "allergen_info TEXT, "
               "safety_notes TEXT, "
               "photo_path TEXT, "
               "check_date DATETIME DEFAULT CURRENT_TIMESTAMP)");
    query.exec("ALTER TABLE provider_safety_checks ADD COLUMN expiry_date TEXT DEFAULT ''");
}
