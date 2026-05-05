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
#include <QFileDialog>
#include <QScrollArea>
#include <QDate>
#include <QDateEdit>
#include <QTableWidget>
#include <QHeaderView>
#include <QProgressBar>
#include <QTimer>
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
    QDateEdit *expiryDate = nullptr;
    QComboBox *deliveryMethod = nullptr;
// Safety check fields
QLineEdit *foodAge = nullptr;
QTextEdit *ingredients = nullptr;
QLineEdit *storageLocation = nullptr;
QLineEdit *storageTemperature = nullptr;
QLineEdit *allergenInfo = nullptr;
QTextEdit *safetyNotes = nullptr;
QLineEdit *photoPath = nullptr;

    // Updates page
    QLineEdit *updatesProviderName = nullptr;
    QTableWidget *updatesTable = nullptr;
    QLabel *statusSummary = nullptr;
    QLabel *statusDetail = nullptr;
    QProgressBar *statusProgress = nullptr;
    QTimer *statusTimer = nullptr;
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
    setMinimumSize(760, 520);
    setStyleSheet(
        "QDialog { background-color: #fffdf8; }"
        "QLabel { color: #20242a; font-size: 14px; font-weight: 800; }"
        "QLineEdit, QTextEdit, QComboBox, QDateEdit {"
        "    background-color: white;"
        "    color: #20242a;"
        "    border: 2px solid #e7eaee;"
        "    border-radius: 16px;"
        "    padding: 10px;"
        "}"
        "QLineEdit:focus, QTextEdit:focus, QComboBox:focus, QDateEdit:focus {"
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

    QWidget* statusBar = new QWidget();
    statusBar->setStyleSheet(
        "QWidget { background-color: #ffffff; border: 1px solid #e7eaee; border-radius: 18px; }"
        "QLabel { background: transparent; border: none; }"
        "QProgressBar { background-color: #f2f4f7; border: none; border-radius: 7px; height: 14px; text-align: center; color: #20242a; font-weight: 800; }"
        "QProgressBar::chunk { background-color: #ff9d18; border-radius: 7px; }"
    );
    QVBoxLayout* statusLayout = new QVBoxLayout(statusBar);
    statusLayout->setContentsMargins(20, 14, 20, 14);
    statusLayout->setSpacing(7);
    d->statusSummary = new QLabel("Latest donation: No order yet");
    d->statusSummary->setStyleSheet("color: #20242a; font-size: 16px; font-weight: 900;");
    d->statusDetail = new QLabel("Submit a donation to start tracking.");
    d->statusDetail->setWordWrap(true);
    d->statusDetail->setStyleSheet("color: #68707a; font-size: 13px; font-weight: 800;");
    d->statusProgress = new QProgressBar();
    d->statusProgress->setRange(0, 100);
    d->statusProgress->setValue(0);
    statusLayout->addWidget(d->statusSummary);
    statusLayout->addWidget(d->statusProgress);
    statusLayout->addWidget(d->statusDetail);
    mainLayout->addWidget(statusBar);

    // Tab buttons
    QHBoxLayout* switchLayout = new QHBoxLayout();
    QPushButton* restaurantBtn = new QPushButton("Restaurant");
    QPushButton* organizationBtn = new QPushButton("Organization");
    QPushButton* donateBtn = new QPushButton("Donation Details");
    QPushButton* updatesBtn = new QPushButton("Updates");
    restaurantBtn->setMinimumWidth(110);
    restaurantBtn->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    organizationBtn->setMinimumWidth(110);
    organizationBtn->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    donateBtn->setMinimumWidth(120);
    donateBtn->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    updatesBtn->setMinimumWidth(100);
    updatesBtn->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    updatesBtn->setStyleSheet(
        "QPushButton { background-color: #20242a; color: white; border: none;"
        "              border-radius: 22px; padding: 11px 18px; font-weight: 900; }"
        "QPushButton:hover { background-color: #3a4250; color: #ffffff; }"
    );

    switchLayout->addWidget(restaurantBtn);
    switchLayout->addWidget(organizationBtn);
    switchLayout->addWidget(donateBtn);
    switchLayout->addWidget(updatesBtn);
    mainLayout->addLayout(switchLayout);

    // Make the stacked pages scrollable so small screens can access all fields
    d->stack = new QStackedWidget(this);
    QScrollArea* stackScroll = new QScrollArea(this);
    stackScroll->setWidgetResizable(true);
    stackScroll->setFrameShape(QFrame::NoFrame);
    stackScroll->setWidget(d->stack);
    mainLayout->addWidget(stackScroll);

    setupRestaurantPage();
    setupOrganizationPage();
    setupDonationPage();
    setupUpdatesPage();

    connect(restaurantBtn, &QPushButton::clicked, this, &OrgModule::onRestaurantTab);
    connect(organizationBtn, &QPushButton::clicked, this, &OrgModule::onOrganizationTab);
    connect(donateBtn, &QPushButton::clicked, this, &OrgModule::onDonationTab);
    connect(updatesBtn, &QPushButton::clicked, this, &OrgModule::onUpdatesTab);

    addLogoutButton();
    updateOrderStatusBar();
    d->statusTimer = new QTimer(this);
    connect(d->statusTimer, &QTimer::timeout, this, &OrgModule::updateOrderStatusBar);
    d->statusTimer->start(5000);
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

    QPushButton* submitBtn = new QPushButton("Submit Restaurant Registration");
    submitBtn->setMinimumSize(300, 54);
    submitBtn->setCursor(Qt::PointingHandCursor);
    submitBtn->setStyleSheet("font-size: 16px; background-color: #20242a; color: white; border-radius: 27px;");
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

    QPushButton* submitBtn = new QPushButton("Submit Organization Registration");
    submitBtn->setMinimumSize(320, 54);
    submitBtn->setCursor(Qt::PointingHandCursor);
    submitBtn->setStyleSheet("font-size: 16px; background-color: #20242a; color: white; border-radius: 27px;");
    connect(submitBtn, &QPushButton::clicked, this, &OrgModule::submitOrganization);

    layout->addLayout(form);
    layout->addWidget(submitBtn, 0, Qt::AlignCenter);
    d->stack->addWidget(page);
}


void OrgModule::setupDonationPage()
{
    QScrollArea* scrollArea = new QScrollArea();
scrollArea->setWidgetResizable(true);
scrollArea->setStyleSheet("QScrollArea { border: none; background: transparent; }");

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
    d->expiryDate = new QDateEdit(QDate::currentDate().addDays(1));
    d->expiryDate->setCalendarPopup(true);
    d->expiryDate->setDisplayFormat("yyyy-MM-dd");
    d->expiryDate->setMinimumDate(QDate::currentDate());
    d->deliveryMethod = new QComboBox();
d->foodAge = new QLineEdit();
d->ingredients = new QTextEdit();
d->ingredients->setFixedHeight(80);
d->storageLocation = new QLineEdit();
d->storageTemperature = new QLineEdit();
d->allergenInfo = new QLineEdit();
d->safetyNotes = new QTextEdit();
d->safetyNotes->setFixedHeight(80);
d->photoPath = new QLineEdit();
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
    d->expiryDate->setMinimumHeight(42);
    d->deliveryMethod->setMinimumHeight(42);

    form->addRow("Provider Name:", d->providerName);
    form->addRow("Provider Type:", d->providerRole);
    form->addRow("Amount of Food:", d->foodAmount);
    form->addRow("Food Type:", d->donationType);
    form->addRow("City:", d->donationCity);
    form->addRow("Street:", d->donationStreet);
    form->addRow("Building/Details:", d->donationAddressDetails);
    form->addRow("Expiry Date:", d->expiryDate);
    form->addRow("Delivery Method:", d->deliveryMethod);
form->addRow("How long ago was food made?", d->foodAge);
form->addRow("Ingredients:", d->ingredients);
form->addRow("Where was it stored?", d->storageLocation);
form->addRow("Storage temperature:", d->storageTemperature);
form->addRow("Allergen info:", d->allergenInfo);
form->addRow("Safety notes:", d->safetyNotes);
QPushButton *photoButton = new QPushButton("Upload Photo");
QHBoxLayout *photoLayout = new QHBoxLayout();
photoLayout->addWidget(d->photoPath);
photoLayout->addWidget(photoButton);
form->addRow("Photo path:", photoLayout);

connect(photoButton, &QPushButton::clicked, this, [this]() {
    QString fileName = QFileDialog::getOpenFileName(
        this,
        "Select Food Photo",
        "",
        "Images (*.png *.jpg *.jpeg *.bmp)"
    );
    if (!fileName.isEmpty()) {
        d->photoPath->setText(fileName);
    }
});
    QPushButton* submitBtn = new QPushButton("Submit Donation & Safety Check");
    submitBtn->setMinimumSize(330, 54);
    submitBtn->setCursor(Qt::PointingHandCursor);
    submitBtn->setStyleSheet("font-size: 16px; background-color: #20242a; color: white; border-radius: 27px;");
    connect(submitBtn, &QPushButton::clicked, this, &OrgModule::submitDonation);

    layout->addLayout(form);
    layout->addWidget(submitBtn, 0, Qt::AlignCenter);
    scrollArea->setWidget(page);
d->stack->addWidget(scrollArea);
}

void OrgModule::updateOrderStatusBar()
{
    if (!d->statusSummary || !d->statusDetail || !d->statusProgress) return;

    QSqlQuery query;
    query.exec(
        "SELECT fd.provider_name, fd.food_amount, fd.remaining_meals, fd.food_type, "
        "       COALESCE(aa.match_status, 'unmatched'), "
        "       COALESCE(aa.delivery_status, 'pending') "
        "FROM food_donations fd "
        "LEFT JOIN all_addresses aa ON aa.source_type = 'donation' AND aa.source_id = fd.id "
        "ORDER BY fd.donation_date DESC LIMIT 1"
    );

    if (!query.next()) {
        d->statusSummary->setText("Latest donation: No order yet");
        d->statusDetail->setText("Submit a donation to start tracking.");
        d->statusProgress->setValue(0);
        return;
    }

    const QString providerName = query.value(0).toString();
    const int totalMeals = query.value(1).toInt();
    const int remainingMeals = query.value(2).toInt();
    const QString foodType = query.value(3).toString();
    const QString matchStatus = query.value(4).toString();
    const QString deliveryStatus = query.value(5).toString();

    int progress = 25;
    QString statusText = "Submitted";
    QString detail = "Waiting for a matching meal request.";

    if (matchStatus == "matched" || remainingMeals < totalMeals) {
        progress = 65;
        statusText = "Matched";
        detail = "Some meals have been matched with people in need.";
    }
    if (deliveryStatus == "in_transit" || deliveryStatus == "assigned" || deliveryStatus == "in_progress") {
        progress = 85;
        statusText = "In transit";
        detail = "Pickup or delivery is active for this donation.";
    }
    if (deliveryStatus == "delivered" || (totalMeals > 0 && remainingMeals <= 0)) {
        progress = 100;
        statusText = "Completed";
        detail = "All meals from this donation have been allocated or delivered.";
    }

    d->statusSummary->setText(
        QString("Latest donation by %1: %2")
            .arg(providerName.isEmpty() ? QString("provider") : providerName)
            .arg(statusText));
    d->statusDetail->setText(
        QString("%1 of %2 %3 remaining. %4")
            .arg(remainingMeals < 0 ? 0 : remainingMeals)
            .arg(totalMeals)
            .arg(foodType.isEmpty() ? "meals" : foodType)
            .arg(detail));
    d->statusProgress->setValue(progress);
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

void OrgModule::onUpdatesTab()
{
    d->stack->setCurrentIndex(3);
}

void OrgModule::setupUpdatesPage()
{
    QWidget* page = new QWidget();
    QVBoxLayout* layout = new QVBoxLayout(page);
    layout->setContentsMargins(18, 14, 18, 14);
    layout->setSpacing(12);

    QLabel* title = new QLabel("Donation & Delivery Updates");
    title->setFont(QFont("Arial", 18, QFont::Bold));
    title->setAlignment(Qt::AlignCenter);
    title->setStyleSheet("font-size: 18px; font-weight: 900; color: #20242a;");
    layout->addWidget(title);

    QLabel* subtitle = new QLabel("Track your donations — see how many meals were matched and delivery progress.");
    subtitle->setWordWrap(true);
    subtitle->setAlignment(Qt::AlignCenter);
    subtitle->setStyleSheet("color: #68707a; font-size: 13px; font-weight: 600;");
    layout->addWidget(subtitle);

    QHBoxLayout* searchLayout = new QHBoxLayout();
    QLabel* nameLabel = new QLabel("Provider Name:");
    d->updatesProviderName = new QLineEdit();
    d->updatesProviderName->setPlaceholderText("Enter your registered provider name");
    d->updatesProviderName->setMinimumHeight(42);
    QPushButton* refreshBtn = new QPushButton("Check Updates");
    refreshBtn->setMinimumHeight(42);
    searchLayout->addWidget(nameLabel);
    searchLayout->addWidget(d->updatesProviderName, 1);
    searchLayout->addWidget(refreshBtn);
    layout->addLayout(searchLayout);

    d->updatesTable = new QTableWidget(0, 6);
    d->updatesTable->setHorizontalHeaderLabels({"Donation Date", "Food Type", "Total Meals", "Remaining", "Match Status", "Delivery Status"});
    d->updatesTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    d->updatesTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    d->updatesTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    d->updatesTable->setAlternatingRowColors(true);
    d->updatesTable->verticalHeader()->setVisible(false);
    d->updatesTable->setStyleSheet(
        "QTableWidget { border: 1px solid #e7eaee; border-radius: 14px; background: white; }"
        "QHeaderView::section { background-color: #20242a; color: white; font-weight: 800;"
        "                       padding: 8px; border: none; }"
    );
    layout->addWidget(d->updatesTable);

    connect(refreshBtn, &QPushButton::clicked, this, &OrgModule::refreshOrgUpdates);

    d->stack->addWidget(page);
}

void OrgModule::refreshOrgUpdates()
{
    QString providerName = d->updatesProviderName->text().trimmed();
    if (providerName.isEmpty()) {
        QMessageBox::information(this, "Enter Name", "Please enter your provider name.");
        return;
    }

    d->updatesTable->setRowCount(0);

    QSqlQuery query;
    query.prepare(
        "SELECT fd.donation_date, fd.food_type, fd.food_amount, fd.remaining_meals, "
        "       COALESCE(aa.match_status, 'unmatched'), COALESCE(aa.delivery_status, 'pending') "
        "FROM food_donations fd "
        "LEFT JOIN all_addresses aa ON aa.source_type = 'donation' AND aa.source_id = fd.id "
        "WHERE fd.provider_name = :name "
        "ORDER BY fd.donation_date DESC"
    );
    query.bindValue(":name", providerName);

    if (!query.exec()) {
        QMessageBox::critical(this, "Error", query.lastError().text());
        return;
    }

    while (query.next()) {
        int row = d->updatesTable->rowCount();
        d->updatesTable->insertRow(row);
        d->updatesTable->setItem(row, 0, new QTableWidgetItem(query.value(0).toString()));
        d->updatesTable->setItem(row, 1, new QTableWidgetItem(query.value(1).toString()));
        d->updatesTable->setItem(row, 2, new QTableWidgetItem(query.value(2).toString() + " meals"));
        d->updatesTable->setItem(row, 3, new QTableWidgetItem(query.value(3).toString() + " meals"));

        QString matchStatus = query.value(4).toString();
        QTableWidgetItem* matchItem = new QTableWidgetItem(matchStatus == "matched" ? "Matched" : "Pending");
        if (matchStatus == "matched") {
            matchItem->setBackground(QColor("#d4edda"));
            matchItem->setForeground(QColor("#155724"));
        } else {
            matchItem->setBackground(QColor("#fff3cd"));
            matchItem->setForeground(QColor("#856404"));
        }
        d->updatesTable->setItem(row, 4, matchItem);

        QString deliveryStatus = query.value(5).toString();
        QString deliveryText;
        if (deliveryStatus == "delivered") deliveryText = "Delivered";
        else if (deliveryStatus == "in_transit") deliveryText = "In Transit";
        else deliveryText = "Pending";
        QTableWidgetItem* deliveryItem = new QTableWidgetItem(deliveryText);
        if (deliveryStatus == "delivered") {
            deliveryItem->setBackground(QColor("#d4edda"));
            deliveryItem->setForeground(QColor("#155724"));
        } else if (deliveryStatus == "in_transit") {
            deliveryItem->setBackground(QColor("#fff3cd"));
            deliveryItem->setForeground(QColor("#856404"));
        }
        d->updatesTable->setItem(row, 5, deliveryItem);
    }

    if (d->updatesTable->rowCount() == 0) {
        QMessageBox::information(this, "No Donations",
            QString("No donations found for '%1'. Make sure the name matches exactly.").arg(providerName));
    }
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
    if (d->expiryDate->date() < QDate::currentDate()) {
        QMessageBox::warning(this, "Expired Food", "Please choose today or a future expiry date.");
        return;
    }
    QSqlQuery safetyQuery;
safetyQuery.prepare("INSERT INTO provider_safety_checks "
    "(provider_name, provider_role, food_age, ingredients, expiry_date, storage_location, "
    "storage_temperature, allergen_info, safety_notes, photo_path) "
    "VALUES (:provider_name, :provider_role, :food_age, :ingredients, :expiry_date, "
    ":storage_location, :storage_temperature, :allergen_info, :safety_notes, :photo_path)");
safetyQuery.bindValue(":provider_name", d->providerName->text().trimmed());
safetyQuery.bindValue(":provider_role", d->providerRole->currentText());
safetyQuery.bindValue(":food_age", d->foodAge->text().trimmed());
safetyQuery.bindValue(":ingredients", d->ingredients->toPlainText().trimmed());
safetyQuery.bindValue(":expiry_date", d->expiryDate->date().toString("yyyy-MM-dd"));
safetyQuery.bindValue(":storage_location", d->storageLocation->text().trimmed());
safetyQuery.bindValue(":storage_temperature", d->storageTemperature->text().trimmed());
safetyQuery.bindValue(":allergen_info", d->allergenInfo->text().trimmed());
safetyQuery.bindValue(":safety_notes", d->safetyNotes->toPlainText().trimmed());
safetyQuery.bindValue(":photo_path", d->photoPath->text().trimmed());
safetyQuery.exec();
    QSqlQuery query;
    query.prepare("INSERT INTO food_donations "
                  "(provider_name, provider_role, food_amount, remaining_meals, "
                  "food_type, donation_location, delivery_method, expiry_date) "
                  "VALUES (:provider_name, :provider_role, :food_amount, :remaining_meals, "
                  ":food_type, :donation_location, :delivery_method, :expiry_date)");
    query.bindValue(":provider_name", d->providerName->text().trimmed());
    query.bindValue(":provider_role", d->providerRole->currentText());
    query.bindValue(":food_amount", foodAmountInt);
    query.bindValue(":remaining_meals", d->foodAmount->text().trimmed().toInt());
    query.bindValue(":food_type", d->donationType->text().trimmed());
    query.bindValue(":donation_location", fullLocation);
    query.bindValue(":delivery_method", d->deliveryMethod->currentText());
    query.bindValue(":expiry_date", d->expiryDate->date().toString("yyyy-MM-dd"));

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
        updateOrderStatusBar();
        d->providerName->clear();
        d->foodAmount->clear();
        d->donationType->clear();
        d->donationCity->clear();
        d->donationStreet->clear();
        d->donationAddressDetails->clear();
d->expiryDate->setDate(QDate::currentDate().addDays(1));
d->foodAge->clear();
d->ingredients->clear();
d->storageLocation->clear();
d->storageTemperature->clear();
d->allergenInfo->clear();
d->safetyNotes->clear();
d->photoPath->clear();
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
