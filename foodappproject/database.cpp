#include "database.h"
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QMessageBox>
#include <QDebug>
#include <QStringList>
#include <QRegularExpression>

bool DatabaseManager::initDatabase() {
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("food_sharing.db");

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
               "address TEXT NOT NULL,"
               "request_date DATETIME DEFAULT CURRENT_TIMESTAMP,"
               "FOREIGN KEY(person_id) REFERENCES people_sign(id))");
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
               "food_type TEXT NOT NULL, "
               "donation_location TEXT NOT NULL, "
               "delivery_method TEXT NOT NULL, "
               "donation_date DATETIME DEFAULT CURRENT_TIMESTAMP)");
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
               "details TEXT,"
               "match_status TEXT DEFAULT 'unmatched',"  // 'unmatched', 'matched', 'pending'
               "matched_with_id INTEGER DEFAULT -1,"      // ID of the matched address in this same table
               "match_score INTEGER DEFAULT 0,"           // Number of common words
               "created_at DATETIME DEFAULT CURRENT_TIMESTAMP)");
}

// Function to match addresses based on common words
void DatabaseManager::matchAddresses() {
    QSqlQuery query;
    
    // Get all unmatched meal requests
    query.exec("SELECT id, address, person_name FROM all_addresses "
               "WHERE source_type = 'meal_request' AND match_status = 'unmatched' "
               "ORDER BY created_at ASC");
    
    while (query.next()) {
        int mealId = query.value(0).toInt();
        QString mealAddress = query.value(1).toString();
        QString personName = query.value(2).toString();
        
        // Clean and tokenize the meal address
        QStringList mealWords = mealAddress.toLower()
            .remove(QRegularExpression("[^a-zA-Z0-9\\s]"))
            .split(" ", Qt::SkipEmptyParts);
        
        // Find best matching donation
        QSqlQuery donationQuery;
        donationQuery.exec("SELECT id, address, provider_name FROM all_addresses "
                          "WHERE source_type = 'donation' AND match_status = 'unmatched' "
                          "ORDER BY created_at ASC");
        
        int bestMatchId = -1;
        int bestMatchScore = 0;
        QString bestMatchAddress;
        QString bestMatchProvider;
        
        while (donationQuery.next()) {
            int donationId = donationQuery.value(0).toInt();
            QString donationAddress = donationQuery.value(1).toString();
            QString providerName = donationQuery.value(2).toString();
            
            // Clean and tokenize donation address
            QStringList donationWords = donationAddress.toLower()
                .remove(QRegularExpression("[^a-zA-Z0-9\\s]"))
                .split(" ", Qt::SkipEmptyParts);
            
            // Calculate match score (common words count)
            int matchScore = 0;
            for (const QString& mealWord : mealWords) {
                if (donationWords.contains(mealWord) && mealWord.length() > 2) {
                    matchScore++;
                }
            }
            
            // Also check for partial matches
            for (const QString& mealWord : mealWords) {
                for (const QString& donationWord : donationWords) {
                    if (mealWord.length() > 2 && donationWord.length() > 2) {
                        if (donationWord.contains(mealWord) || mealWord.contains(donationWord)) {
                            matchScore++;
                        }
                    }
                }
            }
            
            if (matchScore > bestMatchScore) {
                bestMatchScore = matchScore;
                bestMatchId = donationId;
                bestMatchAddress = donationAddress;
                bestMatchProvider = providerName;
            }
        }
        
        // If a match was found (score > 0), update both entries in the same table
        if (bestMatchId != -1 && bestMatchScore > 0) {
            // Update the meal request entry
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
            
            // Update the donation entry
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
            
            qDebug() << "Matched - Meal Request:" << personName 
                     << "with Donation:" << bestMatchProvider 
                     << "| Score:" << bestMatchScore;
        }
    }
    
    // Log unmatched addresses
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
