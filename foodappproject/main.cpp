#include <QApplication>
#include "mainwindow.h"
#include "database.h"

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);

    // Initialize database
    if (!DatabaseManager::initDatabase()) {
        return -1;
    }

    // Create and show main window
    MainWindow window;
    window.show();

    return app.exec();
}
