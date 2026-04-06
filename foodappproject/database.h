#ifndef DATABASE_H
#define DATABASE_H

#include <QString>

class DatabaseManager {
public:
    // Initialize the database and create all necessary tables
    static bool initDatabase();
    
private:
    // Helper methods for table creation
    static void createPeopleSignTable();
    static void createMealRequestsTable();
    static void createUsersTable();
    static void createRestaurantsTable();
    static void createOrganizationsTable();
    static void createFoodDonationsTable();
    static void createAddressesTable();
};

#endif // DATABASE_H
