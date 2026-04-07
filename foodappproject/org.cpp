#include "org.h"
#include "database.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QLineEdit>
#include <QTextEdit>
#include <QComboBox>
#include <QFormLayout>
#include <QStackedWidget>
#include <QMessageBox>
#include <QSqlQuery>
#include <QSqlError>
#include <QFont>

class OrgModulePrivate {
public:
    QStackedWidget *stack = nullptr;
    
    // Restaurant page
    QLineEdit *resName = nullptr;
    QLineEdit *resLocation = nullptr;
    QComboBox *resFoodType = nullptr;
    QLineEdit *resContact = nullptr;
    
    // Organization page
    QLineEdit *orgName = nullptr;
    QComboBox *orgType = nullptr;
    QLineEdit *orgLocation = nullptr;
    QTextEdit *orgPurpose = nullptr;
    QLineEdit *orgContact = nullptr;
    
    // Donation page
    QLineEdit *providerName = nullptr;
    QComboBox *providerRole = nullptr;
    QLineEdit *foodAmount = nullptr;
    QLineEdit *donationType = nullptr;
    QLineEdit *donationLocation = nullptr;
    QComboBox *deliveryMethod = nullptr;
};

OrgModule::OrgModule(QWidget *parent)
    : QDialog(parent), d(new OrgModulePrivate())
{
    setupUI();
}

OrgModule::~OrgModule()
{
    delete d;
}

void OrgModule::setupUI()
{
    setWindowTitle("Provider Registration");
    setMinimumSize(560, 620);
    setStyleSheet(
        "QDialog { background-color: #efe4d0; }"
        "QLabel { color: #813e15; font-size: 14px; }"
        "QLineEdit, QTextEdit, QComboBox {"
        "    background-color: white;"
        "    color: black;"
        "    border: 2px solid #813e15;"
        "    border-radius: 10px;"
        "    padding: 6px;"
        "}"
        "QPushButton {"
        "    background-color: #813e15;"
        "    color: white;"
        "    border-radius: 12px;"
        "    padding: 10px;"
        "    font-weight: bold;"
        "}"
    );

    QVBoxLayout* mainLayout = new QVBoxLayout(this);

    QLabel* title = new QLabel("Community Food Connect");
    title->setAlignment(Qt::AlignCenter);
    title->setStyleSheet("font-size: 24px; font-weight: 700; color: #813e15;");
    mainLayout->addWidget(title);

    // Tab buttons
    QHBoxLayout* switchLayout = new QHBoxLayout();
    QPushButton* restaurantBtn = new QPushButton("Restaurant");
    QPushButton* organizationBtn = new QPushButton("Organization");
    QPushButton* donateBtn = new QPushButton("Donate Food Details");

    switchLayout->addWidget(restaurantBtn);
    switchLayout->addWidget(organizationBtn);
    switchLayout->addWidget(donateBtn);
    mainLayout->addLayout(switchLayout);

    d->stack = new QStackedWidget(this);
    mainLayout->addWidget(d->stack);

    setupRestaurantPage();
    setupOrganizationPage();
    setupDonationPage();

    connect(restaurantBtn, &QPushButton::clicked, this, &OrgModule::onRestaurantTab);
    connect(organizationBtn, &QPushButton::clicked, this, &OrgModule::onOrganizationTab);
    connect(donateBtn, &QPushButton::clicked, this, &OrgModule::onDonationTab);
}

void OrgModule::setupRestaurantPage()
{
    QWidget* page = new QWidget();
    QVBoxLayout* layout = new QVBoxLayout(page);
    QFormLayout* form = new QFormLayout();

    d->resName = new QLineEdit();
    d->resLocation = new QLineEdit();
    d->resFoodType = new QComboBox();
    d->resContact = new QLineEdit();

    d->resFoodType->addItems({
        "Bakery", "Fast Food", "Groceries", "Home Meals", 
        "Restaurant Meals", "Vegetarian", "Other"
    });

    form->addRow("Restaurant Name:", d->resName);
    form->addRow("Location:", d->resLocation);
    form->addRow("Food Type:", d->resFoodType);
    form->addRow("Contact Info:", d->resContact);

    QPushButton* submitBtn = new QPushButton("Register Restaurant");
    connect(submitBtn, &QPushButton::clicked, this, &OrgModule::submitRestaurant);

    layout->addLayout(form);
    layout->addWidget(submitBtn);
    d->stack->addWidget(page);
}

void OrgModule::setupOrganizationPage()
{
    QWidget* page = new QWidget();
    QVBoxLayout* layout = new QVBoxLayout(page);
    QFormLayout* form = new QFormLayout();

    d->orgName = new QLineEdit();
    d->orgType = new QComboBox();
    d->orgLocation = new QLineEdit();
    d->orgPurpose = new QTextEdit();
    d->orgContact = new QLineEdit();

    d->orgType->addItems({
        "Charity", "Community Kitchen", "Food Bank", 
        "Mosque", "NGO", "Shelter", "Other"
    });

    form->addRow("Organization Name:", d->orgName);
    form->addRow("Organization Type:", d->orgType);
    form->addRow("Location:", d->orgLocation);
    form->addRow("Purpose:", d->orgPurpose);
    form->addRow("Contact Info:", d->orgContact);

    QPushButton* submitBtn = new QPushButton("Register Organization");
    connect(submitBtn, &QPushButton::clicked, this, &OrgModule::submitOrganization);

    layout->addLayout(form);
    layout->addWidget(submitBtn);
    d->stack->addWidget(page);
}

void OrgModule::setupDonationPage()
{
    QWidget* page = new QWidget();
    QVBoxLayout* layout = new QVBoxLayout(page);
    QFormLayout* form = new QFormLayout();

    d->providerName = new QLineEdit();
    d->providerRole = new QComboBox();
    d->foodAmount = new QLineEdit();
    d->donationType = new QLineEdit();
    d->donationLocation = new QLineEdit();
    d->deliveryMethod = new QComboBox();

    d->providerRole->addItems({"Restaurant", "Organization"});
    d->deliveryMethod->addItems({"Pickup", "Delivery", "Either"});

    form->addRow("Provider Name:", d->providerName);
    form->addRow("Provider Type:", d->providerRole);
    form->addRow("Amount of Food:", d->foodAmount);
    form->addRow("Food Type:", d->donationType);
    form->addRow("Donation Location:", d->donationLocation);
    form->addRow("Delivery Method:", d->deliveryMethod);

    QPushButton* submitBtn = new QPushButton("Submit Donation Details");
    connect(submitBtn, &QPushButton::clicked, this, &OrgModule::submitDonation);

    layout->addLayout(form);
    layout->addWidget(submitBtn);
    d->stack->addWidget(page);
}

void OrgModule::onRestaurantTab()
{
    d->stack->setCurrentIndex(0);
}

void OrgModule::onOrganizationTab()
{
    d->stack->setCurrentIndex(1);
}

void OrgModule::onDonationTab()
{
    d->stack->setCurrentIndex(2);
}

void OrgModule::submitRestaurant()
{
    if (d->resName->text().trimmed().isEmpty() ||
        d->resLocation->text().trimmed().isEmpty() ||
        d->resContact->text().trimmed().isEmpty()) {
        QMessageBox::warning(this, "Missing Information", "Please fill in all restaurant fields.");
        return;
    }

    QSqlQuery query;
    query.prepare("INSERT INTO restaurants (name, location, food_type, contact_info) "
                  "VALUES (:name, :location, :food_type, :contact_info)");
    query.bindValue(":name", d->resName->text().trimmed());
    query.bindValue(":location", d->resLocation->text().trimmed());
    query.bindValue(":food_type", d->resFoodType->currentText());
    query.bindValue(":contact_info", d->resContact->text().trimmed());

    if (query.exec()) {
        QMessageBox::information(this, "Success", "Restaurant registered successfully.");
        d->resName->clear();
        d->resLocation->clear();
        d->resContact->clear();
    } else {
        QMessageBox::critical(this, "Database Error", query.lastError().text());
    }
}

void OrgModule::submitOrganization()
{
    if (d->orgName->text().trimmed().isEmpty() ||
        d->orgLocation->text().trimmed().isEmpty() ||
        d->orgPurpose->toPlainText().trimmed().isEmpty() ||
        d->orgContact->text().trimmed().isEmpty()) {
        QMessageBox::warning(this, "Missing Information", "Please fill in all organization fields.");
        return;
    }

    QSqlQuery query;
    query.prepare("INSERT INTO organizations (name, organization_type, location, purpose, contact_info) "
                  "VALUES (:name, :organization_type, :location, :purpose, :contact_info)");
    query.bindValue(":name", d->orgName->text().trimmed());
    query.bindValue(":organization_type", d->orgType->currentText());
    query.bindValue(":location", d->orgLocation->text().trimmed());
    query.bindValue(":purpose", d->orgPurpose->toPlainText().trimmed());
    query.bindValue(":contact_info", d->orgContact->text().trimmed());

    if (query.exec()) {
        QMessageBox::information(this, "Success", "Organization registered successfully.");
        d->orgName->clear();
        d->orgLocation->clear();
        d->orgPurpose->clear();
        d->orgContact->clear();
    } else {
        QMessageBox::critical(this, "Database Error", query.lastError().text());
    }
}

void OrgModule::submitDonation()
{
    if (d->providerName->text().trimmed().isEmpty() ||
        d->foodAmount->text().trimmed().isEmpty() ||
        d->donationType->text().trimmed().isEmpty() ||
        d->donationLocation->text().trimmed().isEmpty()) {
        QMessageBox::warning(this, "Missing Information", "Please fill in all donation fields.");
        return;
    }

    // Check if provider is registered
    bool registered = false;
    QSqlQuery checkQuery;

    if (d->providerRole->currentText() == "Restaurant") {
        checkQuery.prepare("SELECT id FROM restaurants WHERE name = :name LIMIT 1");
    } else {
        checkQuery.prepare("SELECT id FROM organizations WHERE name = :name LIMIT 1");
    }

    checkQuery.bindValue(":name", d->providerName->text().trimmed());

    if (checkQuery.exec() && checkQuery.next()) {
        registered = true;
    }

    if (!registered) {
        QMessageBox::warning(this, "Not Registered", 
            "This provider is not registered yet. Please sign up first.");
        return;
    }

    // Submit donation
    QSqlQuery query;
    query.prepare("INSERT INTO food_donations "
                  "(provider_name, provider_role, food_amount, food_type, donation_location, delivery_method) "
                  "VALUES (:provider_name, :provider_role, :food_amount, :food_type, :donation_location, :delivery_method)");
    query.bindValue(":provider_name", d->providerName->text().trimmed());
    query.bindValue(":provider_role", d->providerRole->currentText());
    query.bindValue(":food_amount", d->foodAmount->text().trimmed());
    query.bindValue(":food_type", d->donationType->text().trimmed());
    query.bindValue(":donation_location", d->donationLocation->text().trimmed());
    query.bindValue(":delivery_method", d->deliveryMethod->currentText());

    if (query.exec()) {
        QSqlQuery addrQuery;
        addrQuery.prepare("INSERT INTO all_addresses (source_type, source_id, provider_name, address, details) "
                          "VALUES ('donation', :source_id, :provider_name, :address, :details)");
        addrQuery.bindValue(":source_id", query.lastInsertId().toInt());
        addrQuery.bindValue(":provider_name", d->providerName->text().trimmed());
        addrQuery.bindValue(":address", d->donationLocation->text().trimmed());
        addrQuery.bindValue(":details", QString("%1 of %2 - Delivery: %3")
            .arg(d->foodAmount->text().trimmed())
            .arg(d->donationType->text().trimmed())
            .arg(d->deliveryMethod->currentText()));
            
        if (addrQuery.exec()) {
            // Run matching after successful donation insertion
            DatabaseManager::matchAddresses();
        }

        QMessageBox::information(this, "Success", "Donation details submitted successfully.");
        d->providerName->clear();
        d->foodAmount->clear();
        d->donationType->clear();
        d->donationLocation->clear();
    } else {
        QMessageBox::critical(this, "Database Error", query.lastError().text());
    }
}

