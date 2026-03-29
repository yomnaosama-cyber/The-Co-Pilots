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
#include <QComboBox>
#include <QFormLayout>
#include <QHBoxLayout>
#include <QStackedWidget>
#include <QTextEdit>
#include <QDebug>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("mydata.db");

    if (!db.open()) {
        qDebug() << "Error: connection with database fail";
    } else {
        qDebug() << "Database: connection ok";
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
    w.setMinimumSize(450, 760);

    QString buttonStyle =
        "QMainWindow { background-color: #0f0c29; }"
        "QPushButton {"
        "  color: white;"
        "  font-weight: bold;"
        "  font-size: 15px;"
        "  border-radius: 12px;"
        "  padding: 15px;"
        "  min-height: 40px;"
        "  border: 1px solid rgba(255, 255, 255, 40);"
        "}"
        "QPushButton#btn1 { background: qlineargradient(x1:0, y1:0, x2:1, y2:0, stop:0 #ff8c61, stop:1 #ff5e62); }"
        "QPushButton#btn2 { background: qlineargradient(x1:0, y1:0, x2:1, y2:0, stop:0 #4facfe, stop:1 #00f2fe); }"
        "QPushButton#btn3 { background: qlineargradient(x1:0, y1:0, x2:1, y2:0, stop:0 #43e97b, stop:1 #38f9d7); }"
        "QPushButton#btn4 { background: qlineargradient(x1:0, y1:0, x2:1, y2:0, stop:0 #8e6bff, stop:1 #d16bff); }"
        "QPushButton:hover { margin-top: 2px; }";

    QPushButton Btn1("Sign up");
    QPushButton Btn2("Delivary notifications");
    QPushButton Btn3("Pickup");
    QPushButton Btn4("Provider Registration");

    Btn1.setObjectName("btn1");
    Btn2.setObjectName("btn2");
    Btn3.setObjectName("btn3");
    Btn4.setObjectName("btn4");

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

    QGraphicsDropShadowEffect *glow4 = new QGraphicsDropShadowEffect();
    glow4->setBlurRadius(25);
    glow4->setColor(QColor(209, 107, 255, 180));
    glow4->setOffset(0, 0);
    Btn4.setGraphicsEffect(glow4);

    QVBoxLayout *layout = new QVBoxLayout();
    layout->setSpacing(25);
    layout->setContentsMargins(60, 80, 60, 80);

    QWidget *CW = new QWidget();
    layout->addWidget(&Btn1);
    layout->addWidget(&Btn2);
    layout->addWidget(&Btn3);
    layout->addWidget(&Btn4);
    CW->setLayout(layout);

    w.setCentralWidget(CW);
    a.setStyleSheet(buttonStyle);
    w.show();

    QObject::connect(&Btn1, &QPushButton::clicked, [&]() {
        QDialog dialog(&w);
        dialog.setWindowTitle("User Questions");
        dialog.setMinimumSize(300, 400);
        dialog.setStyleSheet("QDialog { background-color: #1a1a2e; } QLabel { color: white; } QLineEdit { padding: 5px; border-radius: 3px; }");

        QVBoxLayout *dialogLayout = new QVBoxLayout(&dialog);

        QStringList questions = {" Name", "National ID number:", "Age:", "preffered City or towns to work in:", "vehicle type"};
        QList<QLineEdit*> inputFields;

        for (const QString &qText : questions) {
            dialogLayout->addWidget(new QLabel(qText, &dialog));
            QLineEdit *edit = new QLineEdit(&dialog);
            dialogLayout->addWidget(edit);
            inputFields.append(edit);
        }

        QPushButton *submitBtn = new QPushButton("Submit", &dialog);
        submitBtn->setStyleSheet("background-color: #4facfe; color: white; padding: 8px; border-radius: 5px;");
        dialogLayout->addWidget(submitBtn);

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

    QObject::connect(&Btn4, &QPushButton::clicked, [&]() {
        QDialog dialog(&w);
        dialog.setWindowTitle("Provider Registration");
        dialog.setMinimumSize(520, 560);
        dialog.setStyleSheet(
            "QDialog { background-color: #120f2b; }"
            "QLabel { color: white; font-size: 14px; }"
            "QLineEdit, QTextEdit, QComboBox {"
            "background-color: #1f1a3d;"
            "color: white;"
            "border: 1px solid #4b4b6b;"
            "border-radius: 10px;"
            "padding: 8px;"
            "font-size: 14px;"
            "}"
            "QPushButton {"
            "color: white;"
            "font-size: 16px;"
            "font-weight: 700;"
            "border: none;"
            "border-radius: 16px;"
            "padding: 12px;"
            "background: qlineargradient(x1:0, y1:0, x2:1, y2:0, stop:0 #8e6bff, stop:1 #d16bff);"
            "}"
        );

        QVBoxLayout *mainLayout = new QVBoxLayout(&dialog);

        QLabel *title = new QLabel("Community Food Connect", &dialog);
        title->setAlignment(Qt::AlignCenter);
        title->setStyleSheet("font-size: 24px; font-weight: 700; color: white;");
        mainLayout->addWidget(title);

        QHBoxLayout *switchLayout = new QHBoxLayout();
        QPushButton *restaurantButton = new QPushButton("Restaurant", &dialog);
        QPushButton *organizationButton = new QPushButton("Organization", &dialog);
        switchLayout->addWidget(restaurantButton);
        switchLayout->addWidget(organizationButton);
        mainLayout->addLayout(switchLayout);

        QStackedWidget *stack = new QStackedWidget(&dialog);

        QWidget *restaurantPage = new QWidget();
        QVBoxLayout *restaurantPageLayout = new QVBoxLayout(restaurantPage);
        QFormLayout *restaurantForm = new QFormLayout();

        QLineEdit *resName = new QLineEdit();
        QLineEdit *resLocation = new QLineEdit();
        QComboBox *resFoodType = new QComboBox();
        QLineEdit *resContact = new QLineEdit();

        resFoodType->addItems({"Bakery", "Fast Food", "Groceries", "Home Meals", "Restaurant Meals", "Vegetarian", "Other"});

        restaurantForm->addRow("Restaurant Name:", resName);
        restaurantForm->addRow("Location:", resLocation);
        restaurantForm->addRow("Food Type:", resFoodType);
        restaurantForm->addRow("Contact Info:", resContact);

        QPushButton *restaurantSubmit = new QPushButton("Register Restaurant");
        restaurantPageLayout->addLayout(restaurantForm);
        restaurantPageLayout->addWidget(restaurantSubmit);

        QWidget *organizationPage = new QWidget();
        QVBoxLayout *organizationPageLayout = new QVBoxLayout(organizationPage);
        QFormLayout *organizationForm = new QFormLayout();

        QLineEdit *orgName = new QLineEdit();
        QComboBox *orgType = new QComboBox();
        QLineEdit *orgLocation = new QLineEdit();
        QTextEdit *orgPurpose = new QTextEdit();
        QLineEdit *orgContact = new QLineEdit();

        orgType->addItems({"Charity", "Community Kitchen", "Food Bank", "Mosque", "NGO", "Shelter", "Other"});

        organizationForm->addRow("Organization Name:", orgName);
        organizationForm->addRow("Organization Type:", orgType);
        organizationForm->addRow("Location:", orgLocation);
        organizationForm->addRow("Purpose:", orgPurpose);
        organizationForm->addRow("Contact Info:", orgContact);

        QPushButton *organizationSubmit = new QPushButton("Register Organization");
        organizationPageLayout->addLayout(organizationForm);
        organizationPageLayout->addWidget(organizationSubmit);

        stack->addWidget(restaurantPage);
        stack->addWidget(organizationPage);
        mainLayout->addWidget(stack);

        QObject::connect(restaurantButton, &QPushButton::clicked, [&]() {
            stack->setCurrentIndex(0);
        });

        QObject::connect(organizationButton, &QPushButton::clicked, [&]() {
            stack->setCurrentIndex(1);
        });

        QObject::connect(restaurantSubmit, &QPushButton::clicked, [&]() {
            if (resName->text().trimmed().isEmpty() ||
                resLocation->text().trimmed().isEmpty() ||
                resContact->text().trimmed().isEmpty()) {
                QMessageBox::warning(&dialog, "Missing Information", "Please fill in all restaurant fields.");
                return;
            }

            QMessageBox::information(&dialog, "Success", "Restaurant registered successfully.");
            dialog.accept();
        });

        QObject::connect(organizationSubmit, &QPushButton::clicked, [&]() {
            if (orgName->text().trimmed().isEmpty() ||
                orgLocation->text().trimmed().isEmpty() ||
                orgPurpose->toPlainText().trimmed().isEmpty() ||
                orgContact->text().trimmed().isEmpty()) {
                QMessageBox::warning(&dialog, "Missing Information", "Please fill in all organization fields.");
                return;
            }

            QMessageBox::information(&dialog, "Success", "Organization registered successfully.");
            dialog.accept();
        });

        dialog.exec();
    });

    return a.exec();
}

