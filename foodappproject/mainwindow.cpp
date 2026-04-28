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
    setStyleSheet("QMainWindow { background-color: #efe4d0; }");

    QWidget* central = new QWidget();
    setCentralWidget(central);

    QVBoxLayout* mainLayout = new QVBoxLayout(central);
    mainLayout->setSpacing(40);
    mainLayout->setContentsMargins(20, 20, 20, 20);

    // Header Widget
    QWidget* headerWidget = new QWidget();
    headerWidget->setStyleSheet(
        "QWidget {"
        "   background-color: #813e15;"
        "}"
    );
    headerWidget->setFixedHeight(140);

    QVBoxLayout* headerLayout = new QVBoxLayout(headerWidget);
    headerLayout->setAlignment(Qt::AlignCenter);

    QLabel* header = new QLabel("Connect excess food - Reduce waste - Fight hunger");
    header->setFont(QFont("Arial", 30, QFont::Bold));
    header->setAlignment(Qt::AlignCenter);
    header->setStyleSheet("color: #f4ece7; background-color: transparent; padding: 20px;");

    headerLayout->addWidget(header);
    mainLayout->addWidget(headerWidget);

    // Main buttons layout
    mainLayout->addStretch();
    
    QHBoxLayout* btnLayout = new QHBoxLayout();
    btnLayout->setAlignment(Qt::AlignCenter);
    btnLayout->setSpacing(30);

    QString buttonStyle =
        "QPushButton {"
        "   background-color: #f4ece7;"
        "   border: 2px solid #813e15;"
        "   border-radius: 28px;"
        "   padding: 0px;"
        "   text-align: left;"
        "}"
        "QPushButton:hover {"
        "   background-color: #e8cebe;"
        "   border-color: #4b240c;"
        "}";

    auto createMainCardButton = [&](const QString& title, const QString& description) {
        QPushButton* cardBtn = new QPushButton();
        cardBtn->setFixedSize(250, 300);
        cardBtn->setCursor(Qt::PointingHandCursor);
        cardBtn->setStyleSheet(buttonStyle);

        QVBoxLayout* cardLayout = new QVBoxLayout(cardBtn);
        cardLayout->setContentsMargins(20, 20, 20, 20);
        cardLayout->setSpacing(8);
        cardLayout->setAlignment(Qt::AlignCenter);

        QLabel* titleLabel = new QLabel(title, cardBtn);
        titleLabel->setWordWrap(true);
        titleLabel->setAlignment(Qt::AlignTop | Qt::AlignHCenter);
        titleLabel->setStyleSheet("color: #813e15; font-size: 27px; font-weight: 800; background: transparent;");

        QLabel* descLabel = new QLabel(description, cardBtn);
        descLabel->setWordWrap(true);
        descLabel->setAlignment(Qt::AlignCenter | Qt::AlignHCenter);
        descLabel->setStyleSheet("color: #4b240c; font-size: 20px; font-weight: 500; background: transparent;");

        cardLayout->addWidget(titleLabel);
        cardLayout->addWidget(descLabel);
        cardLayout->addStretch();

        return cardBtn;
    };

    // Delivery Button
    QPushButton* deliveryBtn = createMainCardButton(
        "\nDelivery System",
        "\n\nClick here if you can offer delivery services"
    );
    connect(deliveryBtn, &QPushButton::clicked, this, &MainWindow::onDeliveryClicked);

    // Organizations Button
    QPushButton* organizationsBtn = createMainCardButton(
        "\nOrganizations & Restaurants",
        "\nShare your surplus food with the community"
    );
    connect(organizationsBtn, &QPushButton::clicked, this, &MainWindow::onOrganizationsClicked);

    // People Button
    QPushButton* peopleBtn = createMainCardButton(
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
    footer->setStyleSheet("color: #813e15; padding: 15px;");
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

