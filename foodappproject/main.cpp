#include <QApplication>
#include "mainwindow.h"
#include "database.h"
#include <QSettings>

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);
    app.setOrganizationName("CoPilots");
    app.setApplicationName("FoodSharingApp");

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
