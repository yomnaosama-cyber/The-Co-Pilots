#include <QApplication>
#include "mainwindow.h"
#include "database.h"
#include <QSettings>
#include <QDebug>
#include <cstdlib>

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);
    app.setOrganizationName("CoPilots");
    app.setApplicationName("FoodSharingApp");

    // Print configuration for debugging
    qDebug() << "=== Food Sharing App Configuration ===";
    
    QString dbPath = qgetenv("FOODAPP_DB_PATH");
    if (dbPath.isEmpty()) {
        dbPath = "food_sharing.db (local)";
    }
    qDebug() << "Database Path:" << dbPath;
    
    QString serverUrl = qgetenv("FOODAPP_SERVER_BASE_URL");
    if (serverUrl.isEmpty()) {
        serverUrl = "http://localhost:3000 (fallback)";
    }
    qDebug() << "Server URL:" << serverUrl;
    
    QString runServer = qgetenv("FOODAPP_RUN_SERVER");
    qDebug() << "Run Map Server:" << (runServer == "1" ? "YES" : "NO (client mode)");
    
    QString apiKey = qgetenv("GOOGLE_MAPS_API_KEY");
    qDebug() << "API Key:" << (apiKey.isEmpty() ? "Using fallback key" : "Using environment key");
    qDebug() << "====================================";

    // Initialize database
    if (!DatabaseManager::initDatabase()) {
        return -1;
    }

    QSettings settings;
    QString lastModule = settings.value("lastModule", "").toString();
    QString lastModuleData = settings.value("lastModuleData", "").toString();

    // Create and show main window
    MainWindow window;

    if (!lastModule.isEmpty()) {
        window.showModuleDirectly(lastModule, lastModuleData);
    } else {
        window.show();
    }

    return app.exec();
}
