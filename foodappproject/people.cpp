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
#include <QTableWidget>
#include <QHeaderView>
#include <QProgressBar>
#include <QTimer>

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

    QDialog *updatesDialog = nullptr;
    QLineEdit *updatesIdLine = nullptr;
    QTableWidget *updatesTable = nullptr;
    QLabel *statusSummary = nullptr;
    QLabel *statusDetail = nullptr;
    QProgressBar *statusProgress = nullptr;
    QTimer *statusTimer = nullptr;
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

    QPushButton* updatesBtn = new QPushButton("View Updates");
    updatesBtn->setMinimumWidth(320);
    updatesBtn->setStyleSheet(buttonStyle);
    updatesBtn->setCursor(Qt::PointingHandCursor);

    QWidget* statusBar = new QWidget();
    statusBar->setMinimumWidth(560);
    statusBar->setStyleSheet(
        "QWidget { background-color: #ffffff; border: 1px solid #e7eaee; border-radius: 18px; }"
        "QLabel { background: transparent; border: none; }"
        "QProgressBar { background-color: #f2f4f7; border: none; border-radius: 7px; height: 14px; text-align: center; color: #20242a; font-weight: 800; }"
        "QProgressBar::chunk { background-color: #20a675; border-radius: 7px; }"
    );
    QVBoxLayout* statusLayout = new QVBoxLayout(statusBar);
    statusLayout->setContentsMargins(20, 14, 20, 14);
    statusLayout->setSpacing(7);
    d->statusSummary = new QLabel("Latest order: No request yet");
    d->statusSummary->setStyleSheet("color: #20242a; font-size: 16px; font-weight: 900;");
    d->statusDetail = new QLabel("Submit a meal request to start tracking.");
    d->statusDetail->setWordWrap(true);
    d->statusDetail->setStyleSheet("color: #68707a; font-size: 13px; font-weight: 800;");
    d->statusProgress = new QProgressBar();
    d->statusProgress->setRange(0, 100);
    d->statusProgress->setValue(0);
    statusLayout->addWidget(d->statusSummary);
    statusLayout->addWidget(d->statusProgress);
    statusLayout->addWidget(d->statusDetail);

    layout->addStretch();
    layout->addWidget(header);
    layout->addWidget(foodPictures, 0, Qt::AlignCenter);
    layout->addWidget(signupBtn);
    layout->addWidget(requestBtn);
    layout->addWidget(updatesBtn);
    layout->addWidget(statusBar);
    layout->addStretch();

    setupSignUpDialog();
    setupMealRequestDialog();
    setupUpdatesDialog();

    connect(signupBtn, &QPushButton::clicked, this, &PeopleModule::handleSignUp);
    connect(requestBtn, &QPushButton::clicked, this, &PeopleModule::handleMealRequest);
    connect(updatesBtn, &QPushButton::clicked, this, &PeopleModule::handleUpdates);

    addLogoutButton();
    updateOrderStatusBar();
    d->statusTimer = new QTimer(this);
    connect(d->statusTimer, &QTimer::timeout, this, &PeopleModule::updateOrderStatusBar);
    d->statusTimer->start(5000);
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
        updateOrderStatusBar();
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

void PeopleModule::setupUpdatesDialog()
{
    d->updatesDialog = new QDialog(this);
    d->updatesDialog->setWindowTitle("Your Request Updates");
    d->updatesDialog->setMinimumSize(760, 500);
    d->updatesDialog->setStyleSheet(
        "QDialog { background-color: #fffdf8; }"
        "QLabel { color: #20242a; font-size: 14px; font-weight: 800; }"
        "QLineEdit { background: white; border: 2px solid #e7eaee; border-radius: 18px;"
        "            padding: 10px 14px; color: #20242a; font-size: 15px; }"
        "QLineEdit:focus { border-color: #ef3038; }"
        "QPushButton { background-color: #ef3038; color: #20242a; border: none;"
        "              border-radius: 20px; padding: 10px 22px; font-size: 15px; font-weight: 900; }"
        "QPushButton:hover { background-color: #d92731; color: white; }"
        "QTableWidget { border: 1px solid #e7eaee; border-radius: 14px; background: white; }"
        "QHeaderView::section { background-color: #20242a; color: white; font-weight: 800;"
        "                       padding: 8px; border: none; }"
    );

    QVBoxLayout* layout = new QVBoxLayout(d->updatesDialog);
    layout->setSpacing(14);
    layout->setContentsMargins(24, 22, 24, 22);

    QLabel* title = new QLabel("Your Meal Request Status");
    title->setFont(QFont("Arial", 20, QFont::Bold));
    title->setAlignment(Qt::AlignCenter);
    layout->addWidget(title);

    QHBoxLayout* searchLayout = new QHBoxLayout();
    QLabel* idLabel = new QLabel("Your ID:");
    d->updatesIdLine = new QLineEdit();
    d->updatesIdLine->setPlaceholderText("Enter your registered ID");
    d->updatesIdLine->setMinimumHeight(44);
    QPushButton* refreshBtn = new QPushButton("Check Status");
    refreshBtn->setMinimumHeight(44);
    searchLayout->addWidget(idLabel);
    searchLayout->addWidget(d->updatesIdLine, 1);
    searchLayout->addWidget(refreshBtn);
    layout->addLayout(searchLayout);

    d->updatesTable = new QTableWidget(0, 5);
    d->updatesTable->setHorizontalHeaderLabels({"Date", "Details", "Address", "Match Status", "Delivery Status"});
    d->updatesTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    d->updatesTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    d->updatesTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    d->updatesTable->setAlternatingRowColors(true);
    d->updatesTable->verticalHeader()->setVisible(false);
    layout->addWidget(d->updatesTable);

    connect(refreshBtn, &QPushButton::clicked, this, &PeopleModule::refreshUpdates);
}

void PeopleModule::updateOrderStatusBar()
{
    if (!d->statusSummary || !d->statusDetail || !d->statusProgress) return;

    QSqlQuery query;
    query.exec(
        "SELECT ps.name, mr.meal_count, mr.delivered_meals, "
        "       COALESCE(aa.match_status, 'unmatched'), "
        "       COALESCE(aa.delivery_status, 'pending'), mr.request_date "
        "FROM meal_requests mr "
        "JOIN people_sign ps ON ps.id = mr.person_id "
        "LEFT JOIN all_addresses aa ON aa.source_type = 'meal_request' AND aa.source_id = mr.person_id "
        "ORDER BY mr.request_date DESC LIMIT 1"
    );

    if (!query.next()) {
        d->statusSummary->setText("Latest order: No request yet");
        d->statusDetail->setText("Submit a meal request to start tracking.");
        d->statusProgress->setValue(0);
        return;
    }

    const QString personName = query.value(0).toString();
    const int mealCount = query.value(1).toInt();
    const int deliveredMeals = query.value(2).toInt();
    const QString matchStatus = query.value(3).toString();
    const QString deliveryStatus = query.value(4).toString();

    int progress = 25;
    QString statusText = "Submitted";
    QString detail = "Waiting for a nearby provider match.";

    if (matchStatus == "matched") {
        progress = 65;
        statusText = "Matched";
        detail = "A provider has available meals for this request.";
    }
    if (deliveryStatus == "in_transit" || deliveryStatus == "assigned" || deliveryStatus == "in_progress") {
        progress = 85;
        statusText = "In transit";
        detail = "A delivery volunteer is handling the order.";
    }
    if (deliveryStatus == "delivered" || (mealCount > 0 && deliveredMeals >= mealCount)) {
        progress = 100;
        statusText = "Delivered";
        detail = "The requested meals have been completed.";
    }

    d->statusSummary->setText(
        QString("Latest order for %1: %2")
            .arg(personName.isEmpty() ? QString("person in need") : personName)
            .arg(statusText));
    d->statusDetail->setText(
        QString("%1 %2 requested, %3 delivered. %4")
            .arg(mealCount)
            .arg(mealCount == 1 ? "meal" : "meals")
            .arg(deliveredMeals)
            .arg(detail));
    d->statusProgress->setValue(progress);
}

void PeopleModule::handleUpdates()
{
    d->updatesDialog->exec();
}

void PeopleModule::refreshUpdates()
{
    QString personIdText = d->updatesIdLine->text().trimmed();
    if (personIdText.isEmpty()) {
        QMessageBox::information(d->updatesDialog, "Enter ID", "Please enter your registered ID.");
        return;
    }

    d->updatesTable->setRowCount(0);

    QSqlQuery idQuery;
    idQuery.prepare("SELECT id FROM people_sign WHERE people_id = :pid ORDER BY id DESC LIMIT 1");
    idQuery.bindValue(":pid", personIdText);
    if (!idQuery.exec() || !idQuery.next()) {
        QMessageBox::warning(d->updatesDialog, "Not Found", "No account found with this ID.");
        return;
    }
    int autoId = idQuery.value(0).toInt();

    QSqlQuery query;
    query.prepare(
        "SELECT created_at, details, address, match_status, delivery_status "
        "FROM all_addresses "
        "WHERE source_type = 'meal_request' AND source_id = :pid "
        "ORDER BY created_at DESC"
    );
    query.bindValue(":pid", autoId);

    if (!query.exec()) {
        QMessageBox::critical(d->updatesDialog, "Error", query.lastError().text());
        return;
    }

    while (query.next()) {
        int row = d->updatesTable->rowCount();
        d->updatesTable->insertRow(row);
        d->updatesTable->setItem(row, 0, new QTableWidgetItem(query.value(0).toString()));
        d->updatesTable->setItem(row, 1, new QTableWidgetItem(query.value(1).toString()));
        d->updatesTable->setItem(row, 2, new QTableWidgetItem(query.value(2).toString()));

        QString matchStatus = query.value(3).toString();
        QTableWidgetItem* matchItem = new QTableWidgetItem(matchStatus == "matched" ? "Matched" : "Pending");
        if (matchStatus == "matched") {
            matchItem->setBackground(QColor("#d4edda"));
            matchItem->setForeground(QColor("#155724"));
        } else {
            matchItem->setBackground(QColor("#fff3cd"));
            matchItem->setForeground(QColor("#856404"));
        }
        d->updatesTable->setItem(row, 3, matchItem);

        QString deliveryStatus = query.value(4).toString();
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
        d->updatesTable->setItem(row, 4, deliveryItem);
    }

    if (d->updatesTable->rowCount() == 0) {
        QMessageBox::information(d->updatesDialog, "No Requests",
            "No meal requests found for this ID.");
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
