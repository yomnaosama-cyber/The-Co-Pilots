#include <gtest/gtest.h>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include "../database.h"

class AddressMatcherTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Use in-memory database
        db = QSqlDatabase::addDatabase("QSQLITE");
        db.setDatabaseName(":memory:");
        bool ok = db.open();
        ASSERT_TRUE(ok) << "Failed to open test database: " << db.lastError().text().toStdString();
        
        // Initialize all tables
        bool initOk = DatabaseManager::initDatabase();
        ASSERT_TRUE(initOk) << "Failed to initialize database";
        
        donationCounter = 1;
        personCounter = 1;
    }
    
    void TearDown() override {
        db.close();
        QSqlDatabase::removeDatabase("qt_sql_default_connection");
    }
    
    QSqlDatabase db;
    int donationCounter;
    int personCounter;
    
    // Helper: Create a meal request with address
    void createMealRequest(const QString& personName, 
                          const QString& city, const QString& street, 
                          int mealCount, const QString& details = "") {
        int personId = personCounter++;
        QString fullAddress = city + ", " + street;
        QString detailsText = details.isEmpty() ? QString("%1 meals requested").arg(mealCount) : details;
        
        // Insert into people_sign
        QSqlQuery personQuery;
        personQuery.prepare("INSERT INTO people_sign (id, name, people_id, address, contact) "
                           "VALUES (?, ?, ?, ?, ?)");
        personQuery.addBindValue(personId);
        personQuery.addBindValue(personName);
        personQuery.addBindValue("ID" + QString::number(personId));
        personQuery.addBindValue(fullAddress);
        personQuery.addBindValue("555-0000");
        ASSERT_TRUE(personQuery.exec()) << personQuery.lastError().text().toStdString();
        
        // Insert into meal_requests
        QSqlQuery requestQuery;
        requestQuery.prepare("INSERT INTO meal_requests (person_id, meal_count, address) "
                            "VALUES (?, ?, ?)");
        requestQuery.addBindValue(personId);
        requestQuery.addBindValue(mealCount);
        requestQuery.addBindValue(fullAddress);
        ASSERT_TRUE(requestQuery.exec()) << requestQuery.lastError().text().toStdString();
        
        // Insert into all_addresses
        QSqlQuery addrQuery;
        addrQuery.prepare("INSERT INTO all_addresses "
                         "(source_type, source_id, person_name, address, "
                         "city, street, details) "
                         "VALUES (?, ?, ?, ?, ?, ?, ?)");
        addrQuery.addBindValue("meal_request");
        addrQuery.addBindValue(personId);
        addrQuery.addBindValue(personName);
        addrQuery.addBindValue(fullAddress);
        addrQuery.addBindValue(city);
        addrQuery.addBindValue(street);
        addrQuery.addBindValue(detailsText);
        ASSERT_TRUE(addrQuery.exec()) << addrQuery.lastError().text().toStdString();
    }
    
    // Helper: Create a donation with address
    void createDonation(const QString& providerName, 
                       const QString& city, const QString& street, 
                       int mealCount) {
        int donationId = donationCounter++;
        QString fullAddress = city + ", " + street;
        
        // Insert into food_donations (let DB auto-generate ID)
        QSqlQuery donationQuery;
        donationQuery.prepare("INSERT INTO food_donations "
                             "(provider_name, provider_role, food_amount, remaining_meals, "
                             "food_type, donation_location, delivery_method) "
                             "VALUES (?, ?, ?, ?, ?, ?, ?)");
        donationQuery.addBindValue(providerName);
        donationQuery.addBindValue("Restaurant");
        donationQuery.addBindValue(mealCount);
        donationQuery.addBindValue(mealCount);
        donationQuery.addBindValue("Food");
        donationQuery.addBindValue(fullAddress);
        donationQuery.addBindValue("Pickup");
        ASSERT_TRUE(donationQuery.exec()) << donationQuery.lastError().text().toStdString();
        
        int actualDonationId = donationQuery.lastInsertId().toInt();
        
        // Insert into all_addresses
        QSqlQuery addrQuery;
        addrQuery.prepare("INSERT INTO all_addresses "
                         "(source_type, source_id, provider_name, address, "
                         "city, street, details) "
                         "VALUES (?, ?, ?, ?, ?, ?, ?)");
        addrQuery.addBindValue("donation");
        addrQuery.addBindValue(actualDonationId);
        addrQuery.addBindValue(providerName);
        addrQuery.addBindValue(fullAddress);
        addrQuery.addBindValue(city);
        addrQuery.addBindValue(street);
        addrQuery.addBindValue(QString("%1 meals available").arg(mealCount));
        ASSERT_TRUE(addrQuery.exec()) << addrQuery.lastError().text().toStdString();
    }
    
    // Helper: Count meal requests that are matched
    int getMatchedMealRequestsCount() {
        QSqlQuery query;
        query.exec("SELECT COUNT(*) FROM all_addresses WHERE source_type = 'meal_request' AND match_status = 'matched'");
        query.next();
        return query.value(0).toInt();
    }
    
    // Helper: Check if donation has remaining meals (if > 0, it stays unmatched per your logic)
    int getDonationRemainingMeals() {
        QSqlQuery query;
        query.exec("SELECT remaining_meals FROM food_donations ORDER BY id DESC LIMIT 1");
        if (query.next()) {
            return query.value(0).toInt();
        }
        return -1;
    }
    
    // Helper: Clear all tables between tests
    void clearTables() {
        QSqlQuery query;
        query.exec("DELETE FROM all_addresses");
        query.exec("DELETE FROM food_donations");
        query.exec("DELETE FROM meal_requests");
        query.exec("DELETE FROM people_sign");
        donationCounter = 1;
        personCounter = 1;
    }
};

// ============================================
// TEST 1: MatchesSameCityExactStreet
// ============================================
TEST_F(AddressMatcherTest, MatchesSameCityExactStreet) {
    clearTables();
    
    // Create matching meal request and donation
    createMealRequest("John Doe", "Cairo", "Tahrir Street", 10);
    createDonation("Good Restaurant", "Cairo", "Tahrir Street", 20);
    
    // Run matcher
    DatabaseManager::matchAddresses();
    
    // Verify meal request is matched (donation only gets matched if fully consumed)
    int matchedMeals = getMatchedMealRequestsCount();
    EXPECT_EQ(matchedMeals, 1) << "Meal request should be matched. Matched: " << matchedMeals;
    
    // Donation should still have 10 remaining meals (20 - 10 taken = 10)
    int remaining = getDonationRemainingMeals();
    EXPECT_EQ(remaining, 10) << "Donation should have 10 remaining meals. Remaining: " << remaining;
}

// ============================================
// TEST 2: NoMatchDifferentCities
// ============================================
TEST_F(AddressMatcherTest, NoMatchDifferentCities) {
    clearTables();
    
    // Create request in Cairo, donation in Alexandria (different cities)
    createMealRequest("John Doe", "Cairo", "Tahrir Street", 10);
    createDonation("Good Restaurant", "Alexandria", "Corniche", 20);
    
    // Run matcher
    DatabaseManager::matchAddresses();
    
    // Verify NO match was made
    int matchedMeals = getMatchedMealRequestsCount();
    EXPECT_EQ(matchedMeals, 0) << "Different cities should NOT create matches. Matched: " << matchedMeals;
}

// ============================================
// TEST 3: PartialStreetMatch
// ============================================
TEST_F(AddressMatcherTest, PartialStreetMatch) {
    clearTables();
    
    // Create request with "Main Street", donation with "Main St" (abbreviated)
    createMealRequest("John Doe", "Cairo", "Main Street", 5);
    createDonation("Good Restaurant", "Cairo", "Main St", 10);
    
    // Run matcher
    DatabaseManager::matchAddresses();
    
    // Verify match was made despite Street vs St difference
    int matchedMeals = getMatchedMealRequestsCount();
    EXPECT_EQ(matchedMeals, 1) << "Should match 'Main Street' with 'Main St'. Matched: " << matchedMeals;
}

// ============================================
// TEST 4: MatchesPartialMeals
// ============================================
TEST_F(AddressMatcherTest, MatchesPartialMeals) {
    clearTables();
    
    // Request needs 30 meals, Donation has only 20 meals
    createMealRequest("John Doe", "Cairo", "Tahrir Street", 30);
    createDonation("Good Restaurant", "Cairo", "Tahrir Street", 20);
    
    // Run matcher
    DatabaseManager::matchAddresses();
    
    // Verify donation has 0 remaining meals (all 20 taken)
    int remaining = getDonationRemainingMeals();
    EXPECT_EQ(remaining, 0) << "Donation should have 0 remaining meals. Remaining: " << remaining;
    
    // Verify delivered_meals on request is 20
    QSqlQuery requestQuery;
    requestQuery.exec("SELECT delivered_meals FROM meal_requests ORDER BY id DESC LIMIT 1");
    if (requestQuery.next()) {
        int delivered = requestQuery.value(0).toInt();
        EXPECT_EQ(delivered, 20) << "Request should have received 20 meals. Delivered: " << delivered;
    }
    
    // Verify request still needs 10 more
    QSqlQuery completeQuery;
    completeQuery.exec("SELECT (meal_count - delivered_meals) as remaining FROM meal_requests ORDER BY id DESC LIMIT 1");
    if (completeQuery.next()) {
        int remainingNeeded = completeQuery.value(0).toInt();
        EXPECT_EQ(remainingNeeded, 10) << "Request should still need 10 meals. Remaining needed: " << remainingNeeded;
    }
}