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
    // Modules are deleted automatically as children or with their parents
}

void MainWindow::setupUI()
{
    setWindowTitle("Co-Pilots");
    setMinimumSize(1000, 800);
    setStyleSheet(
        "QMainWindow { background-color: #fff6f8; }"
        "QToolTip { background-color: #231f20; color: white; border: none; padding: 6px; }"
    );

    QWidget* central = new QWidget();
    setCentralWidget(central);

    QVBoxLayout* mainLayout = new QVBoxLayout(central);
    mainLayout->setSpacing(26);
    mainLayout->setContentsMargins(28, 24, 28, 24);

    // Header Widget
    QWidget* headerWidget = new QWidget();
    headerWidget->setStyleSheet(
        "QWidget {"
        "   background-color: #f28fa0;"
        "   border-radius: 28px;"
        "}"
    );
    headerWidget->setFixedHeight(120);

    QVBoxLayout* headerLayout = new QVBoxLayout(headerWidget);
    headerLayout->setAlignment(Qt::AlignCenter);

    QLabel* header = new QLabel("Connect excess food - Reduce waste - Fight hunger");
    header->setFont(QFont("Arial", 28, QFont::Bold));
    header->setAlignment(Qt::AlignCenter);
    header->setStyleSheet("color: #231f20; background-color: transparent; padding: 20px;");

    headerLayout->addWidget(header);
    mainLayout->addWidget(headerWidget);

    QWidget* heroWidget = new QWidget();
    heroWidget->setStyleSheet(
        "QWidget {"
        "   background-color: white;"
        "   border: 1px solid #f2d9de;"
        "   border-radius: 30px;"
        "}"
    );
    QHBoxLayout* heroLayout = new QHBoxLayout(heroWidget);
    heroLayout->setContentsMargins(30, 24, 30, 24);
    heroLayout->setSpacing(28);

    QVBoxLayout* heroTextLayout = new QVBoxLayout();
    heroTextLayout->setSpacing(8);
    QLabel* heroTitle = new QLabel("food delivery\nat doorstep");
    heroTitle->setFont(QFont("Arial", 36, QFont::Bold));
    heroTitle->setStyleSheet("color: #231f20; background: transparent; border: none;");
    QLabel* heroText = new QLabel("Soft pink style, friendlier controls, and a delivery rider animation without changing the app logic.");
    heroText->setWordWrap(true);
    heroText->setStyleSheet("color: #77676c; font-size: 17px; font-weight: 600; background: transparent; border: none;");
    heroTextLayout->addWidget(heroTitle);
    heroTextLayout->addWidget(heroText);
    heroTextLayout->addStretch();

    QFrame* riderScene = new QFrame();
    riderScene->setFixedSize(360, 230);
    riderScene->setStyleSheet(
        "QFrame {"
        "   background-color: #f28fa0;"
        "   border: none;"
        "   border-radius: 115px;"
        "}"
    );

    QLabel* backWheel = new QLabel(riderScene);
    backWheel->setGeometry(48, 145, 72, 72);
    backWheel->setStyleSheet("background-color: #ef5b70; border: 10px solid #231f20; border-radius: 36px;");

    QLabel* frontWheel = new QLabel(riderScene);
    frontWheel->setGeometry(226, 145, 72, 72);
    frontWheel->setStyleSheet("background-color: #ef5b70; border: 10px solid #231f20; border-radius: 36px;");

    QLabel* scooterBody = new QLabel(riderScene);
    scooterBody->setGeometry(62, 122, 215, 44);
    scooterBody->setStyleSheet("background-color: #ef5b70; border: 5px solid #231f20; border-radius: 22px;");

    QLabel* deliveryBox = new QLabel("food\ndelivery", riderScene);
    deliveryBox->setGeometry(48, 70, 88, 66);
    deliveryBox->setAlignment(Qt::AlignCenter);
    deliveryBox->setStyleSheet("background-color: #dc354f; color: white; border: 5px solid #231f20; border-radius: 8px; font-weight: 900; font-size: 14px;");

    QLabel* riderBody = new QLabel(riderScene);
    riderBody->setGeometry(176, 58, 52, 86);
    riderBody->setStyleSheet("background-color: #f4b7a8; border: 5px solid #231f20; border-radius: 24px;");

    QLabel* riderHead = new QLabel(riderScene);
    riderHead->setGeometry(190, 20, 48, 48);
    riderHead->setStyleSheet("background-color: #efad9d; border: 5px solid #231f20; border-radius: 24px;");

    QLabel* riderCap = new QLabel(riderScene);
    riderCap->setGeometry(178, 14, 72, 24);
    riderCap->setStyleSheet("background-color: #ef5365; border: 5px solid #231f20; border-radius: 12px;");

    QLabel* handle = new QLabel(riderScene);
    handle->setGeometry(264, 94, 52, 34);
    handle->setStyleSheet("background: transparent; border-top: 5px solid #231f20; border-right: 5px solid #231f20; border-radius: 12px;");

    QGraphicsOpacityEffect* riderOpacity = new QGraphicsOpacityEffect(riderScene);
    riderOpacity->setOpacity(0.92);
    riderScene->setGraphicsEffect(riderOpacity);
    QPropertyAnimation* riderPulse = new QPropertyAnimation(riderOpacity, "opacity", riderScene);
    riderPulse->setDuration(1500);
    riderPulse->setStartValue(0.86);
    riderPulse->setEndValue(1.0);
    riderPulse->setEasingCurve(QEasingCurve::InOutSine);
    riderPulse->setLoopCount(-1);
    riderPulse->start();

    heroLayout->addLayout(heroTextLayout, 1);
    heroLayout->addWidget(riderScene, 0, Qt::AlignRight | Qt::AlignVCenter);
    mainLayout->addWidget(heroWidget);

    // Main buttons layout
    mainLayout->addStretch();
    
    QHBoxLayout* btnLayout = new QHBoxLayout();
    btnLayout->setAlignment(Qt::AlignCenter);
    btnLayout->setSpacing(30);

    QString buttonStyle =
        "QPushButton {"
        "   background-color: #ffffff;"
        "   border: 2px solid #f2d9de;"
        "   border-radius: 26px;"
        "   padding: 0px;"
        "   text-align: left;"
        "   color: #231f20;"
        "}"
        "QPushButton:hover {"
        "   background-color: #fff0f3;"
        "   border-color: #f28fa0;"
        "   margin-top: -2px;"
        "}"
        "QPushButton:pressed {"
        "   background-color: #f9c5ce;"
        "}";

    auto createMainCardButton = [&](const QString& icon, const QString& title, const QString& description) {
        QPushButton* cardBtn = new QPushButton();
        cardBtn->setFixedSize(270, 250);
        cardBtn->setCursor(Qt::PointingHandCursor);
        cardBtn->setStyleSheet(buttonStyle);

        QVBoxLayout* cardLayout = new QVBoxLayout(cardBtn);
        cardLayout->setContentsMargins(20, 20, 20, 20);
        cardLayout->setSpacing(8);
        cardLayout->setAlignment(Qt::AlignCenter);

        QLabel* iconLabel = new QLabel(icon, cardBtn);
        iconLabel->setFixedSize(72, 72);
        iconLabel->setAlignment(Qt::AlignCenter);
        iconLabel->setStyleSheet(
            "background-color: #fff0f3;"
            "border: 2px solid #f2d9de;"
            "border-radius: 36px;"
            "font-size: 34px;"
        );

        QLabel* titleLabel = new QLabel(title, cardBtn);
        titleLabel->setWordWrap(true);
        titleLabel->setAlignment(Qt::AlignTop | Qt::AlignHCenter);
        titleLabel->setStyleSheet("color: #231f20; font-size: 26px; font-weight: 900; background: transparent;");

        QLabel* descLabel = new QLabel(description, cardBtn);
        descLabel->setWordWrap(true);
        descLabel->setAlignment(Qt::AlignCenter | Qt::AlignHCenter);
        descLabel->setStyleSheet("color: #77676c; font-size: 18px; font-weight: 700; background: transparent;");

        cardLayout->addWidget(iconLabel, 0, Qt::AlignHCenter);
        cardLayout->addWidget(titleLabel);
        cardLayout->addWidget(descLabel);
        cardLayout->addStretch();

        return cardBtn;
    };

    // Delivery Button
    QPushButton* deliveryBtn = createMainCardButton(
        "🛵",
        "\nDelivery System",
        "\n\nClick here if you can offer delivery services"
    );
    connect(deliveryBtn, &QPushButton::clicked, this, &MainWindow::onDeliveryClicked);

    // Organizations Button
    QPushButton* organizationsBtn = createMainCardButton(
        "🍱",
        "\nOrganizations & Restaurants",
        "\nShare your surplus food with the community"
    );
    connect(organizationsBtn, &QPushButton::clicked, this, &MainWindow::onOrganizationsClicked);

    // People Button
    QPushButton* peopleBtn = createMainCardButton(
        "🥗",
        "\nPeople in Need",
        "\n\nRequest meals and assistance if you need help"
    );
    connect(peopleBtn, &QPushButton::clicked, this, &MainWindow::onPeopleClicked);

    btnLayout->addWidget(deliveryBtn);
    btnLayout->addWidget(organizationsBtn);
    btnLayout->addWidget(peopleBtn);
    mainLayout->addLayout(btnLayout);

    mainLayout->addStretch();

    // Footer
    QLabel* footer = new QLabel("Ready to start? Select an option to connect food with community");
    footer->setFont(QFont("Arial", 18));
    footer->setAlignment(Qt::AlignCenter);
    footer->setStyleSheet("color: #df6076; padding: 15px; font-weight: 800;");
    mainLayout->addWidget(footer);

    // Initialize modules
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
            // For organizations, we might need to restore specific data
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
    this->hide(); // Hide main window
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

