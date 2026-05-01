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
    setMinimumSize(900, 680);
    setStyleSheet("QMainWindow { background-color: #fffdf8; }");

    QWidget* central = new QWidget();
    setCentralWidget(central);

    QVBoxLayout* layout = new QVBoxLayout(central);
    layout->setAlignment(Qt::AlignHCenter);
    layout->setSpacing(24);
    layout->setContentsMargins(44, 34, 44, 34);

    QLabel* header = new QLabel("People in Need");
    header->setFont(QFont("Arial", 40, QFont::Bold));
    header->setAlignment(Qt::AlignCenter);
    header->setStyleSheet("color: #20242a; background-color: transparent; padding: 20px;");

    QWidget* foodPictures = new QWidget();
    foodPictures->setStyleSheet("background: transparent;");
    QHBoxLayout* foodLayout = new QHBoxLayout(foodPictures);
    foodLayout->setSpacing(14);
    foodLayout->setContentsMargins(0, 0, 0, 0);

    auto createFoodPicture = [](const QString& label, const QString& color) {
        QWidget* card = new QWidget();
        card->setMinimumSize(170, 132);
        card->setStyleSheet(
            "QWidget {"
            "   background-color: #ffffff;"
            "   border: 1px solid #e7eaee;"
            "   border-radius: 24px;"
            "}"
        );
        QVBoxLayout* cardLayout = new QVBoxLayout(card);
        cardLayout->setContentsMargins(14, 12, 14, 12);
        QLabel* photoLabel = new QLabel(card);
        photoLabel->setFixedSize(118, 66);
        photoLabel->setStyleSheet(
            "background-color: qlineargradient(x1:0, y1:0, x2:1, y2:1, stop:0 #fff8e9, stop:0.45 #ffc66d, stop:1 #cf5838);"
            "border-radius: 18px;"
        );
        QLabel* plate = new QLabel(photoLabel);
        plate->setGeometry(32, 11, 54, 44);
        plate->setStyleSheet("background-color: #fffdf8; border-radius: 22px;");
        QLabel* food = new QLabel(plate);
        food->setGeometry(13, 10, 28, 22);
        food->setStyleSheet(QString("background-color: %1; border-radius: 11px;").arg(color));
        QLabel* textLabel = new QLabel(label);
        textLabel->setWordWrap(true);
        textLabel->setMinimumHeight(34);
        textLabel->setAlignment(Qt::AlignCenter);
        textLabel->setStyleSheet("color: #68707a; font-size: 13px; font-weight: 800; background: transparent; border: none;");
        cardLayout->addWidget(photoLabel, 0, Qt::AlignCenter);
        cardLayout->addWidget(textLabel);
        return card;
    };

    foodLayout->addWidget(createFoodPicture("Warm meals", "#20a675"));
    foodLayout->addWidget(createFoodPicture("Fresh plates", "#ef3038"));
    foodLayout->addWidget(createFoodPicture("Daily food", "#ff9d18"));

    QString buttonStyle =
        "QPushButton {"
        "   background-color: #ffffff;"
        "   border: 2px solid #e7eaee;"
        "   border-radius: 28px;"
        "   padding: 17px 34px;"
        "   font-size: 18px;"
        "   font-weight: 900;"
        "   color: #20242a;"
        "}"
        "QPushButton:hover {"
        "   background-color: #fff0e6;"
        "   border-color: #ef3038;"
        "   color: #d92731;"
        "}"
        "QPushButton:pressed {"
        "   background-color: #ffe9d9;"
        "}";

    QPushButton* signupBtn = new QPushButton("Sign Up");
    signupBtn->setMinimumWidth(320);
    signupBtn->setStyleSheet(buttonStyle);
    signupBtn->setCursor(Qt::PointingHandCursor);

    QPushButton* requestBtn = new QPushButton("Request Meals");
    requestBtn->setMinimumWidth(320);
    requestBtn->setStyleSheet(buttonStyle);
    requestBtn->setCursor(Qt::PointingHandCursor);

    layout->addStretch();
    layout->addWidget(header);
    layout->addWidget(foodPictures, 0, Qt::AlignCenter);
    layout->addWidget(signupBtn);
    layout->addWidget(requestBtn);
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
    d->signwindow->setMinimumSize(620, 650);
    d->signwindow->setStyleSheet(
        "QMainWindow { background-color: #fffdf8; }"
        "QLabel { color: #20242a; font-size: 15px; font-weight: 800; }"
        "QLineEdit { background: white; border: 2px solid #e7eaee; border-radius: 18px; padding: 12px 14px; color: #20242a; font-size: 16px; }"
        "QLineEdit:focus { border-color: #ef3038; }"
        "QPushButton { background-color: #ef3038; color: #20242a; border: none; border-radius: 24px; padding: 13px 26px; font-size: 16px; font-weight: 900; }"
        "QPushButton:hover { background-color: #d92731; color: white; }"
    );

    QWidget* central = new QWidget();
    d->signwindow->setCentralWidget(central);

    QVBoxLayout* layout = new QVBoxLayout(central);
    layout->setAlignment(Qt::AlignHCenter);
    layout->setSpacing(12);
    layout->setContentsMargins(38, 30, 38, 30);

    // Name
    QLabel* nameLabel = new QLabel("Enter Your Name:");
    d->peoplename = new QLineEdit();
    d->peoplename->setPlaceholderText("Your Name");
    d->peoplename->setMinimumSize(500, 52);

    // ID
    QLabel* idLabel = new QLabel("Enter Your ID:");
    d->peopleid = new QLineEdit();
    d->peopleid->setPlaceholderText("Your ID");
    d->peopleid->setMinimumSize(500, 52);

    QLabel* addressLabelLabel = new QLabel("Enter Your Address:");
    d->peopleaddress1 = new QLineEdit();
    d->peopleaddress1->setPlaceholderText("Your Address");
    d->peopleaddress1->setMinimumSize(500, 52);

    // Contact
    QLabel* contactLabel = new QLabel("Enter Your Contact Info:");
    d->peoplecontact = new QLineEdit();
    d->peoplecontact->setPlaceholderText("Your Contact Info");
    d->peoplecontact->setMinimumSize(500, 52);

    // Notes
    QLabel* noteLabel = new QLabel("Additional notes or dietary restrictions:");
    noteLabel->setWordWrap(true);
    noteLabel->setMinimumWidth(500);
    d->peoplenote = new QLineEdit();
    d->peoplenote->setPlaceholderText("Additional Notes");
    d->peoplenote->setMinimumSize(500, 52);

    QPushButton* submitBtn = new QPushButton("Submit");
    submitBtn->setMinimumSize(180, 52);
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
    d->mealDialog->setMinimumSize(560, 560);
    d->mealDialog->setStyleSheet(
        "QDialog { background-color: #fffdf8; }"
        "QLabel { color: #20242a; font-size: 15px; font-weight: 800; }"
        "QLineEdit { background: white; border: 2px solid #e7eaee; border-radius: 18px; padding: 12px 14px; color: #20242a; font-size: 16px; }"
        "QLineEdit:focus { border-color: #ef3038; }"
        "QPushButton { background-color: #ef3038; color: #20242a; border: none; border-radius: 24px; padding: 13px 26px; font-size: 16px; font-weight: 900; }"
        "QPushButton:hover { background-color: #d92731; color: white; }"
    );

    QVBoxLayout* layout = new QVBoxLayout(d->mealDialog);
    layout->setSpacing(12);
    layout->setContentsMargins(34, 28, 34, 28);

    QLabel* mealLabel = new QLabel("How many meals do you need?");
    mealLabel->setFont(QFont("Arial", 18, QFont::Bold));
    mealLabel->setAlignment(Qt::AlignCenter);
    mealLabel->setStyleSheet("color: #20242a;");

    d->mealNumberLine = new QLineEdit();
    d->mealNumberLine->setPlaceholderText("Number of meals");
    d->mealNumberLine->setMinimumSize(480, 52);
    d->mealNumberLine->setAlignment(Qt::AlignCenter);

    QLabel* cityLabel = new QLabel("Enter Your City:");
    d->city = new QLineEdit();
    d->city->setPlaceholderText("Your City");
    d->city->setMinimumSize(480, 52);

    QLabel* streetLabel = new QLabel("Enter Your Street:");
    d->street = new QLineEdit();
    d->street->setPlaceholderText("Your Street");
    d->street->setMinimumSize(480, 52);

    QLabel* addressDetailsLabel = new QLabel("Building/Floor/Landmark:");
    d->addressDetails = new QLineEdit();
    d->addressDetails->setPlaceholderText("e.g. Building 5, Floor 3");
    d->addressDetails->setMinimumSize(480, 52);

    QPushButton* submitBtn = new QPushButton("Submit");
    submitBtn->setMinimumSize(180, 52);
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
    // Updated validation.
    if (d->mealNumberLine->text().trimmed().isEmpty() ||
        d->city->text().trimmed().isEmpty() ||
        d->street->text().trimmed().isEmpty() ||
        d->mealNumberLine->text().toInt() <= 0) {
        QMessageBox::warning(this, "Missing Information", "Please fill in all required fields.");
        return;
    }

    // Combine city and street for matching.
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

    // Updated meal_requests insert.
    QSqlQuery insertQuery;
    insertQuery.prepare("INSERT INTO meal_requests (person_id, meal_count, address) "
                        "VALUES (:person_id, :meal_count, :address)");
    insertQuery.bindValue(":person_id", personId);
    insertQuery.bindValue(":meal_count", d->mealNumberLine->text().trimmed().toInt());
    insertQuery.bindValue(":address", fullAddress);

    if (insertQuery.exec()) {
        // Updated all_addresses insert with new columns.
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
    logoutBtn->setMinimumWidth(260);
    logoutBtn->setCursor(Qt::PointingHandCursor);
    logoutBtn->setStyleSheet(
        "QPushButton {"
        "   background-color: #ffffff;"
        "   border: 2px solid #e7eaee;"
        "   border-radius: 28px;"
        "   padding: 10px;"
        "   font-size: 14px;"
        "   font-weight: 900;"
        "   color: #d92731;"
        "   margin-top: 20px;"
        "}"
        "QPushButton:hover {"
        "   background-color: #fff0e6;"
        "   border-color: #ef3038;"
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
