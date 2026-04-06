#include "delivery.h"
#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QLineEdit>
#include <QDialog>
#include <QIntValidator>
#include <QMessageBox>
#include <QSqlQuery>
#include <QSqlError>
#include <QFont>

class DeliveryModulePrivate {
public:
    QDialog *signUpDialog = nullptr;
    QLineEdit *nameField = nullptr;
    QLineEdit *nationalIdField = nullptr;
    QLineEdit *personalIdField = nullptr;
    QLineEdit *ageField = nullptr;
    QLineEdit *cityField = nullptr;
    QLineEdit *vehicleField = nullptr;
};

DeliveryModule::DeliveryModule(QWidget *parent)
    : QMainWindow(parent), d(new DeliveryModulePrivate())
{
    setupUI();
}

DeliveryModule::~DeliveryModule()
{
    delete d;
}

void DeliveryModule::setupUI()
{
    setWindowTitle("Delivery");
    setMinimumSize(800, 600);
    setStyleSheet("QMainWindow { background-color: #efe4d0; }");

    QWidget* central = new QWidget();
    setCentralWidget(central);

    QVBoxLayout* layout = new QVBoxLayout(central);
    layout->setAlignment(Qt::AlignHCenter);
    layout->setSpacing(40);

    QLabel* header = new QLabel("Delivery System");
    header->setFont(QFont("Arial", 40, QFont::Bold));
    header->setAlignment(Qt::AlignCenter);
    header->setStyleSheet("color: #813e15; background-color: transparent; padding: 20px;");

    QString buttonStyle = 
        "QPushButton {"
        "   background-color: #f4ece7;"
        "   border: 2px solid #813e15;"
        "   border-radius: 28px;"
        "   padding: 15px;"
        "   font-size: 18px;"
        "   font-weight: bold;"
        "   color: #ba6c3b;"
        "}"
        "QPushButton:hover {"
        "   background-color: #e8cebe;"
        "   border-color: #4b240c;"
        "}";

    QPushButton* signUpBtn = new QPushButton("Sign up");
    signUpBtn->setStyleSheet(buttonStyle);
    signUpBtn->setCursor(Qt::PointingHandCursor);

    QPushButton* notificationsBtn = new QPushButton("Delivery notifications");
    notificationsBtn->setStyleSheet(buttonStyle);
    notificationsBtn->setCursor(Qt::PointingHandCursor);

    QPushButton* pickupBtn = new QPushButton("Pickup");
    pickupBtn->setStyleSheet(buttonStyle);
    pickupBtn->setCursor(Qt::PointingHandCursor);

    layout->addStretch();
    layout->addWidget(header);
    layout->addWidget(signUpBtn);
    layout->addWidget(notificationsBtn);
    layout->addWidget(pickupBtn);
    layout->addStretch();

    setupSignUpDialog();

    connect(signUpBtn, &QPushButton::clicked, this, &DeliveryModule::handleSignUp);
    connect(notificationsBtn, &QPushButton::clicked, this, &DeliveryModule::handleNotifications);
    connect(pickupBtn, &QPushButton::clicked, this, &DeliveryModule::handlePickup);
}

void DeliveryModule::setupSignUpDialog()
{
    d->signUpDialog = new QDialog(this);
    d->signUpDialog->setWindowTitle("Delivery Sign-Up");
    d->signUpDialog->setMinimumSize(400, 550);
    d->signUpDialog->setStyleSheet(
        "QDialog { background-color: #f4ece7; }"
        "QLabel { color: #4b240c; font-size: 14px; font-weight: 500; margin-top: 8px; }"
        "QLineEdit {"
        "    background-color: #ffffff;"
        "    color: #4b240c;"
        "    padding: 10px;"
        "    border: 2px solid #ba6c3b;"
        "    border-radius: 15px;"
        "    font-size: 13px;"
        "}"
        "QLineEdit:focus { border: 2px solid #813e15; background-color: #fffaf7; }"
        "QLineEdit:hover { border: 2px solid #813e15; }"
        "QPushButton {"
        "    background-color: #f4ece7;"
        "    border: 2px solid #813e15;"
        "    border-radius: 28px;"
        "    padding: 12px;"
        "    font-size: 18px;"
        "    font-weight: bold;"
        "    color: #ba6c3b;"
        "    margin-top: 20px;"
        "}"
        "QPushButton:hover {"
        "    background-color: #e8cebe;"
        "    border-color: #4b240c;"
        "    color: #813e15;"
        "}"
        "QPushButton:pressed { background-color: #d4b89c; }"
    );

    QVBoxLayout* layout = new QVBoxLayout(d->signUpDialog);
    layout->setSpacing(12);
    layout->setContentsMargins(30, 30, 30, 30);

    // Create form fields
    QLabel* nameLabel = new QLabel("Name:");
    d->nameField = new QLineEdit();
    d->nameField->setPlaceholderText("Enter your name");

    QLabel* nationalIdLabel = new QLabel("National ID Number:");
    d->nationalIdField = new QLineEdit();
    d->nationalIdField->setPlaceholderText("Enter national ID");

    QLabel* personalIdLabel = new QLabel("Personal ID:");
    d->personalIdField = new QLineEdit();
    d->personalIdField->setPlaceholderText("Enter personal ID");

    QLabel* ageLabel = new QLabel("Age:");
    d->ageField = new QLineEdit();
    d->ageField->setValidator(new QIntValidator(0, 120, d->ageField));
    d->ageField->setPlaceholderText("Enter age (0-120)");

    QLabel* cityLabel = new QLabel("Preferred City/Town:");
    d->cityField = new QLineEdit();
    d->cityField->setPlaceholderText("Enter preferred city");

    QLabel* vehicleLabel = new QLabel("Vehicle Type:");
    d->vehicleField = new QLineEdit();
    d->vehicleField->setPlaceholderText("Enter vehicle type");

    layout->addWidget(nameLabel);
    layout->addWidget(d->nameField);
    layout->addWidget(nationalIdLabel);
    layout->addWidget(d->nationalIdField);
    layout->addWidget(personalIdLabel);
    layout->addWidget(d->personalIdField);
    layout->addWidget(ageLabel);
    layout->addWidget(d->ageField);
    layout->addWidget(cityLabel);
    layout->addWidget(d->cityField);
    layout->addWidget(vehicleLabel);
    layout->addWidget(d->vehicleField);

    QPushButton* submitBtn = new QPushButton("Submit Registration");
    submitBtn->setCursor(Qt::PointingHandCursor);
    layout->addWidget(submitBtn);

    connect(submitBtn, &QPushButton::clicked, [this]() {
        // Validation
        if (d->nameField->text().isEmpty() || 
            d->nationalIdField->text().isEmpty() ||
            d->personalIdField->text().isEmpty() || 
            d->ageField->text().isEmpty() ||
            d->cityField->text().isEmpty() || 
            d->vehicleField->text().isEmpty()) {
            QMessageBox::warning(d->signUpDialog, "Validation Error", "Please fill all fields!");
            return;
        }

        bool ageOk;
        int age = d->ageField->text().toInt(&ageOk);
        if (!ageOk || age < 0 || age > 120) {
            QMessageBox::warning(d->signUpDialog, "Validation Error", "Please enter a valid age!");
            return;
        }

        // Save to database
        QSqlQuery query;
        query.prepare("INSERT INTO users (name, national_id, personal_id, age, city, vehicle) "
                      "VALUES (:name, :national_id, :personal_id, :age, :city, :vehicle)");
        query.bindValue(":name", d->nameField->text().trimmed());
        query.bindValue(":national_id", d->nationalIdField->text().trimmed());
        query.bindValue(":personal_id", d->personalIdField->text().trimmed());
        query.bindValue(":age", QString::number(age));
        query.bindValue(":city", d->cityField->text().trimmed());
        query.bindValue(":vehicle", d->vehicleField->text().trimmed());

        if (query.exec()) {
            QMessageBox::information(d->signUpDialog, "Success!",
                QString("✓ Delivery Person Registered Successfully!\n\nName: %1\nPersonal ID: %2")
                .arg(d->nameField->text())
                .arg(d->personalIdField->text()));
            d->signUpDialog->close();
            d->nameField->clear();
            d->nationalIdField->clear();
            d->personalIdField->clear();
            d->ageField->clear();
            d->cityField->clear();
            d->vehicleField->clear();
        } else {
            QMessageBox::critical(d->signUpDialog, "Database Error",
                "Error: " + query.lastError().text());
        }
    });
}

void DeliveryModule::handleSignUp()
{
    d->signUpDialog->exec();
}

void DeliveryModule::handleNotifications()
{
    QMessageBox::information(this, "Delivery Notifications", 
        "Delivery notifications feature coming soon!");
}

void DeliveryModule::handlePickup()
{
    QMessageBox::information(this, "Pickup", 
        "Pickup scheduling feature coming soon!");
}

