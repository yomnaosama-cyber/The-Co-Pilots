#include "people.h"
#include "database.h"
#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
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
#include <QSettings>

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
    setStyleSheet("QMainWindow { background-color: #fff6f8; }");

    QWidget* central = new QWidget();
    setCentralWidget(central);

    QVBoxLayout* layout = new QVBoxLayout(central);
    layout->setAlignment(Qt::AlignCenter);
    layout->setSpacing(28);
    layout->setContentsMargins(44, 34, 44, 34);

    QLabel* header = new QLabel("Request Meals and Assistance");
    header->setFont(QFont("Arial", 40, QFont::Bold));
    header->setAlignment(Qt::AlignCenter);
    header->setStyleSheet("color: #231f20; background-color: transparent; padding: 20px;");

    QWidget* foodPictures = new QWidget();
    foodPictures->setStyleSheet("background: transparent;");
    QHBoxLayout* foodLayout = new QHBoxLayout(foodPictures);
    foodLayout->setSpacing(18);
    foodLayout->setContentsMargins(0, 0, 0, 0);

    auto createFoodPicture = [](const QString& icon, const QString& label) {
        QWidget* card = new QWidget();
        card->setFixedSize(150, 128);
        card->setStyleSheet(
            "QWidget {"
            "   background-color: #ffffff;"
            "   border: 1px solid #f2d9de;"
            "   border-radius: 24px;"
            "}"
        );
        QVBoxLayout* cardLayout = new QVBoxLayout(card);
        cardLayout->setContentsMargins(14, 12, 14, 12);
        QLabel* iconLabel = new QLabel(icon);
        iconLabel->setAlignment(Qt::AlignCenter);
        iconLabel->setStyleSheet("font-size: 42px; background: transparent; border: none;");
        QLabel* textLabel = new QLabel(label);
        textLabel->setAlignment(Qt::AlignCenter);
        textLabel->setStyleSheet("color: #77676c; font-size: 13px; font-weight: 800; background: transparent; border: none;");
        cardLayout->addWidget(iconLabel);
        cardLayout->addWidget(textLabel);
        return card;
    };

    QString largeButtonStyle =
        "QPushButton {"
        "   background-color: #ffffff;"
        "   border: 2px solid #f2d9de;"
        "   border-radius: 30px;"
        "   padding: 18px;"
        "   font-size: 30px;"
        "   font-weight: 900;"
        "   color: #231f20;"
        "}"
        "QPushButton:hover {"
        "   background-color: #fff0f3;"
        "   border-color: #f28fa0;"
        "   color: #df6076;"
        "}"
        "QPushButton:pressed {"
        "   background-color: #f9c5ce;"
        "}";

    QPushButton* signupBtn = new QPushButton("Sign Up");
    signupBtn->setFixedSize(500, 200);
    signupBtn->setCursor(Qt::PointingHandCursor);
    signupBtn->setStyleSheet(largeButtonStyle);

    QPushButton* requestBtn = new QPushButton("Request Meals");
    requestBtn->setFixedSize(500, 200);
    requestBtn->setCursor(Qt::PointingHandCursor);
    requestBtn->setStyleSheet(largeButtonStyle);

    layout->addWidget(header);
    foodLayout->addWidget(createFoodPicture("🍲", "Warm meals"));
    foodLayout->addWidget(createFoodPicture("🥗", "Fresh plates"));
    foodLayout->addWidget(createFoodPicture("🥖", "Daily food"));
    layout->addWidget(foodPictures, 0, Qt::AlignCenter);
    layout->addStretch();
    layout->addWidget(signupBtn, 0, Qt::AlignCenter);
    layout->addStretch();
    layout->addWidget(requestBtn, 0, Qt::AlignCenter);
    layout->addStretch();

    setupSignUpDialog();
    setupMealRequestDialog();

    connect(signupBtn, &QPushButton::clicked, this, &PeopleModule::handleSignUp);
    connect(requestBtn, &QPushButton::clicked, this, &PeopleModule::handleMealRequest);

    addLogoutButton();
}

void PeopleModule::setupSignUpDialog()
{
    d->signwindow = new QMainWindow();
    d->signwindow->setWindowTitle("Sign Up for Assistance");
    d->signwindow->setMinimumSize(600, 400);
    d->signwindow->setStyleSheet(
        "QMainWindow { background-color: #fff6f8; }"
        "QLabel { color: #231f20; font-weight: 800; }"
        "QLineEdit { background: white; border: 2px solid #f2d9de; border-radius: 18px; padding: 12px; color: #231f20; }"
        "QLineEdit:focus { border-color: #f28fa0; }"
        "QPushButton { background-color: #f28fa0; color: #231f20; border: none; border-radius: 24px; padding: 12px 24px; font-weight: 900; }"
        "QPushButton:hover { background-color: #df6076; color: white; }"
    );

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
    d->mealDialog->setStyleSheet(
        "QDialog { background-color: #fff6f8; }"
        "QLabel { color: #231f20; font-weight: 800; }"
        "QLineEdit { background: white; border: 2px solid #f2d9de; border-radius: 18px; padding: 12px; color: #231f20; }"
        "QLineEdit:focus { border-color: #f28fa0; }"
        "QPushButton { background-color: #f28fa0; color: #231f20; border: none; border-radius: 24px; padding: 12px 24px; font-weight: 900; }"
        "QPushButton:hover { background-color: #df6076; color: white; }"
    );

    QVBoxLayout* layout = new QVBoxLayout(d->mealDialog);

    QLabel* mealLabel = new QLabel("How many meals do you need?");
    mealLabel->setFont(QFont("Arial", 18, QFont::Bold));
    mealLabel->setAlignment(Qt::AlignCenter);
    mealLabel->setStyleSheet("color: #231f20;");

    d->mealNumberLine = new QLineEdit();
    d->mealNumberLine->setPlaceholderText("Enter number of meals in numerals");
    d->mealNumberLine->setMinimumHeight(60);
    d->mealNumberLine->setAlignment(Qt::AlignCenter);
    d->mealNumberLine->setStyleSheet("font-size: 18px;");

    QLabel* cityLabel = new QLabel("Enter Your City:");
    cityLabel->setStyleSheet("color: #231f20; font-size: 18px; font-weight: bold;");
    d->city = new QLineEdit();
    d->city->setPlaceholderText("Your City");
    d->city->setMinimumHeight(60);

    QLabel* streetLabel = new QLabel("Enter Your Street:");
    streetLabel->setStyleSheet("color: #231f20; font-size: 18px; font-weight: bold;");
    d->street = new QLineEdit();
    d->street->setPlaceholderText("Your Street");
    d->street->setMinimumHeight(60);

    QLabel* addressDetailsLabel = new QLabel("Building/Floor/Landmark:");
    addressDetailsLabel->setStyleSheet("color: #231f20; font-size: 18px; font-weight: bold;");
    d->addressDetails = new QLineEdit();
    d->addressDetails->setPlaceholderText("e.g. Building 5, Floor 3");
    d->addressDetails->setMinimumHeight(60);

    QPushButton* submitBtn = new QPushButton("Submit");
    submitBtn->setFixedSize(150, 50);
    submitBtn->setCursor(Qt::PointingHandCursor);


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
        savePersonState(d->peopleid->text().trimmed());
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

void PeopleModule::addLogoutButton()
{
    QWidget* central = this->centralWidget();
    if (!central) return;
    
    QVBoxLayout* mainLayout = qobject_cast<QVBoxLayout*>(central->layout());
    if (!mainLayout) return;
    
    QPushButton* logoutBtn = new QPushButton("Logout & Switch Account");
    logoutBtn->setCursor(Qt::PointingHandCursor);
    logoutBtn->setStyleSheet(
        "QPushButton {"
        "   background-color: #ffffff;"
        "   border: 2px solid #f2d9de;"
        "   border-radius: 28px;"
        "   padding: 10px;"
        "   font-size: 14px;"
        "   font-weight: 900;"
        "   color: #df6076;"
        "   margin-top: 20px;"
        "}"
        "QPushButton:hover {"
        "   background-color: #fff0f3;"
        "   border-color: #f28fa0;"
        "}"
    );
    
    mainLayout->addWidget(logoutBtn);
    connect(logoutBtn, &QPushButton::clicked, this, &PeopleModule::handleLogout);
}

void PeopleModule::handleLogout()
{
    QSettings settings;
    settings.remove("lastModule");
    settings.remove("lastModuleData");
    
    QMessageBox::information(this, "Logged Out", "You have been logged out successfully.");
    this->close();
    
    QWidget* parent = this->parentWidget();
    while (parent && !parent->isWindow()) {
        parent = parent->parentWidget();
    }
    if (parent) {
        parent->show();
    }
}

void PeopleModule::savePersonState(const QString& personId)
{
    QSettings settings;
    settings.setValue("lastModule", "people");
    settings.setValue("lastModuleData", personId);
}
