#include "people.h"
#include <QWidget>
#include <QVBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QLineEdit>
#include <QDialog>
#include <QFormLayout>
#include <QMessageBox>
#include <QSqlQuery>
#include <QSqlError>
#include <QFont>

class PeopleModulePrivate {
public:
    QMainWindow *signwindow = nullptr;
    QLineEdit *peoplename = nullptr;
    QLineEdit *peopleid = nullptr;
    QLineEdit *peoplecontact = nullptr;
    QLineEdit *peoplenote = nullptr;
    
    QDialog *mealDialog = nullptr;
    QLineEdit *mealNumberLine = nullptr;
    QLineEdit *peopleaddress = nullptr;
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
    setMinimumSize(800, 600);
    setStyleSheet("QMainWindow { background-color: #efe4d0; }");

    QWidget* central = new QWidget();
    setCentralWidget(central);

    QVBoxLayout* layout = new QVBoxLayout(central);
    layout->setAlignment(Qt::AlignCenter);
    layout->setSpacing(40);

    QLabel* header = new QLabel("Request Meals and Assistance");
    header->setFont(QFont("Arial", 40, QFont::Bold));
    header->setAlignment(Qt::AlignCenter);
    header->setStyleSheet("color: #813e15; background-color: transparent; padding: 20px;");

    QPushButton* signupBtn = new QPushButton("Sign Up");
    signupBtn->setFixedSize(500, 200);
    signupBtn->setCursor(Qt::PointingHandCursor);
    signupBtn->setStyleSheet(
        "QPushButton {"
        "   background-color: #f4ece7;"
        "   border: 2px solid #813e15;"
        "   border-radius: 28px;"
        "   padding: 15px;"
        "   font-size: 30px;"
        "   font-weight: bold;"
        "   color: #ba6c3b;"
        "}"
        "QPushButton:hover {"
        "   background-color: #e8cebe;"
        "   border-color: #4b240c;"
        "}"
    );

    QPushButton* requestBtn = new QPushButton("Request Meals");
    requestBtn->setFixedSize(500, 200);
    requestBtn->setCursor(Qt::PointingHandCursor);
    requestBtn->setStyleSheet(
        "QPushButton {"
        "   background-color: #f4ece7;"
        "   border: 2px solid #813e15;"
        "   border-radius: 28px;"
        "   padding: 15px;"
        "   font-size: 30px;"
        "   font-weight: bold;"
        "   color: #ba6c3b;"
        "}"
        "QPushButton:hover {"
        "   background-color: #e8cebe;"
        "   border-color: #4b240c;"
        "}"
    );

    layout->addWidget(header);
    layout->addStretch();
    layout->addWidget(signupBtn, 0, Qt::AlignCenter);
    layout->addStretch();
    layout->addWidget(requestBtn, 0, Qt::AlignCenter);
    layout->addStretch();

    setupSignUpDialog();
    setupMealRequestDialog();

    connect(signupBtn, &QPushButton::clicked, this, &PeopleModule::handleSignUp);
    connect(requestBtn, &QPushButton::clicked, this, &PeopleModule::handleMealRequest);
}

void PeopleModule::setupSignUpDialog()
{
    d->signwindow = new QMainWindow();
    d->signwindow->setWindowTitle("Sign Up for Assistance");
    d->signwindow->setMinimumSize(600, 400);
    d->signwindow->setStyleSheet("QMainWindow { background-color: #efe4d0; }");

    QWidget* central = new QWidget();
    d->signwindow->setCentralWidget(central);

    QVBoxLayout* layout = new QVBoxLayout(central);
    layout->setAlignment(Qt::AlignCenter);
    layout->setSpacing(30);

    // Name
    QLabel* nameLabel = new QLabel("Enter Your Name:");
    nameLabel->setFont(QFont("Arial", 18, QFont::Bold));
    d->peoplename = new QLineEdit();
    d->peoplename->setPlaceholderText("Your Name");
    d->peoplename->setStyleSheet("font-size: 18px;");

    // ID
    QLabel* idLabel = new QLabel("Enter Your ID:");
    idLabel->setFont(QFont("Arial", 18, QFont::Bold));
    d->peopleid = new QLineEdit();
    d->peopleid->setPlaceholderText("Your ID");
    d->peopleid->setStyleSheet("font-size: 18px;");

    // Contact
    QLabel* contactLabel = new QLabel("Enter Your Contact Info:");
    contactLabel->setFont(QFont("Arial", 18, QFont::Bold));
    d->peoplecontact = new QLineEdit();
    d->peoplecontact->setPlaceholderText("Your Contact Info");
    d->peoplecontact->setStyleSheet("font-size: 18px;");

    // Notes
    QLabel* noteLabel = new QLabel("Enter any additional notes or dietary restrictions:");
    noteLabel->setFont(QFont("Arial", 18, QFont::Bold));
    d->peoplenote = new QLineEdit();
    d->peoplenote->setPlaceholderText("Additional Notes");
    d->peoplenote->setStyleSheet("font-size: 18px;");

    QPushButton* submitBtn = new QPushButton("Submit");
    submitBtn->setFixedSize(150, 50);
    submitBtn->setCursor(Qt::PointingHandCursor);
    submitBtn->setStyleSheet(
        "QPushButton {"
        "   background-color: #f4ece7;"
        "   border: 2px solid #813e15;"
        "   border-radius: 28px;"
        "   padding: 15px;"
        "   font-size: 18px;"
        "   font-weight: bold;"
        "   color: #ba6c3b;"
        "}"
        "QPushButton:hover {"
        "   background-color: #e8cebe;"
        "   border-color: #4b240c;"
        "}"
    );

    layout->addWidget(nameLabel);
    layout->addWidget(d->peoplename);
    layout->addWidget(idLabel);
    layout->addWidget(d->peopleid);
    layout->addWidget(contactLabel);
    layout->addWidget(d->peoplecontact);
    layout->addWidget(noteLabel);
    layout->addWidget(d->peoplenote);
    layout->addWidget(submitBtn);

    connect(submitBtn, &QPushButton::clicked, this, &PeopleModule::handleSignUp);
}

void PeopleModule::setupMealRequestDialog()
{
    d->mealDialog = new QDialog(this);
    d->mealDialog->setWindowTitle("Request Meals");
    d->mealDialog->setMinimumSize(400, 200);
    d->mealDialog->setStyleSheet("background-color: #efe4d0;");

    QVBoxLayout* layout = new QVBoxLayout(d->mealDialog);

    QLabel* mealLabel = new QLabel("How many meals do you need?");
    mealLabel->setFont(QFont("Arial", 18, QFont::Bold));
    mealLabel->setAlignment(Qt::AlignCenter);
    mealLabel->setStyleSheet("color: #813e15;");
    
    d->mealNumberLine = new QLineEdit();
    d->mealNumberLine->setPlaceholderText("Enter number of meals in numerals");
    d->mealNumberLine->setMinimumHeight(60);
    d->mealNumberLine->setAlignment(Qt::AlignCenter);
    d->mealNumberLine->setStyleSheet("font-size: 18px;");

    QLabel* addressLabel = new QLabel("Enter Your Address:");
    addressLabel->setFont(QFont("Arial", 18, QFont::Bold));
    addressLabel->setAlignment(Qt::AlignCenter);
    addressLabel->setStyleSheet("color: #813e15;");
    
    d->peopleaddress = new QLineEdit();
    d->peopleaddress->setPlaceholderText("Your Address");
    d->peopleaddress->setMinimumHeight(60);
    d->peopleaddress->setAlignment(Qt::AlignCenter);
    d->peopleaddress->setStyleSheet("font-size: 18px;");

    QPushButton* submitBtn = new QPushButton("Submit");
    submitBtn->setFixedSize(150, 50);
    submitBtn->setCursor(Qt::PointingHandCursor);
    submitBtn->setStyleSheet(
        "QPushButton {"
        "   background-color: #f4ece7;"
        "   border: 2px solid #813e15;"
        "   border-radius: 28px;"
        "   padding: 15px;"
        "   font-size: 18px;"
        "   font-weight: bold;"
        "   color: #ba6c3b;"
        "}"
        "QPushButton:hover {"
        "   background-color: #e8cebe;"
        "   border-color: #4b240c;"
        "}"
    );

    layout->addWidget(mealLabel);
    layout->addWidget(d->mealNumberLine);
    layout->addWidget(addressLabel);
    layout->addWidget(d->peopleaddress);
    layout->addWidget(submitBtn);

    connect(submitBtn, &QPushButton::clicked, this, &PeopleModule::handleMealRequest);
}

void PeopleModule::handleSignUp()
{
    d->signwindow->show();
}

void PeopleModule::handleMealRequest()
{
    d->mealDialog->exec();
}

