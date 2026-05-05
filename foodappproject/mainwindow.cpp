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
#include <QScrollArea>
#include <QResizeEvent>
#include <QGridLayout>
#include <QScrollBar>
#include <QColor>
#include <QPoint>
#include <QAbstractAnimation>

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
    setWindowTitle("Sella");
    setMinimumSize(420, 600);
    setStyleSheet(
        "QMainWindow { background-color: #e53935; }"
        "QToolTip { background-color: #20242a; color: white; border: none; padding: 7px; border-radius: 6px; }"
    );

    QWidget* central = new QWidget();
    setCentralWidget(central);
    QVBoxLayout* centralLayout = new QVBoxLayout(central);
    centralLayout->setContentsMargins(28, 24, 28, 24);
    centralLayout->setSpacing(0);

    QWidget* appCanvas = new QWidget();
    appCanvas->setObjectName("appCanvas");
    appCanvas->setStyleSheet(
        "QWidget#appCanvas {"
        "   background-color: #fffdf8;"
        "   border-radius: 32px;"
        "}"
    );
    centralLayout->addWidget(appCanvas);

    QVBoxLayout* appLayout = new QVBoxLayout(appCanvas);
    appLayout->setContentsMargins(36, 24, 36, 0);
    appLayout->setSpacing(0);

    QWidget* navBar = new QWidget();
    navBar->setObjectName("navBar");
    navBar->setStyleSheet(
        "QWidget#navBar { background-color: #fffdf8; border-bottom: 1px solid #eef0f2; }"
    );
    QHBoxLayout* navLayout = new QHBoxLayout(navBar);
    navLayout->setContentsMargins(0, 0, 0, 16);
    navLayout->setSpacing(14);

    QLabel* logo = new QLabel("Sella");
    logo->setStyleSheet("color: #18202a; font-family: Inter, Poppins, Arial; font-size: 28px; font-weight: 900; background: transparent;");

    QString navLinkStyle =
        "QPushButton {"
        "   background: transparent;"
        "   border: 2px solid transparent;"
        "   border-radius: 18px;"
        "   color: #4d5560;"
        "   font-family: Inter, Poppins, Arial;"
        "   font-size: 15px;"
        "   font-weight: 800;"
        "   padding: 8px 12px;"
        "}"
        "QPushButton:hover { color: #e53935; background-color: #fff5f5; }"
        "QPushButton:focus { border-color: #e53935; }";

    QPushButton* howItWorksNav = new QPushButton("How it Works");
    QPushButton* aboutNav = new QPushButton("About");
    QPushButton* contactNav = new QPushButton("Contact");
    howItWorksNav->setCursor(Qt::PointingHandCursor);
    aboutNav->setCursor(Qt::PointingHandCursor);
    contactNav->setCursor(Qt::PointingHandCursor);
    howItWorksNav->setStyleSheet(navLinkStyle);
    aboutNav->setStyleSheet(navLinkStyle);
    contactNav->setStyleSheet(navLinkStyle);
    this->m_howItWorksNav = howItWorksNav;
    this->m_aboutNav = aboutNav;
    this->m_contactNav = contactNav;

    QPushButton* loginButton = new QPushButton("Login");
    loginButton->setCursor(Qt::PointingHandCursor);
    loginButton->setFixedHeight(42);
    loginButton->setMinimumWidth(96);
    loginButton->setStyleSheet(
        "QPushButton {"
        "   background-color: #ffffff;"
        "   color: #e53935;"
        "   border: 2px solid #e53935;"
        "   border-radius: 21px;"
        "   font-family: Inter, Poppins, Arial;"
        "   font-size: 15px;"
        "   font-weight: 900;"
        "}"
        "QPushButton:hover { background-color: #fff5f5; }"
        "QPushButton:focus { border-color: #20242a; }"
    );

    navLayout->addWidget(logo);
    navLayout->addStretch();
    navLayout->addWidget(howItWorksNav);
    navLayout->addWidget(aboutNav);
    navLayout->addWidget(contactNav);
    navLayout->addWidget(loginButton);
    appLayout->addWidget(navBar);

    QScrollArea* scroll = new QScrollArea(appCanvas);
    scroll->setFrameShape(QFrame::NoFrame);
    scroll->setWidgetResizable(true);
    scroll->setStyleSheet("QScrollArea { background: transparent; border: none; } QScrollBar:vertical { background: #f2f4f6; width: 10px; } QScrollBar::handle:vertical { background: #e53935; border-radius: 5px; min-height: 44px; }");
    QWidget* scrollContent = new QWidget();
    scrollContent->setStyleSheet("background: transparent;");
    scroll->setWidget(scrollContent);
    appLayout->addWidget(scroll);

    QVBoxLayout* mainLayout = new QVBoxLayout(scrollContent);
    mainLayout->setSpacing(24);
    mainLayout->setContentsMargins(0, 28, 0, 28);

    QWidget* heroWidget = new QWidget();
    heroWidget->setStyleSheet("background: transparent;");
    QHBoxLayout* heroLayout = new QHBoxLayout(heroWidget);
    heroLayout->setContentsMargins(0, 2, 0, 0);
    heroLayout->setSpacing(26);

    QVBoxLayout* heroTextLayout = new QVBoxLayout();
    heroTextLayout->setSpacing(18);

    QLabel* heroKicker = new QLabel("Share surplus food with people who need it");
    heroKicker->setStyleSheet("color: #e53935; font-family: Inter, Poppins, Arial; font-size: 17px; font-weight: 900; background: transparent;");

    QLabel* heroTitle = new QLabel("Donate food.\nRequest meals.\nDeliver safely.");
    heroTitle->setWordWrap(true);
    heroTitle->setFont(QFont("Inter", 42, QFont::Bold));
    heroTitle->setStyleSheet("color: #18202a; font-family: Inter, Poppins, Arial; background: transparent;");

    QLabel* heroText = new QLabel("Restaurants and organizations can offer extra meals, people can request help, and delivery volunteers can accept matched orders.");
    heroText->setWordWrap(true);
    heroText->setMinimumHeight(72);
    heroText->setMaximumWidth(580);
    heroText->setStyleSheet("color: #3f4852; font-family: Inter, Poppins, Arial; font-size: 18px; font-weight: 600; line-height: 1.6; background: transparent;");

    QPushButton* startButton = new QPushButton("Choose Your Role");
    startButton->setFixedHeight(56);
    startButton->setMinimumWidth(220);
    startButton->setMaximumWidth(248);
    startButton->setCursor(Qt::PointingHandCursor);
    startButton->setStyleSheet(
        "QPushButton {"
        "   background-color: #e53935;"
        "   color: #ffffff;"
        "   border: 2px solid #e53935;"
        "   border-radius: 28px;"
        "   font-family: Inter, Poppins, Arial;"
        "   font-size: 16px;"
        "   font-weight: 900;"
        "   padding: 0 22px;"
        "}"
        "QPushButton:hover { background-color: #c62828; border-color: #c62828; }"
        "QPushButton:pressed { background-color: #a91f1f; border-color: #a91f1f; }"
        "QPushButton:focus { border-color: #20242a; }"
    );

    heroTextLayout->addWidget(heroKicker);
    heroTextLayout->addWidget(heroTitle);
    heroTextLayout->addWidget(heroText);
    heroTextLayout->addSpacing(8);
    heroTextLayout->addWidget(startButton);
    heroTextLayout->addStretch();

    QFrame* riderScene = new QFrame();
    riderScene->setFixedSize(380, 252);
    riderScene->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    riderScene->setStyleSheet("QFrame { background-color: #fff2df; border: none; border-radius: 30px; }");

    QLabel* phone = new QLabel(riderScene);
    phone->setGeometry(248, 34, 104, 202);
    phone->setStyleSheet("background-color: #20242a; border-radius: 22px;");
    QLabel* phoneScreen = new QLabel(phone);
    phoneScreen->setGeometry(12, 30, 88, 150);
    phoneScreen->setStyleSheet("background-color: #fffdf8; border-radius: 8px;");
    QLabel* phoneButton = new QLabel(phone);
    phoneButton->setGeometry(45, 190, 24, 24);
    phoneButton->setStyleSheet("background-color: #414852; border-radius: 12px;");

    QLabel* hand = new QLabel(riderScene);
    hand->setGeometry(333, 86, 72, 162);
    hand->setStyleSheet("background-color: #f3b18f; border-radius: 42px;");

    QLabel* foodBubble = new QLabel(riderScene);
    foodBubble->setGeometry(314, 88, 96, 72);
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
    pin->setGeometry(346, 144, 42, 70);
    pin->setStyleSheet("background-color: #ef3038; border-radius: 23px;");
    QLabel* pinDot = new QLabel(pin);
    pinDot->setGeometry(12, 12, 22, 22);
    pinDot->setStyleSheet("background-color: #fffdf8; border-radius: 11px;");

    QLabel* backWheel = new QLabel(riderScene);
    backWheel->setGeometry(72, 206, 52, 52);
    backWheel->setStyleSheet("background-color: #fffdf8; border: 10px solid #414852; border-radius: 29px;");
    QLabel* frontWheel = new QLabel(riderScene);
    frontWheel->setGeometry(214, 206, 52, 52);
    frontWheel->setStyleSheet("background-color: #fffdf8; border: 10px solid #414852; border-radius: 29px;");
    QLabel* scooterBody = new QLabel(riderScene);
    scooterBody->setGeometry(64, 184, 208, 42);
    scooterBody->setStyleSheet("background-color: #ef3038; border-radius: 22px;");
    QLabel* scooterSeat = new QLabel(riderScene);
    scooterSeat->setGeometry(150, 169, 68, 18);
    scooterSeat->setStyleSheet("background-color: #20242a; border-radius: 9px;");
    QLabel* deliveryBox = new QLabel("HOT", riderScene);
    deliveryBox->setGeometry(68, 124, 68, 58);
    deliveryBox->setAlignment(Qt::AlignCenter);
    deliveryBox->setStyleSheet("background-color: #ff5a4f; color: white; border-radius: 12px; font-size: 18px; font-weight: 900;");
    QLabel* riderBody = new QLabel(riderScene);
    riderBody->setGeometry(172, 105, 48, 72);
    riderBody->setStyleSheet("background-color: #ef3038; border-radius: 22px;");
    QLabel* riderHead = new QLabel(riderScene);
    riderHead->setGeometry(178, 70, 42, 42);
    riderHead->setStyleSheet("background-color: #f3b18f; border-radius: 21px;");
    QLabel* riderCap = new QLabel(riderScene);
    riderCap->setGeometry(168, 64, 62, 20);
    riderCap->setStyleSheet("background-color: #20242a; border-radius: 10px;");
    QLabel* road = new QLabel(riderScene);
    road->setGeometry(48, 240, 304, 8);
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
    // keep pointers for responsive adjustments
    this->m_heroLayout = heroLayout;
    this->m_riderScene = riderScene;
    this->m_heroTitle = heroTitle;
    mainLayout->addWidget(heroWidget);

    QWidget* statsBand = new QWidget();
    statsBand->setStyleSheet("QWidget { background-color: #f4f6f8; border-radius: 18px; }");
    QHBoxLayout* statsLayout = new QHBoxLayout(statsBand);
    statsLayout->setContentsMargins(24, 16, 24, 16);
    statsLayout->setSpacing(24);

    auto createStat = [](const QString& number, const QString& label) {
        QWidget* stat = new QWidget();
        stat->setStyleSheet("background: transparent;");
        QVBoxLayout* statLayout = new QVBoxLayout(stat);
        statLayout->setContentsMargins(0, 0, 0, 0);
        statLayout->setSpacing(2);
        QLabel* value = new QLabel(number);
        value->setAlignment(Qt::AlignCenter);
        value->setStyleSheet("color: #c81935; font-family: Inter, Poppins, Arial; font-size: 24px; font-weight: 900; background: transparent;");
        QLabel* caption = new QLabel(label);
        caption->setWordWrap(true);
        caption->setMinimumHeight(34);
        caption->setAlignment(Qt::AlignCenter);
        caption->setStyleSheet("color: #5d6670; font-family: Inter, Poppins, Arial; font-size: 15px; font-weight: 800; background: transparent;");
        statLayout->addWidget(value);
        statLayout->addWidget(caption);
        return stat;
    };
    statsLayout->addWidget(createStat("Donors", "restaurants and organizations"));
    statsLayout->addWidget(createStat("People", "meal requests"));
    statsLayout->addWidget(createStat("Drivers", "pickup and delivery"));
    mainLayout->addWidget(statsBand);

    QGridLayout* btnLayout = new QGridLayout();
    btnLayout->setAlignment(Qt::AlignCenter);
    btnLayout->setHorizontalSpacing(16);
    btnLayout->setVerticalSpacing(14);

    QString buttonStyle =
        "QPushButton {"
        "   background-color: #ffffff;"
        "   border: 2px solid #e2e7ec;"
        "   border-radius: 16px;"
        "   padding: 0px;"
        "   text-align: left;"
        "   color: #18202a;"
        "}"
        "QPushButton:hover { background-color: #fff7f7; border-color: #e53935; }"
        "QPushButton:pressed { background-color: #ffe9e9; }"
        "QPushButton:focus { border-color: #20242a; }";

    auto createMainCardButton = [&](const QString& icon, const QString& badge, const QString& title, const QString& description, const QString& accent) {
        QPushButton* cardBtn = new QPushButton();
        cardBtn->setMinimumSize(278, 212);
        cardBtn->setMaximumWidth(360);
        cardBtn->setCursor(Qt::PointingHandCursor);
        cardBtn->setStyleSheet(buttonStyle);

        QGraphicsDropShadowEffect* cardShadow = new QGraphicsDropShadowEffect(cardBtn);
        cardShadow->setBlurRadius(24);
        cardShadow->setOffset(0, 8);
        cardShadow->setColor(QColor(24, 32, 42, 24));
        cardBtn->setGraphicsEffect(cardShadow);

        QVBoxLayout* cardLayout = new QVBoxLayout(cardBtn);
        cardLayout->setContentsMargins(24, 22, 24, 22);
        cardLayout->setSpacing(10);
        cardLayout->setAlignment(Qt::AlignTop);

        QLabel* accentBar = new QLabel(cardBtn);
        accentBar->setFixedHeight(5);
        accentBar->setStyleSheet("background-color: " + accent + "; border-radius: 3px;");

        QHBoxLayout* cardTopLayout = new QHBoxLayout();
        cardTopLayout->setContentsMargins(0, 0, 0, 0);
        cardTopLayout->setSpacing(10);

        QLabel* iconLabel = new QLabel(icon, cardBtn);
        iconLabel->setFixedSize(52, 52);
        iconLabel->setAlignment(Qt::AlignCenter);
        iconLabel->setStyleSheet("background-color: " + accent + "; color: white; border-radius: 26px; font-family: Inter, Poppins, Arial; font-size: 15px; font-weight: 900;");

        QLabel* badgeLabel = new QLabel(badge, cardBtn);
        badgeLabel->setFixedSize(44, 44);
        badgeLabel->setAlignment(Qt::AlignCenter);
        badgeLabel->setStyleSheet("background-color: #f7f9fb; color: " + accent + "; border: 1px solid #e2e7ec; border-radius: 22px; font-family: Inter, Poppins, Arial; font-size: 15px; font-weight: 900;");
        cardTopLayout->addWidget(iconLabel, 0, Qt::AlignLeft);
        cardTopLayout->addStretch();
        cardTopLayout->addWidget(badgeLabel, 0, Qt::AlignRight);

        QLabel* titleLabel = new QLabel(title, cardBtn);
        titleLabel->setWordWrap(true);
        titleLabel->setMinimumHeight(56);
        titleLabel->setAlignment(Qt::AlignLeft);
        titleLabel->setStyleSheet("color: #18202a; font-family: Inter, Poppins, Arial; font-size: 23px; font-weight: 900; background: transparent;");

        QLabel* descLabel = new QLabel(description, cardBtn);
        descLabel->setWordWrap(true);
        descLabel->setMinimumHeight(58);
        descLabel->setAlignment(Qt::AlignLeft);
        descLabel->setStyleSheet("color: #4d5560; font-family: Inter, Poppins, Arial; font-size: 16px; font-weight: 650; line-height: 1.6; background: transparent;");

        cardLayout->addWidget(accentBar);
        cardLayout->addSpacing(6);
        cardLayout->addLayout(cardTopLayout);
        cardLayout->addWidget(titleLabel);
        cardLayout->addWidget(descLabel);
        cardLayout->addStretch();

        return cardBtn;
    };

    QPushButton* organizationsBtn = createMainCardButton(
        "DNR",
        "01",
        "Donors",
        "Restaurants and organizations can share surplus meals with clear donation details.",
        "#e53935"
    );
    connect(organizationsBtn, &QPushButton::clicked, this, &MainWindow::onOrganizationsClicked);

    QPushButton* peopleBtn = createMainCardButton(
        "PPL",
        "02",
        "People",
        "Request the number of meals you need and get matched with available food support.",
        "#20a675"
    );
    connect(peopleBtn, &QPushButton::clicked, this, &MainWindow::onPeopleClicked);

    QPushButton* deliveryBtn = createMainCardButton(
        "DRV",
        "03",
        "Drivers",
        "Accept matched food requests, pick up donations, and deliver meals safely.",
        "#ff9d18"
    );
    connect(deliveryBtn, &QPushButton::clicked, this, &MainWindow::onDeliveryClicked);

    btnLayout->addWidget(organizationsBtn, 0, 0);
    btnLayout->addWidget(peopleBtn, 0, 1);
    btnLayout->addWidget(deliveryBtn, 0, 2);
    btnLayout->setColumnStretch(0, 1);
    btnLayout->setColumnStretch(1, 1);
    btnLayout->setColumnStretch(2, 1);
    this->m_roleGrid = btnLayout;
    this->m_deliveryButton = deliveryBtn;
    this->m_organizationsButton = organizationsBtn;
    this->m_peopleButton = peopleBtn;
    mainLayout->addLayout(btnLayout);

    QWidget* footer = new QWidget();
    footer->setObjectName("footer");
    footer->setStyleSheet("QWidget#footer { background-color: #f4f6f8; border-radius: 18px; }");
    QHBoxLayout* footerLayout = new QHBoxLayout(footer);
    footerLayout->setContentsMargins(24, 18, 24, 18);
    footerLayout->setSpacing(16);
    QLabel* footerBrand = new QLabel("Sella");
    footerBrand->setStyleSheet("color: #18202a; font-family: Inter, Poppins, Arial; font-size: 18px; font-weight: 900; background: transparent;");
    QLabel* footerLinks = new QLabel("How it Works   About   Contact");
    footerLinks->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    footerLinks->setStyleSheet("color: #5d6670; font-family: Inter, Poppins, Arial; font-size: 15px; font-weight: 800; background: transparent;");
    footerLayout->addWidget(footerBrand);
    footerLayout->addStretch();
    footerLayout->addWidget(footerLinks);
    mainLayout->addWidget(footer);

    auto smoothScrollTo = [scroll, scrollContent](QWidget* target) {
        QPropertyAnimation* scrollAnimation = new QPropertyAnimation(scroll->verticalScrollBar(), "value", scroll);
        int targetValue = target->mapTo(scrollContent, QPoint(0, 0)).y() - 16;
        scrollAnimation->setDuration(420);
        scrollAnimation->setStartValue(scroll->verticalScrollBar()->value());
        scrollAnimation->setEndValue(qMax(0, targetValue));
        scrollAnimation->setEasingCurve(QEasingCurve::InOutCubic);
        scrollAnimation->start(QAbstractAnimation::DeleteWhenStopped);
    };

    connect(startButton, &QPushButton::clicked, this, [organizationsBtn, smoothScrollTo]() {
        smoothScrollTo(organizationsBtn);
        organizationsBtn->setFocus();
    });
    connect(howItWorksNav, &QPushButton::clicked, this, [organizationsBtn, smoothScrollTo]() { smoothScrollTo(organizationsBtn); });
    connect(aboutNav, &QPushButton::clicked, this, [statsBand, smoothScrollTo]() { smoothScrollTo(statsBand); });
    connect(contactNav, &QPushButton::clicked, this, [footer, smoothScrollTo]() { smoothScrollTo(footer); });

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

void MainWindow::resizeEvent(QResizeEvent* event)
{
    QMainWindow::resizeEvent(event);

    if (!m_heroLayout || !m_heroTitle || !m_riderScene)
        return;

    int w = event->size().width();

    if (w < 900) {
        m_heroLayout->setDirection(QBoxLayout::TopToBottom);
        m_riderScene->setVisible(w >= 560);
        QFont f = m_heroTitle->font();
        f.setPointSize(30);
        m_heroTitle->setFont(f);
    } else {
        m_heroLayout->setDirection(QBoxLayout::LeftToRight);
        m_riderScene->setVisible(true);
        QFont f = m_heroTitle->font();
        f.setPointSize(42);
        m_heroTitle->setFont(f);
    }

    if (m_howItWorksNav && m_aboutNav && m_contactNav) {
        bool showNavLinks = w >= 760;
        m_howItWorksNav->setVisible(showNavLinks);
        m_aboutNav->setVisible(showNavLinks);
        m_contactNav->setVisible(showNavLinks);
    }

    if (m_roleGrid && m_deliveryButton && m_organizationsButton && m_peopleButton) {
        if (w < 900) {
            m_roleGrid->addWidget(m_organizationsButton, 0, 0);
            m_roleGrid->addWidget(m_peopleButton, 1, 0);
            m_roleGrid->addWidget(m_deliveryButton, 2, 0);
            m_roleGrid->setColumnStretch(0, 1);
            m_roleGrid->setColumnStretch(1, 0);
            m_roleGrid->setColumnStretch(2, 0);
        } else {
            m_roleGrid->addWidget(m_organizationsButton, 0, 0);
            m_roleGrid->addWidget(m_peopleButton, 0, 1);
            m_roleGrid->addWidget(m_deliveryButton, 0, 2);
            m_roleGrid->setColumnStretch(0, 1);
            m_roleGrid->setColumnStretch(1, 1);
            m_roleGrid->setColumnStretch(2, 1);
        }
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
