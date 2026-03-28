#include <QApplication>
#include <QMainWindow>
#include <QPushButton>
#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QPainter>
#include <QPropertyAnimation>
#include <QMouseEvent>
#include <QLabel>
#include <QMessageBox>
#include <QFile>
#include <QTextStream>

int main(int argc, char* argv[]) {
    QApplication app (argc, argv);

    QMainWindow window;
    window.setWindowTitle("Co-Pilots");
    window.setMinimumSize(1000, 800);
    window.setStyleSheet("QMainWindow { background-color: #efe4d0; }");

    QWidget* central = new QWidget();
    window.setCentralWidget(central);

    QVBoxLayout* mainlayout = new QVBoxLayout(central);
    mainlayout->setSpacing(40); 

    QHBoxLayout* btnlayout = new QHBoxLayout();
    mainlayout->setSpacing(100);
    btnlayout->setAlignment(Qt::AlignCenter);

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

    QPushButton* delivery = new QPushButton("\n\nDelivery\n\n\n\nClick here if you \ncan offer delivery\n services");
    delivery->setFixedSize(250, 300);
    delivery->setCursor(Qt::PointingHandCursor);
    delivery->setStyleSheet(
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
        "   transform: translateY(-5px);"
        "}"
    );

    QPushButton* organizations = new QPushButton("\n\nOrganizations\n\n\n\nPartner with us to \njoin a network \nworking to reduce\n food waste");
    organizations->setFixedSize(250, 300);
    organizations->setCursor(Qt::PointingHandCursor);
    organizations->setStyleSheet(
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
        "   transform: translateY(-5px);"
        "}"
    );

    QPushButton* restaurants = new QPushButton("\n\nRestaurants\n\n\n\nDonate your \nsurplus food from \ntoday's menu");
    restaurants->setFixedSize(250, 300);
    restaurants->setCursor(Qt::PointingHandCursor);
    restaurants->setStyleSheet(
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
        "   transform: translateY(-5px);"
        "}"
    );

    QPushButton* people = new QPushButton("\n\nPeople in Need\n\n\n\nRequest meals and \nassistance if you \nneed help");
    people->setFixedSize(250, 300);
    people->setCursor(Qt::PointingHandCursor);
    people->setStyleSheet(
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
        "   transform: translateY(-5px);"
        "}"
    );

    QLabel* footer = new QLabel("Ready to start? Select an option to connect food with community");
    footer->setFont(QFont("Arial", 18));
    footer->setAlignment(Qt::AlignCenter);
    footer->setStyleSheet("color: #813e15; padding: 15px;");

    headerLayout->addWidget(header);
    mainlayout->addWidget(headerWidget);

    mainlayout->addStretch();
    btnlayout->addWidget(delivery);
    btnlayout->addWidget(organizations);
    btnlayout->addWidget(restaurants);
    btnlayout->addWidget(people);
    mainlayout->addLayout(btnlayout);

    mainlayout->addStretch();
    mainlayout->addWidget(footer);



    QMainWindow* peoplewindow = new QMainWindow();
    peoplewindow->setWindowTitle("People in Need");
    peoplewindow->setMinimumSize(800, 600);
    peoplewindow->setStyleSheet("QMainWindow { background-color: #efe4d0; }");

    QWidget* peoplecentral = new QWidget();
    peoplewindow->setCentralWidget(peoplecentral);

    QVBoxLayout* peoplelayout = new QVBoxLayout(peoplecentral);
    peoplelayout->setAlignment(Qt::AlignCenter);
    peoplelayout->setSpacing(40); 

    QLabel* peopleheader = new QLabel("Request Meals and Assistance");
    peopleheader->setFont(QFont("Arial", 40, QFont::Bold));
    peopleheader->setAlignment(Qt::AlignCenter);
    peopleheader->setStyleSheet("color: #813e15; background-color: transparent; padding: 20px;");

    QPushButton* peoplesign = new QPushButton("Sign Up");
    peoplesign->setFixedSize(500, 200);
    peoplesign->setCursor(Qt::PointingHandCursor);
    peoplesign->setStyleSheet(
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
        "   transform: translateY(-5px);"
        "}"
    );

    QPushButton* peoplerequest = new QPushButton("Request Meals");
    peoplerequest->setFixedSize(500, 200);
    peoplerequest->setCursor(Qt::PointingHandCursor);
    peoplerequest->setStyleSheet(
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
        "   transform: translateY(-5px);"
        "}"
    );

    peoplelayout->addWidget(peopleheader);
    peoplelayout->addStretch();
    peoplelayout->addWidget(peoplesign, 0, Qt::AlignCenter);
    peoplelayout->addStretch();
    peoplelayout->addWidget(peoplerequest, 0, Qt::AlignCenter);
    peoplelayout->addStretch();



    QMainWindow* signwindow = new QMainWindow();
    signwindow->setWindowTitle("Sign Up for Assistance");
    signwindow->setMinimumSize(600, 400);
    signwindow->setStyleSheet("QMainWindow { background-color: #efe4d0; }");

    QWidget* signcentral = new QWidget();
    signwindow->setCentralWidget(signcentral);

    QVBoxLayout* signlayout = new QVBoxLayout(signcentral);
    signlayout->setAlignment(Qt::AlignCenter);
    signlayout->setSpacing(30);

    QLabel* signname = new QLabel("Enter Your Name:");
    signname->setFont(QFont("Arial", 18, QFont::Bold));
    QLineEdit* peoplename = new QLineEdit();
    peoplename->setPlaceholderText("Your Name");
    peoplename->setStyleSheet("font-size: 18px;");

    QLabel* signid = new QLabel("Enter Your ID:");
    signid->setFont(QFont("Arial", 18, QFont::Bold));
    QLineEdit* peopleid = new QLineEdit();
    peopleid->setPlaceholderText("Your ID");
    peopleid->setStyleSheet("font-size: 18px;");

    QLabel* signaddress = new QLabel("Enter Your Address:");
    signaddress->setFont(QFont("Arial", 18, QFont::Bold));
    QLineEdit* peopleaddress = new QLineEdit();
    peopleaddress->setPlaceholderText("Your Address");
    peopleaddress->setStyleSheet("font-size: 18px;");

    QLabel* signcontact = new QLabel("Enter Your Contact Info:");
    signcontact->setFont(QFont("Arial", 18, QFont::Bold));
    QLineEdit* peoplecontact = new QLineEdit();
    peoplecontact->setPlaceholderText("Your Contact Info");
    peoplecontact->setStyleSheet("font-size: 18px;");

    QLabel* signnote = new QLabel("Enter any additional notes or dietary restrictions:");
    signnote->setFont(QFont("Arial", 18, QFont::Bold));
    QLineEdit* peoplenote = new QLineEdit();
    peoplenote->setPlaceholderText("Additional Notes");
    peoplenote->setStyleSheet("font-size: 18px;");

    QPushButton* peoplesubmit = new QPushButton("Submit");
    peoplesubmit->setFixedSize(150, 50);
    peoplesubmit->setCursor(Qt::PointingHandCursor);
    peoplesubmit->setStyleSheet(
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
        "   transform: translateY(-5px);"
        "}"
    );

    signlayout->addWidget(signname);
    signlayout->addWidget(peoplename);
    signlayout->addWidget(signid);
    signlayout->addWidget(peopleid);
    signlayout->addWidget(signaddress);
    signlayout->addWidget(peopleaddress);
    signlayout->addWidget(signcontact);
    signlayout->addWidget(peoplecontact);
    signlayout->addWidget(signnote);
    signlayout->addWidget(peoplenote);
    signlayout->addWidget(peoplesubmit);

    QDialog* mealDialog = new QDialog();
    mealDialog->setWindowTitle("Request Meals");
    mealDialog->setMinimumSize(400, 200);
    mealDialog->setStyleSheet("background-color: #efe4d0;");
    
    QVBoxLayout* dialogLayout = new QVBoxLayout(mealDialog);
    
    QLabel* mealLabel = new QLabel("How many meals do you need?");
    mealLabel->setFont(QFont("Arial", 18, QFont::Bold));
    mealLabel->setAlignment(Qt::AlignCenter);
    mealLabel->setStyleSheet("color: #813e15;");
    
    QLineEdit* mealNumberLine = new QLineEdit();
    mealNumberLine->setPlaceholderText("Enter number of meals in numerals");
    mealNumberLine->setMinimumHeight(60);
    mealNumberLine->setAlignment(Qt::AlignCenter);
    mealNumberLine->setStyleSheet("font-size: 18px;");
    
    QPushButton* submitMealRequest = new QPushButton("Submit");
    submitMealRequest->setFixedSize(150, 50);
    submitMealRequest->setCursor(Qt::PointingHandCursor);
    submitMealRequest->setStyleSheet(
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
    
    dialogLayout->addWidget(mealLabel);
    dialogLayout->addWidget(mealNumberLine);
    dialogLayout->addWidget(submitMealRequest);

    QObject::connect(peoplesign, &QPushButton::clicked, [=]() { signwindow->show(); });

    QObject::connect(peoplesubmit, &QPushButton::clicked, [=]() {
        QString name = peoplename->text();
        QString id = peopleid->text();
        QString address = peopleaddress->text();
        QString contact = peoplecontact->text();
        QString note = peoplenote->text();
        if (name.isEmpty() || id.isEmpty() || address.isEmpty() || contact.isEmpty()) {
            QMessageBox::warning(signwindow, "Input Error", "Please fill in all required fields.");
            return;
        }
        QFile file("people_sign.txt");
        if (file.open(QIODevice::Append | QIODevice::Text)) {
            QTextStream out(&file);
            out << "Name: " << name << ", ID: " << id << ", Address: " << address
                << ", Contact: " << contact << ", Notes: " << note << Qt::endl;
            file.close();
            QMessageBox::information(signwindow, "Request Submitted", "Your sign-up information has been submitted.");
            peoplename->clear();
            peopleid->clear();
            peopleaddress->clear();
            peoplecontact->clear();
            peoplenote->clear();
            signwindow->close();
        } else {
            QMessageBox::critical(signwindow, "File Error", "Could not open people_sign.txt for writing.");
        }
    });

    QObject::connect(peoplerequest, &QPushButton::clicked, [=]() { mealDialog->exec(); });
    
    QObject::connect(submitMealRequest, &QPushButton::clicked, [=]() {
        QString meals = mealNumberLine->text();
        if (meals.isEmpty() || meals.toInt() <= 0) {
            QMessageBox::warning(peoplewindow, "Input Error", "Please enter the number of meals you need.");
            return;
        }

        QString details;
        QFile file1("people_sign.txt");
        if (file1.open(QIODevice::ReadOnly | QIODevice::Text)) {
            QTextStream in(&file1);
            details = in.readAll();
            file1.close();
        } else {
            QMessageBox::critical(peoplewindow, "File Error", "Could not open people_sign.txt for reading.");
        }

        QFile file2("requests.txt");
        if (file2.open(QIODevice::Append | QIODevice::Text)) {
            QTextStream out(&file2);
            out << "Request for " << meals << " meals." << Qt::endl;
            out << "Request from: " << details << Qt::endl;
            file2.close();
            QMessageBox::information(peoplewindow, "Request Submitted", "Your meal request has been submitted.");
        } else {
            QMessageBox::critical(peoplewindow, "File Error", "Could not open requests.txt for writing.");
        }
        mealNumberLine->clear();
        mealDialog->close();
    });

    QObject::connect(people, &QPushButton::clicked, [=]() { peoplewindow->show(); });

    window.show();
    return app.exec();
}
