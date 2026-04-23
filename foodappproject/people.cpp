#include "people.h"
#include "database.h"
#include <QWidget>
#include <QVBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QLineEdit>
#include <QDialog>
#include <QFormLayout>
#include <QMessageBox>
#include <QSqlQuery>
#include <QSqlError>
#include <QFont>
#include <QDebug>

class PeopleModulePrivate {
public:
    QMainWindow *signwindow = nullptr;
    QLineEdit *peoplename = nullptr;
    QLineEdit *peopleid = nullptr;
    QLineEdit *peopleaddress1 = nullptr;
    QLineEdit *peoplecontact = nullptr;
    QLineEdit *peoplenote = nullptr;
    
    QDialog *mealDialog = nullptr;
    QLineEdit *mealNumberLine = nullptr;
    QLineEdit *peopleaddress = nullptr;
    QLineEdit *city = nullptr;
    QLineEdit *street = nullptr;
    QLineEdit *addressDetails = nullptr;
};

PeopleModule::PeopleModule(QWidget *parent)
    : QMainWindow(parent), d(new PeopleModulePrivate())
{
    setupUI();
}

PeopleModule::~PeopleModule()
{
    delete d;
}

void PeopleModule::setupUI()
{
    setWindowTitle("People in Need");
    setMinimumSize(800, 600);
    setStyleSheet("QMainWindow { background-color: #efe4d0; }");

    QWidget* central = new QWidget();
    setCentralWidget(central);

    QVBoxLayout* layout = new QVBoxLayout(central);
    layout->setAlignment(Qt::AlignCenter);
    layout->setSpacing(40);

    QLabel* header = new QLabel("Request Meals and Assistance");
    header->setFont(QFont("Arial", 40, QFont::Bold));
    header->setAlignment(Qt::AlignCenter);
    header->setStyleSheet("color: #813e15; background-color: transparent; padding: 20px;");

    QPushButton* signupBtn = new QPushButton("Sign Up");
    signupBtn->setFixedSize(500, 200);
    signupBtn->setCursor(Qt::PointingHandCursor);
    signupBtn->setStyleSheet(
        "QPushButton {"
        "   background-color: #f4ece7;"
        "   border: 2px solid #813e15;"
        "   border-radius: 28px;"
        "   padding: 15px;"
        "   font-size: 30px;"
        "   font-weight: bold;"
        "   color: #ba6c3b;"
        "}"
        "QPushButton:hover {"
        "   background-color: #e8cebe;"
        "   border-color: #4b240c;"
        "}"
    );

    QPushButton* requestBtn = new QPushButton("Request Meals");
    requestBtn->setFixedSize(500, 200);
    requestBtn->setCursor(Qt::PointingHandCursor);
    requestBtn->setStyleSheet(
        "QPushButton {"
        "   background-color: #f4ece7;"
        "   border: 2px solid #813e15;"
        "   border-radius: 28px;"
        "   padding: 15px;"
        "   font-size: 30px;"
        "   font-weight: bold;"
        "   color: #ba6c3b;"
        "}"
        "QPushButton:hover {"
        "   background-color: #e8cebe;"
        "   border-color: #4b240c;"
        "}"
    );

    layout->addWidget(header);
    layout->addStretch();
    layout->addWidget(signupBtn, 0, Qt::AlignCenter);
    layout->addStretch();
    layout->addWidget(requestBtn, 0, Qt::AlignCenter);
    layout->addStretch();

    setupSignUpDialog();
    setupMealRequestDialog();

    connect(signupBtn, &QPushButton::clicked, this, &PeopleModule::handleSignUp);
    connect(requestBtn, &QPushButton::clicked, this, &PeopleModule::handleMealRequest);
}

void PeopleModule::setupSignUpDialog()
{
    d->signwindow = new QMainWindow();
    d->signwindow->setWindowTitle("Sign Up for Assistance");
    d->signwindow->setMinimumSize(600, 400);
    d->signwindow->setStyleSheet("QMainWindow { background-color: #efe4d0; }");

    QWidget* central = new QWidget();
    d->signwindow->setCentralWidget(central);

    QVBoxLayout* layout = new QVBoxLayout(central);
    layout->setAlignment(Qt::AlignCenter);
    layout->setSpacing(30);

    // Name
    QLabel* nameLabel = new QLabel("Enter Your Name:");
    nameLabel->setFont(QFont("Arial", 18, QFont::Bold));
    d->peoplename = new QLineEdit();
    d->peoplename->setPlaceholderText("Your Name");
    d->peoplename->setStyleSheet("font-size: 18px;");

    // ID
    QLabel* idLabel = new QLabel("Enter Your ID:");
    idLabel->setFont(QFont("Arial", 18, QFont::Bold));
    d->peopleid = new QLineEdit();
    d->peopleid->setPlaceholderText("Your ID");
    d->peopleid->setStyleSheet("font-size: 18px;");

    QLabel* addressLabelLabel = new QLabel("Enter Your Adress:");
    addressLabelLabel->setFont(QFont("Arial", 18, QFont::Bold));
    d->peopleaddress1 = new QLineEdit();
    d->peopleaddress1->setPlaceholderText("Your Address");
    d->peopleaddress1->setStyleSheet("font-size: 18px;");

    // Contact
    QLabel* contactLabel = new QLabel("Enter Your Contact Info:");
    contactLabel->setFont(QFont("Arial", 18, QFont::Bold));
    d->peoplecontact = new QLineEdit();
    d->peoplecontact->setPlaceholderText("Your Contact Info");
    d->peoplecontact->setStyleSheet("font-size: 18px;");

    // Notes
    QLabel* noteLabel = new QLabel("Enter any additional notes or dietary restrictions:");
    noteLabel->setFont(QFont("Arial", 18, QFont::Bold));
    d->peoplenote = new QLineEdit();
    d->peoplenote->setPlaceholderText("Additional Notes");
    d->peoplenote->setStyleSheet("font-size: 18px;");

    QPushButton* submitBtn = new QPushButton("Submit");
    submitBtn->setFixedSize(150, 50);
    submitBtn->setCursor(Qt::PointingHandCursor);
    submitBtn->setStyleSheet(
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
        "}"
    );

    layout->addWidget(nameLabel);
    layout->addWidget(d->peoplename);
    layout->addWidget(idLabel);
    layout->addWidget(d->peopleid);
    layout->addWidget(addressLabelLabel);
    layout->addWidget(d->peopleaddress1);
    layout->addWidget(contactLabel);
    layout->addWidget(d->peoplecontact);
    layout->addWidget(noteLabel);
    layout->addWidget(d->peoplenote);
    layout->addWidget(submitBtn);

    connect(submitBtn, &QPushButton::clicked, this, &PeopleModule::submitSignUp);
}

void PeopleModule::setupMealRequestDialog()
{
    d->mealDialog = new QDialog(this);
    d->mealDialog->setWindowTitle("Request Meals");
    d->mealDialog->setMinimumSize(400, 200);
    d->mealDialog->setStyleSheet("background-color: #efe4d0;");

    QVBoxLayout* layout = new QVBoxLayout(d->mealDialog);

    QLabel* mealLabel = new QLabel("How many meals do you need?");
    mealLabel->setFont(QFont("Arial", 18, QFont::Bold));
    mealLabel->setAlignment(Qt::AlignCenter);
    mealLabel->setStyleSheet("color: #813e15;");

    d->mealNumberLine = new QLineEdit();
    d->mealNumberLine->setPlaceholderText("Enter number of meals in numerals");
    d->mealNumberLine->setMinimumHeight(60);
    d->mealNumberLine->setAlignment(Qt::AlignCenter);
    d->mealNumberLine->setStyleSheet("font-size: 18px;");

    QLabel* cityLabel = new QLabel("Enter Your City:");
    cityLabel->setStyleSheet("color: #813e15; font-size: 18px; font-weight: bold;");
    d->city = new QLineEdit();
    d->city->setPlaceholderText("Your City");
    d->city->setMinimumHeight(60);

    QLabel* streetLabel = new QLabel("Enter Your Street:");
    streetLabel->setStyleSheet("color: #813e15; font-size: 18px; font-weight: bold;");
    d->street = new QLineEdit();
    d->street->setPlaceholderText("Your Street");
    d->street->setMinimumHeight(60);

    QLabel* addressDetailsLabel = new QLabel("Building/Floor/Landmark:");
    addressDetailsLabel->setStyleSheet("color: #813e15; font-size: 18px; font-weight: bold;");
    d->addressDetails = new QLineEdit();
    d->addressDetails->setPlaceholderText("e.g. Building 5, Floor 3");
    d->addressDetails->setMinimumHeight(60);

    QPushButton* submitBtn = new QPushButton("Submit");
    submitBtn->setFixedSize(150, 50);
    submitBtn->setCursor(Qt::PointingHandCursor);
    submitBtn->setStyleSheet(
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
        "}"
        );


    layout->addWidget(mealLabel);
    layout->addWidget(d->mealNumberLine);
    layout->addWidget(cityLabel);
    layout->addWidget(d->city);
    layout->addWidget(streetLabel);
    layout->addWidget(d->street);
    layout->addWidget(addressDetailsLabel);
    layout->addWidget(d->addressDetails);
    layout->addWidget(submitBtn);

    connect(submitBtn, &QPushButton::clicked, this, &PeopleModule::submitMealRequest);
}

void PeopleModule::handleSignUp()
{
    d->signwindow->show();
}

void PeopleModule::handleMealRequest()
{
    d->mealDialog->exec();
}

void PeopleModule::submitSignUp()
{
    if (d->peoplename->text().trimmed().isEmpty() ||
        d->peopleid->text().trimmed().isEmpty() ||
        d->peopleaddress1->text().trimmed().isEmpty() ||
        d->peoplecontact->text().trimmed().isEmpty()) {
        QMessageBox::warning(this, "Missing Information", "Please fill in all required fields.");
        return;
    }

    QSqlQuery query;
    query.prepare("INSERT INTO people_sign (name, people_id, address, contact, notes) "
                  "VALUES (:name, :people_id, :address, :contact, :notes)");
    query.bindValue(":name", d->peoplename->text().trimmed());
    query.bindValue(":people_id", d->peopleid->text().trimmed());  // Changed from :id
    query.bindValue(":address", d->peopleaddress1->text().trimmed());
    query.bindValue(":contact", d->peoplecontact->text().trimmed());  // Changed from :contact_info
    query.bindValue(":notes", d->peoplenote->text().trimmed());

    if (query.exec()) {
        QMessageBox::information(this, "Success", "You have signed up successfully.");
        d->peoplename->clear();
        d->peopleid->clear();
        d->peopleaddress1->clear();
        d->peoplecontact->clear();
        d->peoplenote->clear();
        d->signwindow->close();
    } else {
        QMessageBox::critical(this, "Database Error", query.lastError().text());
    }
}

void PeopleModule::submitMealRequest()
{
    // ✅ updated validation
    if (d->mealNumberLine->text().trimmed().isEmpty() ||
        d->city->text().trimmed().isEmpty() ||
        d->street->text().trimmed().isEmpty() ||
        d->mealNumberLine->text().toInt() <= 0) {
        QMessageBox::warning(this, "Missing Information", "Please fill in all required fields.");
        return;
    }

    // ✅ combine city + street for matching
    QString fullAddress = d->city->text().trimmed() + ", " +
                          d->street->text().trimmed();

    // Get most recent person who signed up
    QSqlQuery getIdQuery;
    getIdQuery.exec("SELECT id, name FROM people_sign ORDER BY id DESC LIMIT 1");

    int personId = -1;
    QString personName;
    if (getIdQuery.next()) {
        personId = getIdQuery.value(0).toInt();
        personName = getIdQuery.value(1).toString();
    }

    if (personId == -1) {
        QMessageBox::warning(this, "No Sign-up Found", "Please sign up first before requesting meals.");
        d->mealNumberLine->clear();
        d->mealDialog->close();
        return;
    }

    // ✅ updated meal_requests insert
    QSqlQuery insertQuery;
    insertQuery.prepare("INSERT INTO meal_requests (person_id, meal_count, address) "
                        "VALUES (:person_id, :meal_count, :address)");
    insertQuery.bindValue(":person_id", personId);
    insertQuery.bindValue(":meal_count", d->mealNumberLine->text().trimmed().toInt());
    insertQuery.bindValue(":address", fullAddress);

    if (insertQuery.exec()) {
        // ✅ updated all_addresses insert with new columns
        QSqlQuery addrQuery;
        addrQuery.prepare("INSERT INTO all_addresses "
                          "(source_type, source_id, person_name, provider_name, "
                          "address, city, street, address_details, details) "
                          "VALUES "
                          "('meal_request', :source_id, :person_name, NULL, "
                          ":address, :city, :street, :address_details, :details)");
        addrQuery.bindValue(":source_id", personId);
        addrQuery.bindValue(":person_name", personName);
        addrQuery.bindValue(":address", fullAddress);
        addrQuery.bindValue(":city", d->city->text().trimmed());
        addrQuery.bindValue(":street", d->street->text().trimmed());
        addrQuery.bindValue(":address_details", d->addressDetails->text().trimmed());
        addrQuery.bindValue(":details", QString("%1 meals requested")
                                            .arg(d->mealNumberLine->text().trimmed()));

        if (addrQuery.exec()) {
            DatabaseManager::matchAddresses();
        }

        QMessageBox::information(this, "Success", "Your meal request has been submitted.");
        d->mealNumberLine->clear();
        d->city->clear();
        d->street->clear();
        d->addressDetails->clear();
        d->mealDialog->close();
    } else {
        QMessageBox::critical(this, "Database Error",
                              "Error: " + insertQuery.lastError().text());
    }
}
