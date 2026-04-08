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
#include <QListWidget>
#include <QTextList>
#include <QTextEdit>
class DeliveryModulePrivate {
public:
    QDialog *signUpDialog = nullptr;
    QDialog *notificationsDialog = nullptr;
    QLineEdit *nameField = nullptr;
    QLineEdit *nationalIdField = nullptr;
    QLineEdit *personalIdField = nullptr;
    QLineEdit *ageField = nullptr;
    QLineEdit *cityField = nullptr;
    QLineEdit *vehicleField = nullptr;
    QListWidget *matchingDeliveriesList = nullptr;
    QTextEdit *deliveryDetailsText = nullptr;
    QString currentDeliveryPersonId = "";
    QString currentDeliveryPersonName = "";
    QDialog *loginDialog = nullptr;
    QLineEdit *loginIdField = nullptr;
};

DeliveryModule::DeliveryModule(QWidget *parent)
    : QMainWindow(parent), d(new DeliveryModulePrivate())
{
    setupUI();
    setupSignUpDialog();
    setupNotificationsDialog();
    setupLoginDialog();// ✅ ADD THIS
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

    QPushButton* loginBtn = new QPushButton("Login");
    loginBtn->setStyleSheet(buttonStyle);
    loginBtn->setCursor(Qt::PointingHandCursor);

    layout->addStretch();
    layout->addWidget(header);
    layout->addWidget(signUpBtn);
    layout->addWidget(notificationsBtn);
    layout->addWidget(pickupBtn);
    layout->addWidget(loginBtn);
    layout->addStretch();

    setupSignUpDialog();

    connect(signUpBtn, &QPushButton::clicked, this, &DeliveryModule::handleSignUp);
    connect(notificationsBtn, &QPushButton::clicked, this, &DeliveryModule::handleNotifications);
    connect(pickupBtn, &QPushButton::clicked, this, &DeliveryModule::handlePickup);
    connect(loginBtn, &QPushButton::clicked, [this]() {
        d->loginDialog->exec();
    });
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
        QSqlQuery checkQuery;
        checkQuery.prepare("SELECT COUNT(*) FROM users WHERE personal_id = :id");
        checkQuery.bindValue(":id", d->personalIdField->text().trimmed());

        if (checkQuery.exec() && checkQuery.next()) {
            if (checkQuery.value(0).toInt() > 0) {
                QMessageBox::warning(d->signUpDialog, "Error",
                                     "This Personal ID is already registered!");
                return;
            }
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
            // ✅ SET CURRENT DELIVERY PERSON DATA
            d->currentDeliveryPersonId = d->personalIdField->text().trimmed();
            d->currentDeliveryPersonName = d->nameField->text().trimmed();

            QMessageBox::information(d->signUpDialog, "Success!",
                                     QString("Delivery Person Registered Successfully!\n\nName: %1\nPersonal ID: %2")
                                         .arg(d->nameField->text())
                                         .arg(d->personalIdField->text()));

            d->signUpDialog->close();

            d->nameField->clear();
            d->nationalIdField->clear();
            d->personalIdField->clear();
            d->ageField->clear();
            d->cityField->clear();
            d->vehicleField->clear();

            // ✅ NOW THIS WILL WORK
            handleNotifications();
        }
         else {
            QMessageBox::critical(d->signUpDialog, "Database Error",
                "Error: " + query.lastError().text());
        }
    });
}
void DeliveryModule::setupLoginDialog()
{
    d->loginDialog = new QDialog(this);
    d->loginDialog->setWindowTitle("Login");
    d->loginDialog->setMinimumSize(350, 200);

    QVBoxLayout* layout = new QVBoxLayout(d->loginDialog);

    QLabel* label = new QLabel("Enter your Personal ID:");
    d->loginIdField = new QLineEdit();
    d->loginIdField->setPlaceholderText("Personal ID");

    QPushButton* loginBtn = new QPushButton("Login");

    layout->addWidget(label);
    layout->addWidget(d->loginIdField);
    layout->addWidget(loginBtn);

    connect(loginBtn, &QPushButton::clicked, [this]() {

        QString enteredId = d->loginIdField->text().trimmed();

        if (enteredId.isEmpty()) {
            QMessageBox::warning(d->loginDialog, "Error", "Please enter your ID!");
            return;
        }

        QSqlQuery query;
        query.prepare("SELECT name FROM users WHERE personal_id = :id");
        query.bindValue(":id", enteredId);

        if (!query.exec()) {
            QMessageBox::critical(d->loginDialog, "Database Error",
                                  query.lastError().text());
            return;
        }

        if (query.next()) {
            QString name = query.value(0).toString();

            // ✅ SAVE CURRENT USER
            d->currentDeliveryPersonId = enteredId;
            d->currentDeliveryPersonName = name;

            QMessageBox::information(d->loginDialog, "Welcome",
                                     "Welcome back, " + name + "!");

            d->loginDialog->close();

            // go to notifications
            handleNotifications();

        } else {
            QMessageBox::warning(d->loginDialog, "Error",
                                 "No user found with this ID!");
        }
    });
}

void DeliveryModule::setupNotificationsDialog()
{
    d->notificationsDialog = new QDialog(this);
    d->notificationsDialog->setWindowTitle("Matching Delivery Requests");
    d->notificationsDialog->setMinimumSize(700, 600);
    d->notificationsDialog->setStyleSheet(
        "QDialog { background-color: #f4ece7; }"
        "QLabel { color: #4b240c; font-size: 14px; }"
        "QListWidget { background-color: #ffffff; border: 2px solid #ba6c3b; border-radius: 10px; }"
        "QListWidget::item { padding: 10px; border-bottom: 1px solid #e8cebe; }"
        "QListWidget::item:hover { background-color: #e8cebe; }"
        "QListWidget::item:selected { background-color: #ba6c3b; color: #ffffff; }"
        "QTextEdit { background-color: #ffffff; border: 2px solid #ba6c3b; border-radius: 10px; }"
        "QPushButton {"
        "    background-color: #813e15;"
        "    color: #ffffff;"
        "    border: none;"
        "    border-radius: 8px;"
        "    padding: 10px 20px;"
        "    font-weight: bold;"
        "}"
        "QPushButton:hover { background-color: #ba6c3b; }"
        );

    QVBoxLayout* layout = new QVBoxLayout(d->notificationsDialog);
    layout->setSpacing(15);
    layout->setContentsMargins(20, 20, 20, 20);

    QLabel* titleLabel = new QLabel("📦 Matching Delivery Requests");
    titleLabel->setFont(QFont("Arial", 16, QFont::Bold));
    titleLabel->setStyleSheet("color: #813e15;");
    layout->addWidget(titleLabel);

    QLabel* instructionLabel = new QLabel("Select a request below to view details:");
    instructionLabel->setStyleSheet("color: #4b240c;");
    layout->addWidget(instructionLabel);

    d->matchingDeliveriesList = new QListWidget();
    layout->addWidget(d->matchingDeliveriesList);

    QLabel* detailsLabel = new QLabel("Request Details:");
    detailsLabel->setStyleSheet("color: #813e15; font-weight: bold;");
    layout->addWidget(detailsLabel);

    d->deliveryDetailsText = new QTextEdit();
    d->deliveryDetailsText->setReadOnly(true);
    d->deliveryDetailsText->setMaximumHeight(150);
    layout->addWidget(d->deliveryDetailsText);

    QHBoxLayout* buttonLayout = new QHBoxLayout();
    QPushButton* acceptBtn = new QPushButton("✓ Accept Delivery");
    acceptBtn->setCursor(Qt::PointingHandCursor);

    QPushButton* closeBtn = new QPushButton("Close");
    closeBtn->setCursor(Qt::PointingHandCursor);

    buttonLayout->addWidget(acceptBtn);
    buttonLayout->addStretch();
    buttonLayout->addWidget(closeBtn);
    layout->addLayout(buttonLayout);

    connect(d->matchingDeliveriesList, &QListWidget::itemSelectionChanged, [this]() {
        QListWidgetItem* item = d->matchingDeliveriesList->currentItem();
        if (item) {
            QString pickupLocation = item->data(Qt::UserRole).toString();
            QString deliveryLocation = item->data(Qt::UserRole + 1).toString();
            QString providerName = item->data(Qt::UserRole + 2).toString();
            QString personName = item->data(Qt::UserRole + 3).toString();
            int matchScore = item->data(Qt::UserRole + 4).toInt();

            d->deliveryDetailsText->setPlainText(
                QString(" Pickup From: %1\n"
                        " Deliver To: %2\n"
                        " Provider: %3\n"
                        " Customer: %4\n"
                        " Match Score: %5 common words")
                    .arg(pickupLocation)
                    .arg(deliveryLocation)
                    .arg(providerName)
                    .arg(personName)
                    .arg(matchScore));
        }
    });

    connect(acceptBtn, &QPushButton::clicked, [this]() {
        QListWidgetItem* item = d->matchingDeliveriesList->currentItem();
        if (item) {
            int deliveryId = item->data(Qt::UserRole + 5).toInt();
            QString pickupLocation = item->data(Qt::UserRole).toString();
            QString deliveryLocation = item->data(Qt::UserRole + 1).toString();

            // Update the all_addresses table to mark this delivery as assigned
            QSqlQuery updateQuery;
            updateQuery.prepare("UPDATE all_addresses SET "
                                "match_status = 'pending', "
                                "assigned_to = :user "
                                "WHERE id = :id");

            updateQuery.bindValue(":id", deliveryId);
            updateQuery.bindValue(":user", d->currentDeliveryPersonId);

            if (updateQuery.exec()) {
                QMessageBox::information(d->notificationsDialog, "✓ Delivery Accepted!",
                                         QString("Great! You've accepted this delivery:\n\n"
                                                 "Pickup: %1\n"
                                                 "Delivery To: %2\n\n"
                                                 "Delivery Person: %3")
                                             .arg(pickupLocation)
                                             .arg(deliveryLocation)
                                             .arg(d->currentDeliveryPersonName));
                d->notificationsDialog->close();
            } else {
                QMessageBox::warning(d->notificationsDialog, "Error",
                                     "Failed to accept delivery: " + updateQuery.lastError().text());
            }
        } else {
            QMessageBox::warning(d->notificationsDialog, "Error", "Please select a request first!");
        }
    });

    connect(closeBtn, &QPushButton::clicked, d->notificationsDialog, &QDialog::close);
}

void DeliveryModule::handleSignUp()
{
    d->signUpDialog->exec();
}

void DeliveryModule::handleNotifications()
{
    if (d->currentDeliveryPersonId.isEmpty()) {
        QMessageBox::information(this, "Info",
                                 "Please sign up as a delivery person first to see matching requests!");
       d->loginDialog->exec();
        return;
    }

    // Get matching deliveries from all_addresses table
    d->matchingDeliveriesList->clear();

    QSqlQuery query;
    query.prepare("SELECT aa1.id, aa1.address as pickup, aa2.address as delivery, "
                  "aa2.provider_name, aa1.person_name, aa1.match_score "
                  "FROM all_addresses aa1 "
                  "INNER JOIN all_addresses aa2 ON aa1.matched_with_id = aa2.id "
                  "WHERE aa1.source_type = 'meal_request' "
                  "AND aa2.source_type = 'donation' "
                  "AND aa1.match_status = 'matched' "
                  "ORDER BY aa1.match_score DESC");

    if (!query.exec()) {
        QMessageBox::critical(this, "Database Error",
                              "Error retrieving matched deliveries: " + query.lastError().text());
        return;
    }

    int matchCount = 0;
    while (query.next()) {
        int deliveryId = query.value(0).toInt();
        QString pickupLocation = query.value(1).toString();
        QString deliveryLocation = query.value(2).toString();
        QString providerName = query.value(3).toString();
        QString personName = query.value(4).toString();
        int matchScore = query.value(5).toInt();

        QString displayText = QString("📦 %1 meals from '%2' → '%3' (Score: %4)")
                                  .arg(personName)
                                  .arg(pickupLocation.length() > 20 ? pickupLocation.left(20) + "..." : pickupLocation)
                                  .arg(deliveryLocation.length() > 20 ? deliveryLocation.left(20) + "..." : deliveryLocation)
                                  .arg(matchScore);

        QListWidgetItem* item = new QListWidgetItem(displayText);
        item->setData(Qt::UserRole, pickupLocation);
        item->setData(Qt::UserRole + 1, deliveryLocation);
        item->setData(Qt::UserRole + 2, providerName);
        item->setData(Qt::UserRole + 3, personName);
        item->setData(Qt::UserRole + 4, matchScore);
        item->setData(Qt::UserRole + 5, deliveryId);

        d->matchingDeliveriesList->addItem(item);
        matchCount++;
    }

    if (matchCount == 0) {
        QMessageBox::information(this, "No Matches",
                                 "No delivery requests available right now.\nCheck back soon!");
        return;
    }

    d->notificationsDialog->exec();
}




void DeliveryModule::handlePickup()
{
    QMessageBox::information(this, "Pickup", 
        "Pickup scheduling feature coming soon!");
}

