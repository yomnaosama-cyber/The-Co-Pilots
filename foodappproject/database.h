#ifndef DATABASE_H
#define DATABASE_H

#include <QString>

class DatabaseManager {
public:
    // Initialize the database and create all necessary tables
    static bool initDatabase();
    static QString databasePath();
    // Function for matching addresses
    static void matchAddresses();
    
private:
    // Helper methods for table creation
    static void createPeopleSignTable();
    static void createMealRequestsTable();
    static void createUsersTable();
    static void createRestaurantsTable();
    static void createOrganizationsTable();
    static void createFoodDonationsTable();
    static void createAddressesTable();
static void createProviderSafetyChecksTable();
    static int extractMealCount(const QString& details);
};

#endif // DATABASE_H
