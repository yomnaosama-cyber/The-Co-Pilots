#include "mainwindow.h"
#include "delivery.h"
#include "people.h"
#include "org.h"
#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QFont>
#include <QApplication>
#include <QSettings>
#include <QFrame>
#include <QGraphicsOpacityEffect>
#include <QPropertyAnimation>
#include <QEasingCurve>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      deliveryModule(nullptr),
      peopleModule(nullptr),
      orgModule(nullptr)
{
    setupUI();
}

MainWindow::~MainWindow()
{
    // Modules are deleted automatically as children or with their parents.
}

void MainWindow::setupUI()
{
    setWindowTitle("Co-Pilots Food Sharing");
    setMinimumSize(1180, 820);
    setStyleSheet(
        "QMainWindow { background-color: #ef3038; }"
        "QToolTip { background-color: #20242a; color: white; border: none; padding: 7px; border-radius: 6px; }"
    );

    QWidget* central = new QWidget();
    setCentralWidget(central);

    QVBoxLayout* shellLayout = new QVBoxLayout(central);
    shellLayout->setContentsMargins(48, 38, 48, 38);
    shellLayout->setSpacing(0);

    QWidget* appCanvas = new QWidget();
    appCanvas->setObjectName("appCanvas");
    appCanvas->setStyleSheet(
        "QWidget#appCanvas {"
        "   background-color: #fffdf8;"
        "   border-radius: 48px;"
        "}"
    );
    shellLayout->addWidget(appCanvas);

    QVBoxLayout* mainLayout = new QVBoxLayout(appCanvas);
    mainLayout->setSpacing(22);
    mainLayout->setContentsMargins(42, 30, 42, 34);

    QLabel* logo = new QLabel("Co-Pilots Food Sharing");
    logo->setStyleSheet("color: #20242a; font-size: 24px; font-weight: 900; background: transparent;");
    mainLayout->addWidget(logo);

    QWidget* heroWidget = new QWidget();
    heroWidget->setStyleSheet("background: transparent;");
    QHBoxLayout* heroLayout = new QHBoxLayout(heroWidget);
    heroLayout->setContentsMargins(0, 8, 0, 2);
    heroLayout->setSpacing(34);

    QVBoxLayout* heroTextLayout = new QVBoxLayout();
    heroTextLayout->setSpacing(16);

    QLabel* heroKicker = new QLabel("Share surplus food with people who need it");
    heroKicker->setStyleSheet("color: #ef3038; font-size: 17px; font-weight: 900; background: transparent;");

    QLabel* heroTitle = new QLabel("Donate food.\nRequest meals.\nDeliver safely.");
    heroTitle->setFont(QFont("Arial", 42, QFont::Bold));
    heroTitle->setStyleSheet("color: #20242a; background: transparent;");

    QLabel* heroText = new QLabel("Restaurants and organizations can offer extra meals, people can request help, and delivery volunteers can accept matched orders.");
    heroText->setWordWrap(true);
    heroText->setMinimumHeight(58);
    heroText->setMaximumWidth(480);
    heroText->setStyleSheet("color: #5f6670; font-size: 17px; font-weight: 600; background: transparent;");

    QPushButton* startButton = new QPushButton("Choose Your Role");
    startButton->setFixedSize(190, 54);
    startButton->setCursor(Qt::PointingHandCursor);
    startButton->setStyleSheet(
        "QPushButton {"
        "   background-color: #ef3038;"
        "   color: #ffffff;"
        "   border: none;"
        "   border-radius: 27px;"
        "   font-size: 16px;"
        "   font-weight: 900;"
        "}"
        "QPushButton:hover { background-color: #d92731; }"
        "QPushButton:pressed { background-color: #bd2028; }"
    );

    heroTextLayout->addWidget(heroKicker);
    heroTextLayout->addWidget(heroTitle);
    heroTextLayout->addWidget(heroText);
    heroTextLayout->addSpacing(8);
    heroTextLayout->addWidget(startButton);
    heroTextLayout->addStretch();

    QFrame* riderScene = new QFrame();
    riderScene->setFixedSize(480, 315);
    riderScene->setStyleSheet("QFrame { background-color: #fff3df; border: none; border-radius: 36px; }");

    QLabel* phone = new QLabel(riderScene);
    phone->setGeometry(255, 36, 112, 218);
    phone->setStyleSheet("background-color: #20242a; border-radius: 22px;");
    QLabel* phoneScreen = new QLabel(phone);
    phoneScreen->setGeometry(12, 30, 88, 150);
    phoneScreen->setStyleSheet("background-color: #fffdf8; border-radius: 8px;");
    QLabel* phoneButton = new QLabel(phone);
    phoneButton->setGeometry(45, 190, 24, 24);
    phoneButton->setStyleSheet("background-color: #414852; border-radius: 12px;");

    QLabel* hand = new QLabel(riderScene);
    hand->setGeometry(344, 86, 92, 202);
    hand->setStyleSheet("background-color: #f3b18f; border-radius: 42px;");

    QLabel* foodBubble = new QLabel(riderScene);
    foodBubble->setGeometry(324, 88, 118, 84);
    foodBubble->setStyleSheet("background-color: #ef3038; border-radius: 18px;");

    QLabel* burgerTop = new QLabel(foodBubble);
    burgerTop->setGeometry(25, 18, 68, 28);
    burgerTop->setStyleSheet("background-color: #ffb21f; border-radius: 18px;");
    QLabel* burgerLine = new QLabel(foodBubble);
    burgerLine->setGeometry(22, 50, 74, 10);
    burgerLine->setStyleSheet("background-color: #5b2b20; border-radius: 5px;");
    QLabel* burgerBase = new QLabel(foodBubble);
    burgerBase->setGeometry(29, 61, 60, 12);
    burgerBase->setStyleSheet("background-color: #ff9d18; border-radius: 5px;");

    QLabel* mealPhotoOne = new QLabel(riderScene);
    mealPhotoOne->setGeometry(30, 34, 102, 78);
    mealPhotoOne->setStyleSheet(
        "background-color: qlineargradient(x1:0, y1:0, x2:1, y2:1, stop:0 #fff8e9, stop:0.45 #ffd08a, stop:1 #d85a34);"
        "border: 6px solid #ffffff;"
        "border-radius: 20px;"
    );
    QLabel* plateOne = new QLabel(mealPhotoOne);
    plateOne->setGeometry(24, 16, 54, 44);
    plateOne->setStyleSheet("background-color: #fffdf8; border-radius: 22px;");
    QLabel* foodOne = new QLabel(plateOne);
    foodOne->setGeometry(13, 10, 28, 22);
    foodOne->setStyleSheet("background-color: #20a675; border-radius: 11px;");

    QLabel* mealPhotoTwo = new QLabel(riderScene);
    mealPhotoTwo->setGeometry(140, 32, 92, 72);
    mealPhotoTwo->setStyleSheet(
        "background-color: qlineargradient(x1:0, y1:0, x2:1, y2:1, stop:0 #f7fbff, stop:0.5 #ffbd59, stop:1 #8b4a2d);"
        "border: 6px solid #ffffff;"
        "border-radius: 18px;"
    );
    QLabel* plateTwo = new QLabel(mealPhotoTwo);
    plateTwo->setGeometry(23, 15, 46, 38);
    plateTwo->setStyleSheet("background-color: #fffdf8; border-radius: 19px;");
    QLabel* foodTwo = new QLabel(plateTwo);
    foodTwo->setGeometry(11, 9, 24, 19);
    foodTwo->setStyleSheet("background-color: #ef3038; border-radius: 10px;");

    QLabel* pin = new QLabel(riderScene);
    pin->setGeometry(430, 145, 46, 76);
    pin->setStyleSheet("background-color: #ef3038; border-radius: 23px;");
    QLabel* pinDot = new QLabel(pin);
    pinDot->setGeometry(12, 12, 22, 22);
    pinDot->setStyleSheet("background-color: #fffdf8; border-radius: 11px;");

    QLabel* backWheel = new QLabel(riderScene);
    backWheel->setGeometry(80, 222, 58, 58);
    backWheel->setStyleSheet("background-color: #fffdf8; border: 10px solid #414852; border-radius: 29px;");
    QLabel* frontWheel = new QLabel(riderScene);
    frontWheel->setGeometry(218, 222, 58, 58);
    frontWheel->setStyleSheet("background-color: #fffdf8; border: 10px solid #414852; border-radius: 29px;");
    QLabel* scooterBody = new QLabel(riderScene);
    scooterBody->setGeometry(72, 198, 210, 45);
    scooterBody->setStyleSheet("background-color: #ef3038; border-radius: 22px;");
    QLabel* scooterSeat = new QLabel(riderScene);
    scooterSeat->setGeometry(150, 181, 68, 18);
    scooterSeat->setStyleSheet("background-color: #20242a; border-radius: 9px;");
    QLabel* deliveryBox = new QLabel("HOT", riderScene);
    deliveryBox->setGeometry(70, 134, 72, 64);
    deliveryBox->setAlignment(Qt::AlignCenter);
    deliveryBox->setStyleSheet("background-color: #ff5a4f; color: white; border-radius: 12px; font-size: 18px; font-weight: 900;");
    QLabel* riderBody = new QLabel(riderScene);
    riderBody->setGeometry(172, 113, 48, 76);
    riderBody->setStyleSheet("background-color: #ef3038; border-radius: 22px;");
    QLabel* riderHead = new QLabel(riderScene);
    riderHead->setGeometry(178, 76, 42, 42);
    riderHead->setStyleSheet("background-color: #f3b18f; border-radius: 21px;");
    QLabel* riderCap = new QLabel(riderScene);
    riderCap->setGeometry(168, 70, 62, 20);
    riderCap->setStyleSheet("background-color: #20242a; border-radius: 10px;");
    QLabel* road = new QLabel(riderScene);
    road->setGeometry(52, 280, 380, 8);
    road->setStyleSheet("background-color: #d8dde2; border-radius: 4px;");

    QGraphicsOpacityEffect* riderOpacity = new QGraphicsOpacityEffect(riderScene);
    riderOpacity->setOpacity(0.96);
    riderScene->setGraphicsEffect(riderOpacity);
    QPropertyAnimation* riderPulse = new QPropertyAnimation(riderOpacity, "opacity", riderScene);
    riderPulse->setDuration(1700);
    riderPulse->setStartValue(0.90);
    riderPulse->setEndValue(1.0);
    riderPulse->setEasingCurve(QEasingCurve::InOutSine);
    riderPulse->setLoopCount(-1);
    riderPulse->start();

    heroLayout->addLayout(heroTextLayout, 1);
    heroLayout->addWidget(riderScene, 0, Qt::AlignRight | Qt::AlignVCenter);
    mainLayout->addWidget(heroWidget);

    QWidget* statsBand = new QWidget();
    statsBand->setStyleSheet("QWidget { background-color: #f7f8fa; border-radius: 22px; }");
    QHBoxLayout* statsLayout = new QHBoxLayout(statsBand);
    statsLayout->setContentsMargins(22, 14, 22, 14);
    statsLayout->setSpacing(22);

    auto createStat = [](const QString& number, const QString& label) {
        QWidget* stat = new QWidget();
        stat->setStyleSheet("background: transparent;");
        QVBoxLayout* statLayout = new QVBoxLayout(stat);
        statLayout->setContentsMargins(0, 0, 0, 0);
        statLayout->setSpacing(2);
        QLabel* value = new QLabel(number);
        value->setAlignment(Qt::AlignCenter);
        value->setStyleSheet("color: #ef3038; font-size: 24px; font-weight: 900; background: transparent;");
        QLabel* caption = new QLabel(label);
        caption->setWordWrap(true);
        caption->setMinimumHeight(34);
        caption->setAlignment(Qt::AlignCenter);
        caption->setStyleSheet("color: #68707a; font-size: 13px; font-weight: 800; background: transparent;");
        statLayout->addWidget(value);
        statLayout->addWidget(caption);
        return stat;
    };
    statsLayout->addWidget(createStat("Donors", "restaurants and organizations"));
    statsLayout->addWidget(createStat("People", "meal requests"));
    statsLayout->addWidget(createStat("Drivers", "pickup and delivery"));
    mainLayout->addWidget(statsBand);

    QHBoxLayout* btnLayout = new QHBoxLayout();
    btnLayout->setAlignment(Qt::AlignCenter);
    btnLayout->setSpacing(18);

    QString buttonStyle =
        "QPushButton {"
        "   background-color: #ffffff;"
        "   border: 1px solid #e7eaee;"
        "   border-radius: 22px;"
        "   padding: 0px;"
        "   text-align: left;"
        "   color: #20242a;"
        "}"
        "QPushButton:hover { background-color: #fff9f0; border-color: #ef3038; }"
        "QPushButton:pressed { background-color: #ffe9d9; }";

    auto createMainCardButton = [&](const QString& badge, const QString& title, const QString& description, const QString& accent) {
        QPushButton* cardBtn = new QPushButton();
        cardBtn->setMinimumSize(300, 205);
        cardBtn->setCursor(Qt::PointingHandCursor);
        cardBtn->setStyleSheet(buttonStyle);

        QVBoxLayout* cardLayout = new QVBoxLayout(cardBtn);
        cardLayout->setContentsMargins(22, 20, 22, 18);
        cardLayout->setSpacing(8);
        cardLayout->setAlignment(Qt::AlignTop);

        QLabel* accentBar = new QLabel(cardBtn);
        accentBar->setFixedHeight(5);
        accentBar->setStyleSheet("background-color: " + accent + "; border-radius: 3px;");

        QLabel* badgeLabel = new QLabel(badge, cardBtn);
        badgeLabel->setFixedSize(48, 48);
        badgeLabel->setAlignment(Qt::AlignCenter);
        badgeLabel->setStyleSheet("background-color: " + accent + "; color: white; border-radius: 24px; font-size: 16px; font-weight: 900;");

        QLabel* titleLabel = new QLabel(title, cardBtn);
        titleLabel->setWordWrap(true);
        titleLabel->setMinimumHeight(52);
        titleLabel->setAlignment(Qt::AlignLeft);
        titleLabel->setStyleSheet("color: #20242a; font-size: 22px; font-weight: 900; background: transparent;");

        QLabel* descLabel = new QLabel(description, cardBtn);
        descLabel->setWordWrap(true);
        descLabel->setMinimumHeight(48);
        descLabel->setAlignment(Qt::AlignLeft);
        descLabel->setStyleSheet("color: #68707a; font-size: 14px; font-weight: 700; background: transparent;");

        cardLayout->addWidget(accentBar);
        cardLayout->addSpacing(4);
        cardLayout->addWidget(badgeLabel, 0, Qt::AlignLeft);
        cardLayout->addWidget(titleLabel);
        cardLayout->addWidget(descLabel);
        cardLayout->addStretch();

        return cardBtn;
    };

    QPushButton* deliveryBtn = createMainCardButton(
        "01",
        "Delivery System",
        "Accept matched food requests, pick up donations, and deliver meals.",
        "#ef3038"
    );
    connect(deliveryBtn, &QPushButton::clicked, this, &MainWindow::onDeliveryClicked);

    QPushButton* organizationsBtn = createMainCardButton(
        "02",
        "Organizations & Restaurants",
        "Register a restaurant or organization, then submit donation details.",
        "#ff9d18"
    );
    connect(organizationsBtn, &QPushButton::clicked, this, &MainWindow::onOrganizationsClicked);

    QPushButton* peopleBtn = createMainCardButton(
        "03",
        "People in Need",
        "Sign up and request the number of meals you need.",
        "#20a675"
    );
    connect(peopleBtn, &QPushButton::clicked, this, &MainWindow::onPeopleClicked);

    btnLayout->addWidget(deliveryBtn);
    btnLayout->addWidget(organizationsBtn);
    btnLayout->addWidget(peopleBtn);
    mainLayout->addLayout(btnLayout);

    QLabel* footer = new QLabel("Select your role to continue.");
    footer->setFont(QFont("Arial", 14, QFont::Bold));
    footer->setAlignment(Qt::AlignCenter);
    footer->setStyleSheet("color: #68707a; padding-top: 4px; background: transparent;");
    mainLayout->addWidget(footer);

    connect(startButton, &QPushButton::clicked, this, [deliveryBtn]() {
        deliveryBtn->setFocus();
    });

    deliveryModule = new DeliveryModule(this);
    peopleModule = new PeopleModule(this);
    orgModule = new OrgModule(this);
}

void MainWindow::showModuleDirectly(const QString& module, const QString& moduleData)
{
    if (module == "delivery") {
        if (deliveryModule) {
            deliveryModule->show();
            deliveryModule->raise();
            deliveryModule->activateWindow();
        }
    } else if (module == "organizations") {
        if (orgModule) {
            if (!moduleData.isEmpty()) {
                orgModule->restoreFromData(moduleData);
            }
            orgModule->exec();
        }
    } else if (module == "people") {
        if (peopleModule) {
            peopleModule->show();
            peopleModule->raise();
            peopleModule->activateWindow();
        }
    }
    this->hide();
}

void MainWindow::clearSavedModule()
{
    QSettings settings;
    settings.remove("lastModule");
    settings.remove("lastModuleData");
}

void MainWindow::onDeliveryClicked()
{
    if (deliveryModule) {
        deliveryModule->show();
        deliveryModule->raise();
        deliveryModule->activateWindow();
    }
}

void MainWindow::onOrganizationsClicked()
{
    if (orgModule) {
        orgModule->exec();
    }
}

void MainWindow::onPeopleClicked()
{
    if (peopleModule) {
        peopleModule->show();
        peopleModule->raise();
        peopleModule->activateWindow();
    }
}
