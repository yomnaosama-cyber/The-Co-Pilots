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
#include <QSettings>

class OrgModulePrivate {
public:
    QStackedWidget *stack = nullptr;

    // Restaurant page
    QLineEdit *resName = nullptr;
    QLineEdit *resCity = nullptr;
    QLineEdit *resStreet = nullptr;
    QLineEdit *resAddressDetails = nullptr;
    QComboBox *resFoodType = nullptr;
    QLineEdit *resContact = nullptr;

    // Organization page
    QLineEdit *orgName = nullptr;
    QComboBox *orgType = nullptr;
    QLineEdit *orgCity = nullptr;
    QLineEdit *orgStreet = nullptr;
    QLineEdit *orgAddressDetails = nullptr;
    QTextEdit *orgPurpose = nullptr;
    QLineEdit *orgContact = nullptr;

    // Donation page
    QLineEdit *providerName = nullptr;
    QComboBox *providerRole = nullptr;
    QLineEdit *foodAmount = nullptr;
    QLineEdit *donationType = nullptr;
    QLineEdit *donationCity = nullptr;
    QLineEdit *donationStreet = nullptr;
    QLineEdit *donationAddressDetails = nullptr;
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
    setMinimumSize(900, 740);
    setStyleSheet(
        "QDialog { background-color: #fffdf8; }"
        "QLabel { color: #20242a; font-size: 14px; font-weight: 800; }"
        "QLineEdit, QTextEdit, QComboBox {"
        "    background-color: white;"
        "    color: #20242a;"
        "    border: 2px solid #e7eaee;"
        "    border-radius: 16px;"
        "    padding: 10px;"
        "}"
        "QLineEdit:focus, QTextEdit:focus, QComboBox:focus {"
        "    border: 2px solid #ef3038;"
        "}"
        "QStackedWidget {"
        "    background-color: #ffffff;"
        "    border: 1px solid #e7eaee;"
        "    border-radius: 24px;"
        "}"
        "QPushButton {"
        "    background-color: #ef3038;"
        "    color: #20242a;"
        "    border: none;"
        "    border-radius: 22px;"
        "    padding: 11px 18px;"
        "    font-weight: 900;"
        "}"
        "QPushButton:hover {"
        "    background-color: #d92731;"
        "    color: #ffffff;"
        "}"
    );

    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(24, 24, 24, 24);
    mainLayout->setSpacing(18);

    QLabel* title = new QLabel("Community Food Connect");
    title->setAlignment(Qt::AlignCenter);
    title->setStyleSheet("font-size: 28px; font-weight: 900; color: #20242a;");
    mainLayout->addWidget(title);

    QWidget* providerPictures = new QWidget();
    providerPictures->setStyleSheet("background: transparent;");
    QHBoxLayout* providerPictureLayout = new QHBoxLayout(providerPictures);
    providerPictureLayout->setSpacing(12);
    providerPictureLayout->setContentsMargins(0, 0, 0, 0);

    auto createProviderPicture = [](const QString& icon, const QString& label) {
        QWidget* card = new QWidget();
        card->setStyleSheet(
            "QWidget {"
            "   background-color: #ffffff;"
            "   border: 1px solid #e7eaee;"
            "   border-radius: 20px;"
            "}"
        );
        QVBoxLayout* cardLayout = new QVBoxLayout(card);
        cardLayout->setContentsMargins(12, 10, 12, 10);
        cardLayout->setSpacing(5);
        QLabel* iconLabel = new QLabel(icon);
        iconLabel->setFixedSize(48, 48);
        iconLabel->setAlignment(Qt::AlignCenter);
        iconLabel->setStyleSheet("font-size: 17px; font-weight: 900; color: white; background: #ff9d18; border-radius: 24px;");
        QLabel* textLabel = new QLabel(label);
        textLabel->setWordWrap(true);
        textLabel->setMinimumHeight(32);
        textLabel->setAlignment(Qt::AlignCenter);
        textLabel->setStyleSheet("color: #68707a; font-size: 12px; font-weight: 800; background: transparent; border: none;");
        cardLayout->addWidget(iconLabel, 0, Qt::AlignCenter);
        cardLayout->addWidget(textLabel);
        return card;
    };

    providerPictureLayout->addWidget(createProviderPicture("01", "Restaurants"));
    providerPictureLayout->addWidget(createProviderPicture("02", "Organizations"));
    providerPictureLayout->addWidget(createProviderPicture("03", "Donations"));
    mainLayout->addWidget(providerPictures);

    // Tab buttons
    QHBoxLayout* switchLayout = new QHBoxLayout();
    QPushButton* restaurantBtn = new QPushButton("Restaurant");
    QPushButton* organizationBtn = new QPushButton("Organization");
    QPushButton* donateBtn = new QPushButton("Donation Details");
    restaurantBtn->setMinimumWidth(160);
    organizationBtn->setMinimumWidth(180);
    donateBtn->setMinimumWidth(190);

    switchLayout->addWidget(restaurantBtn);
    switchLayout->addWidget(organizationBtn);
    switchLayout->addWidget(donateBtn);
    mainLayout->addLayout(switchLayout);

    d->stack = new QStackedWidget(this);
    d->stack->setMinimumHeight(460);
    mainLayout->addWidget(d->stack);

    setupRestaurantPage();
    setupOrganizationPage();
    setupDonationPage();

    connect(restaurantBtn, &QPushButton::clicked, this, &OrgModule::onRestaurantTab);
    connect(organizationBtn, &QPushButton::clicked, this, &OrgModule::onOrganizationTab);
    connect(donateBtn, &QPushButton::clicked, this, &OrgModule::onDonationTab);

    addLogoutButton();
}

void OrgModule::setupRestaurantPage()
{
    QWidget* page = new QWidget();
    QVBoxLayout* layout = new QVBoxLayout(page);
    QFormLayout* form = new QFormLayout();
    layout->setContentsMargins(18, 14, 18, 14);
    layout->setSpacing(10);
    form->setSpacing(8);
    form->setFieldGrowthPolicy(QFormLayout::ExpandingFieldsGrow);
    form->setLabelAlignment(Qt::AlignRight | Qt::AlignVCenter);

    d->resName = new QLineEdit();
    d->resCity = new QLineEdit();
    d->resCity->setPlaceholderText("e.g. Cairo");
    d->resStreet = new QLineEdit();
    d->resStreet->setPlaceholderText("e.g. Tahrir Street");
    d->resAddressDetails = new QLineEdit();
    d->resAddressDetails->setPlaceholderText("e.g. Building 5, Floor 3");
    d->resFoodType = new QComboBox();
    d->resContact = new QLineEdit();

    d->resFoodType->addItems({
        "Bakery", "Fast Food", "Groceries", "Home Meals",
        "Restaurant Meals", "Vegetarian", "Other"
    });

    for (QLineEdit* field : {
             d->resName,
             d->resCity,
             d->resStreet,
             d->resAddressDetails,
             d->resContact
         }) {
        field->setMinimumHeight(42);
    }
    d->resFoodType->setMinimumHeight(42);

    form->addRow("Restaurant Name:", d->resName);
    form->addRow("City:", d->resCity);
    form->addRow("Street:", d->resStreet);
    form->addRow("Building/Details:", d->resAddressDetails);
    form->addRow("Food Type:", d->resFoodType);
    form->addRow("Contact Info:", d->resContact);

    QPushButton* submitBtn = new QPushButton("Register Restaurant");
    submitBtn->setMinimumSize(240, 44);
    submitBtn->setCursor(Qt::PointingHandCursor);
    connect(submitBtn, &QPushButton::clicked, this, &OrgModule::submitRestaurant);

    layout->addLayout(form);
    layout->addWidget(submitBtn, 0, Qt::AlignCenter);
    d->stack->addWidget(page);
}
void OrgModule::setupOrganizationPage()
{
    QWidget* page = new QWidget();
    QVBoxLayout* layout = new QVBoxLayout(page);
    QFormLayout* form = new QFormLayout();
    layout->setContentsMargins(18, 14, 18, 14);
    layout->setSpacing(10);
    form->setSpacing(8);
    form->setFieldGrowthPolicy(QFormLayout::ExpandingFieldsGrow);
    form->setLabelAlignment(Qt::AlignRight | Qt::AlignVCenter);

    d->orgName = new QLineEdit();
    d->orgType = new QComboBox();
    d->orgCity = new QLineEdit();
    d->orgCity->setPlaceholderText("e.g. Cairo");
    d->orgStreet = new QLineEdit();
    d->orgStreet->setPlaceholderText("e.g. Tahrir Street");
    d->orgAddressDetails = new QLineEdit();
    d->orgAddressDetails->setPlaceholderText("e.g. Building 5, Floor 3");
    d->orgPurpose = new QTextEdit();
    d->orgContact = new QLineEdit();

    d->orgType->addItems({
        "Charity", "Community Kitchen", "Food Bank",
        "Mosque", "NGO", "Shelter", "Other"
    });

    for (QLineEdit* field : {
             d->orgName,
             d->orgCity,
             d->orgStreet,
             d->orgAddressDetails,
             d->orgContact
         }) {
        field->setMinimumHeight(42);
    }
    d->orgType->setMinimumHeight(42);
    d->orgPurpose->setMinimumHeight(72);

    form->addRow("Organization Name:", d->orgName);
    form->addRow("Organization Type:", d->orgType);
    form->addRow("City:", d->orgCity);
    form->addRow("Street:", d->orgStreet);
    form->addRow("Building/Details:", d->orgAddressDetails);
    form->addRow("Purpose:", d->orgPurpose);
    form->addRow("Contact Info:", d->orgContact);

    QPushButton* submitBtn = new QPushButton("Register Organization");
    submitBtn->setMinimumSize(250, 44);
    submitBtn->setCursor(Qt::PointingHandCursor);
    connect(submitBtn, &QPushButton::clicked, this, &OrgModule::submitOrganization);

    layout->addLayout(form);
    layout->addWidget(submitBtn, 0, Qt::AlignCenter);
    d->stack->addWidget(page);
}


void OrgModule::setupDonationPage()
{
    QWidget* page = new QWidget();
    QVBoxLayout* layout = new QVBoxLayout(page);
    QFormLayout* form = new QFormLayout();
    layout->setContentsMargins(18, 14, 18, 14);
    layout->setSpacing(10);
    form->setSpacing(8);
    form->setContentsMargins(0, 0, 0, 0);
    form->setFieldGrowthPolicy(QFormLayout::ExpandingFieldsGrow);
    form->setLabelAlignment(Qt::AlignRight | Qt::AlignVCenter);

    d->providerName = new QLineEdit();
    d->providerRole = new QComboBox();
    d->foodAmount = new QLineEdit();
    d->donationType = new QLineEdit();
    d->donationCity = new QLineEdit();
    d->donationCity->setPlaceholderText("e.g. Cairo");
    d->donationStreet = new QLineEdit();
    d->donationStreet->setPlaceholderText("e.g. Tahrir Street");
    d->donationAddressDetails = new QLineEdit();
    d->donationAddressDetails->setPlaceholderText("e.g. Building 5, Floor 3");
    d->deliveryMethod = new QComboBox();

    d->providerRole->addItems({"Restaurant", "Organization"});
    d->deliveryMethod->addItems({"Pickup", "Delivery", "Either"});

    for (QLineEdit* field : {
             d->providerName,
             d->foodAmount,
             d->donationType,
             d->donationCity,
             d->donationStreet,
             d->donationAddressDetails
         }) {
        field->setMinimumHeight(42);
    }
    d->providerRole->setMinimumHeight(42);
    d->deliveryMethod->setMinimumHeight(42);

    form->addRow("Provider Name:", d->providerName);
    form->addRow("Provider Type:", d->providerRole);
    form->addRow("Amount of Food:", d->foodAmount);
    form->addRow("Food Type:", d->donationType);
    form->addRow("City:", d->donationCity);
    form->addRow("Street:", d->donationStreet);
    form->addRow("Building/Details:", d->donationAddressDetails);
    form->addRow("Delivery Method:", d->deliveryMethod);

    QPushButton* submitBtn = new QPushButton("Submit Donation Details");
    submitBtn->setMinimumSize(260, 44);
    submitBtn->setCursor(Qt::PointingHandCursor);
    connect(submitBtn, &QPushButton::clicked, this, &OrgModule::submitDonation);

    layout->addLayout(form);
    layout->addWidget(submitBtn, 0, Qt::AlignCenter);
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
        d->resCity->text().trimmed().isEmpty() ||
        d->resStreet->text().trimmed().isEmpty() ||
        d->resContact->text().trimmed().isEmpty()) {
        QMessageBox::warning(this, "Missing Information", "Please fill in all fields.");
        return;
    }

    QString fullLocation = d->resCity->text().trimmed() + ", " +
                           d->resStreet->text().trimmed();

    QSqlQuery query;
    query.prepare("INSERT INTO restaurants (name, location, food_type, contact_info) "
                  "VALUES (:name, :location, :food_type, :contact_info)");
    query.bindValue(":name", d->resName->text().trimmed());
    query.bindValue(":location", fullLocation);
    query.bindValue(":food_type", d->resFoodType->currentText());
    query.bindValue(":contact_info", d->resContact->text().trimmed());

    if (query.exec()) {
        QMessageBox::information(this, "Success", "Restaurant registered successfully.");
        saveOrganizationState(d->resName->text().trimmed());
        d->resName->clear();
        d->resCity->clear();
        d->resStreet->clear();
        d->resAddressDetails->clear();
        d->resContact->clear();
    } else {
        QMessageBox::critical(this, "Database Error", query.lastError().text());
    }
}

void OrgModule::submitOrganization()
{
    if (d->orgName->text().trimmed().isEmpty() ||
        d->orgCity->text().trimmed().isEmpty() ||
        d->orgStreet->text().trimmed().isEmpty() ||
        d->orgPurpose->toPlainText().trimmed().isEmpty() ||
        d->orgContact->text().trimmed().isEmpty()) {
        QMessageBox::warning(this, "Missing Information", "Please fill in all fields.");
        return;
    }

    QString fullLocation = d->orgCity->text().trimmed() + ", " +
                           d->orgStreet->text().trimmed();

    QSqlQuery query;
    query.prepare("INSERT INTO organizations (name, organization_type, location, purpose, contact_info) "
                  "VALUES (:name, :organization_type, :location, :purpose, :contact_info)");
    query.bindValue(":name", d->orgName->text().trimmed());
    query.bindValue(":organization_type", d->orgType->currentText());
    query.bindValue(":location", fullLocation);
    query.bindValue(":purpose", d->orgPurpose->toPlainText().trimmed());
    query.bindValue(":contact_info", d->orgContact->text().trimmed());

    if (query.exec()) {
        QMessageBox::information(this, "Success", "Organization registered successfully.");
        saveOrganizationState(d->orgName->text().trimmed());
        d->orgName->clear();
        d->orgCity->clear();
        d->orgStreet->clear();
        d->orgAddressDetails->clear();
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
        d->donationCity->text().trimmed().isEmpty() ||
        d->donationStreet->text().trimmed().isEmpty()) {
        QMessageBox::warning(this, "Missing Information", "Please fill in all fields.");
        return;
    }

    QString fullLocation = d->donationCity->text().trimmed() + ", " +
                           d->donationStreet->text().trimmed();

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

    int foodAmountInt = d->foodAmount->text().trimmed().toInt();
    if (foodAmountInt <= 0) {
        QMessageBox::warning(this, "Invalid Amount", "Please enter a valid number of meals.");
        return;
    }
    
    QSqlQuery query;
    query.prepare("INSERT INTO food_donations "
                  "(provider_name, provider_role, food_amount, remaining_meals, "
                  "food_type, donation_location, delivery_method) "
                  "VALUES (:provider_name, :provider_role, :food_amount, :remaining_meals, "
                  ":food_type, :donation_location, :delivery_method)");
    query.bindValue(":provider_name", d->providerName->text().trimmed());
    query.bindValue(":provider_role", d->providerRole->currentText());
    query.bindValue(":food_amount", foodAmountInt);
    query.bindValue(":remaining_meals", d->foodAmount->text().trimmed().toInt());
    query.bindValue(":food_type", d->donationType->text().trimmed());
    query.bindValue(":donation_location", fullLocation);
    query.bindValue(":delivery_method", d->deliveryMethod->currentText());

    if (query.exec()) {
        QSqlQuery addrQuery;
        addrQuery.prepare("INSERT INTO all_addresses "
                          "(source_type, source_id, provider_name, address, "
                          "city, street, address_details, details) "
                          "VALUES "
                          "('donation', :source_id, :provider_name, :address, "
                          ":city, :street, :address_details, :details)");
        addrQuery.bindValue(":source_id", query.lastInsertId().toInt());
        addrQuery.bindValue(":provider_name", d->providerName->text().trimmed());
        addrQuery.bindValue(":address", fullLocation);
        addrQuery.bindValue(":city", d->donationCity->text().trimmed());
        addrQuery.bindValue(":street", d->donationStreet->text().trimmed());
        addrQuery.bindValue(":address_details", d->donationAddressDetails->text().trimmed());
        addrQuery.bindValue(":details", QString("%1 of %2 - Delivery: %3")
                                            .arg(d->foodAmount->text().trimmed())
                                            .arg(d->donationType->text().trimmed())
                                            .arg(d->deliveryMethod->currentText()));

        if (addrQuery.exec()) {
            DatabaseManager::matchAddresses();
        }

        QMessageBox::information(this, "Success", "Donation submitted successfully.");
        d->providerName->clear();
        d->foodAmount->clear();
        d->donationType->clear();
        d->donationCity->clear();
        d->donationStreet->clear();
        d->donationAddressDetails->clear();
    } else {
        QMessageBox::critical(this, "Database Error", query.lastError().text());
    }
}

void OrgModule::addLogoutButton()
{
    QVBoxLayout* mainLayout = qobject_cast<QVBoxLayout*>(this->layout());
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
    connect(logoutBtn, &QPushButton::clicked, this, &OrgModule::handleLogout);
}

void OrgModule::handleLogout()
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

void OrgModule::restoreFromData(const QString& data)
{
    // For organizations, you might want to restore which tab was last used
    // This is a placeholder - implement based on your needs
    Q_UNUSED(data);
}

void OrgModule::saveOrganizationState(const QString& orgName)
{
    QSettings settings;
    settings.setValue("lastModule", "organizations");
    settings.setValue("lastModuleData", orgName);
}
