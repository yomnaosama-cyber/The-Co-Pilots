#include "database.h"
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QMessageBox>

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
               "created_at DATETIME DEFAULT CURRENT_TIMESTAMP)");
}
