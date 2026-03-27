#include "mainwindow.h"
#include <QVBoxLayout>
#include <QApplication>
#include <QPushButton>
#include <QDialog>
#include <QLabel>
#include <QLineEdit>
#include <QMessageBox>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QVariant>
#include <QGraphicsDropShadowEffect>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("mydata.db");

    if (!db.open()) {
        qDebug() << "Error: connection with database fail";
    } else {
        qDebug() << "Database: connection ok";
        // Create the table
        QSqlQuery query;
        query.exec("CREATE TABLE IF NOT EXISTS users ("
                   "id INTEGER PRIMARY KEY AUTOINCREMENT, "
                   "name TEXT, "
                   "national_id TEXT, "
                   "age TEXT, "
                   "city TEXT, "
                   "vehicle TEXT)");
    }

    MainWindow w;
    w.setMinimumSize(450, 600); // Set a nice window size for the layout

    // --- Updated Stylesheet to match your image ---
    QString buttonStyle =
        "QMainWindow { background-color: #0f0c29; }" // Dark space background
        "QPushButton {"
        "  color: white;"
        "  font-weight: bold;"
        "  font-size: 15px;"
        "  border-radius: 12px;"
        "  padding: 15px;"
        "  min-height: 40px;"
        "  border: 1px solid rgba(255, 255, 255, 40);"
        "}"
        // Specific gradients for each button ID
        "QPushButton#btn1 { background: qlineargradient(x1:0, y1:0, x2:1, y2:0, stop:0 #ff8c61, stop:1 #ff5e62); }"
        "QPushButton#btn2 { background: qlineargradient(x1:0, y1:0, x2:1, y2:0, stop:0 #4facfe, stop:1 #00f2fe); }"
        "QPushButton#btn3 { background: qlineargradient(x1:0, y1:0, x2:1, y2:0, stop:0 #43e97b, stop:1 #38f9d7); }"
        "QPushButton:hover { margin-top: 2px; }";

    QPushButton Btn1("Sign up");
    QPushButton Btn2("Delivary notifications");
    QPushButton Btn3("Pickup");

    // Give buttons IDs so the stylesheet can color them individually
    Btn1.setObjectName("btn1");
    Btn2.setObjectName("btn2");
    Btn3.setObjectName("btn3");

    // --- Adding the Glow Effects ---
    QGraphicsDropShadowEffect *glow1 = new QGraphicsDropShadowEffect();
    glow1->setBlurRadius(25);
    glow1->setColor(QColor(255, 94, 98, 180));
    glow1->setOffset(0, 0);
    Btn1.setGraphicsEffect(glow1);

    QGraphicsDropShadowEffect *glow2 = new QGraphicsDropShadowEffect();
    glow2->setBlurRadius(25);
    glow2->setColor(QColor(0, 242, 254, 180));
    glow2->setOffset(0, 0);
    Btn2.setGraphicsEffect(glow2);

    QGraphicsDropShadowEffect *glow3 = new QGraphicsDropShadowEffect();
    glow3->setBlurRadius(25);
    glow3->setColor(QColor(56, 249, 215, 180));
    glow3->setOffset(0, 0);
    Btn3.setGraphicsEffect(glow3);

    QVBoxLayout *layout = new QVBoxLayout();
    layout->setSpacing(25);            // Spacing between buttons
    layout->setContentsMargins(60, 80, 60, 80); // Center margins

    QWidget *CW = new QWidget();
    layout->addWidget(&Btn1);
    layout->addWidget(&Btn2);
    layout->addWidget(&Btn3);
    CW->setLayout(layout);

    w.setCentralWidget(CW);
    a.setStyleSheet(buttonStyle);
    w.show();

    QObject::connect(&Btn1, &QPushButton::clicked, [&]() {
        // --- Create the Popup (The different window) ---
        QDialog dialog(&w);
        dialog.setWindowTitle("User Questions");
        dialog.setMinimumSize(300, 400);
        // Added styling to the dialog so it doesn't look like a basic grey window
        dialog.setStyleSheet("QDialog { background-color: #1a1a2e; } QLabel { color: white; } QLineEdit { padding: 5px; border-radius: 3px; }");

        QVBoxLayout *dialogLayout = new QVBoxLayout(&dialog);

        // List of 5 questions
        QStringList questions = {" Name", "National ID number:", "Age:", "preffered City or towns to work in:", "vehicle type"};
        QList<QLineEdit*> inputFields;

        for (const QString &qText : questions) {
            dialogLayout->addWidget(new QLabel(qText, &dialog));
            QLineEdit *edit = new QLineEdit(&dialog);
            dialogLayout->addWidget(edit);
            inputFields.append(edit); // Store to read later
        }

        QPushButton *submitBtn = new QPushButton("Submit", &dialog);
        submitBtn->setStyleSheet("background-color: #4facfe; color: white; padding: 8px; border-radius: 5px;");
        dialogLayout->addWidget(submitBtn);

        // Connect popup button to close the window
        QObject::connect(submitBtn, &QPushButton::clicked, &dialog, &QDialog::accept);

        if (dialog.exec() == QDialog::Accepted) {
            QSqlQuery query;
            query.prepare("INSERT INTO users (name, national_id, age, city, vehicle) "
                          "VALUES (:name, :nid, :age, :city, :vehicle)");

            query.bindValue(":name",    inputFields[0]->text());
            query.bindValue(":nid",     inputFields[1]->text());
            query.bindValue(":age",     inputFields[2]->text());
            query.bindValue(":city",    inputFields[3]->text());
            query.bindValue(":vehicle", inputFields[4]->text());

            if (query.exec()) {
                QMessageBox::information(&w, "Success", "Data Saved to Database!");
            } else {
                QMessageBox::critical(&w, "Error", "Database Error: " + query.lastError().text());
            }
        }
    });

    return a.exec();
}
