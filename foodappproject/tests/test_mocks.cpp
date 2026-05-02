#include <gtest/gtest.h>
#include <gmock/gmock.h>

// ============================================
// Interface to mock (represents your database)
// ============================================
class DatabaseInterface {
public:
    virtual ~DatabaseInterface() = default;
    virtual bool donationExists(int donationId) = 0;
    virtual int getRemainingMeals(int donationId) = 0;
};

// ============================================
// Mock implementation using gMock
// ============================================
class MockDatabase : public DatabaseInterface {
public:
    MOCK_METHOD(bool, donationExists, (int donationId), (override));
    MOCK_METHOD(int, getRemainingMeals, (int donationId), (override));
};

// ============================================
// Service class that uses the database interface
// This simulates business logic from your app
// ============================================
class MealService {
private:
    DatabaseInterface* db;
    
public:
    explicit MealService(DatabaseInterface* database) : db(database) {}
    
    // Returns true if the requested meals can be fulfilled
    bool requestMeal(int donationId, int requestedMeals) {
        // First check if donation exists
        if (!db->donationExists(donationId)) {
            return false;  // Donation doesn't exist
        }
        
        // Get available meals
        int available = db->getRemainingMeals(donationId);
        
        // Check if we can fulfill the request
        return available >= requestedMeals;
    }
};

// ============================================
// TEST 1: RequestMealWhenEnoughAvailable
// ============================================
TEST(MockTest, RequestMealWhenEnoughAvailable) {
    // Create mock database
    MockDatabase mockDb;
    MealService service(&mockDb);
    
    // Set expectations
    EXPECT_CALL(mockDb, donationExists(123))
        .Times(1)
        .WillOnce(testing::Return(true));
    
    EXPECT_CALL(mockDb, getRemainingMeals(123))
        .Times(1)
        .WillOnce(testing::Return(50));
    
    // Execute the test
    bool result = service.requestMeal(123, 30);
    
    // Verify
    EXPECT_TRUE(result) << "Request with 30 meals should be accepted when 50 available";
}

// ============================================
// TEST 2: RequestMealWhenNotEnoughAvailable
// ============================================
TEST(MockTest, RequestMealWhenNotEnoughAvailable) {
    // Create mock database
    MockDatabase mockDb;
    MealService service(&mockDb);
    
    // Set expectations
    EXPECT_CALL(mockDb, donationExists(123))
        .Times(1)
        .WillOnce(testing::Return(true));
    
    EXPECT_CALL(mockDb, getRemainingMeals(123))
        .Times(1)
        .WillOnce(testing::Return(10));
    
    // Execute the test
    bool result = service.requestMeal(123, 30);
    
    // Verify
    EXPECT_FALSE(result) << "Request with 30 meals should be rejected when only 10 available";
}

// ============================================
// TEST 3: RequestMealWhenDonationDoesNotExist
// ============================================
TEST(MockTest, RequestMealWhenDonationDoesNotExist) {
    // Create mock database
    MockDatabase mockDb;
    MealService service(&mockDb);
    
    // Set expectations
    EXPECT_CALL(mockDb, donationExists(999))
        .Times(1)
        .WillOnce(testing::Return(false));
    
    // getRemainingMeals should NEVER be called if donation doesn't exist
    EXPECT_CALL(mockDb, getRemainingMeals(999))
        .Times(0);
    
    // Execute the test
    bool result = service.requestMeal(999, 30);
    
    // Verify
    EXPECT_FALSE(result) << "Request should be rejected when donation doesn't exist";
}