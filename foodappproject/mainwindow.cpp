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
        "   padding: 15px;"
        "   font-size: 18px;"
        "   font-weight: bold;"
        "   color: #ba6c3b;"
        "   text-align: center;"
        "}"
        "QPushButton:hover {"
        "   background-color: #e8cebe;"
        "   border-color: #4b240c;"
        "}";

    // Delivery Button
    QPushButton* deliveryBtn = new QPushButton(
        "\n\nDelivery\n\n\n\nClick here if you\ncan offer delivery\nservices"
    );
    deliveryBtn->setFixedSize(250, 300);
    deliveryBtn->setCursor(Qt::PointingHandCursor);
    deliveryBtn->setStyleSheet(buttonStyle);
    connect(deliveryBtn, &QPushButton::clicked, this, &MainWindow::onDeliveryClicked);

    // Organizations Button
    QPushButton* organizationsBtn = new QPushButton(
        "\nOrganizations\nand\nrestaurants\nShare your surplus\nfood with the\ncommunity and\nminimize your\nenvironmental impact."
    );
    organizationsBtn->setFixedSize(250, 300);
    organizationsBtn->setCursor(Qt::PointingHandCursor);
    organizationsBtn->setStyleSheet(buttonStyle);
    connect(organizationsBtn, &QPushButton::clicked, this, &MainWindow::onOrganizationsClicked);

    // People Button
    QPushButton* peopleBtn = new QPushButton(
        "\n\nPeople in Need\n\n\n\nRequest meals and\nassistance if you\nneed help"
    );
    peopleBtn->setFixedSize(250, 300);
    peopleBtn->setCursor(Qt::PointingHandCursor);
    peopleBtn->setStyleSheet(buttonStyle);
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

