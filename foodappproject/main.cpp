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
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlRecord>
#include <QDialog>
#include <QVariant>
#include <QComboBox>
#include <QFormLayout>
#include <QStackedWidget>
#include <QTextEdit>
#include <QDebug>

bool initDatabase() {
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("food_sharing.db");

    if (!db.open()) {
        QMessageBox::critical(nullptr, "Database Error", "Could not open database");
        return false;
    }

    QSqlQuery query;
    query.exec("CREATE TABLE IF NOT EXISTS people_sign ("
               "id INTEGER PRIMARY KEY AUTOINCREMENT,"
               "name TEXT NOT NULL,"
               "people_id TEXT NOT NULL,"
               "address TEXT NOT NULL,"
               "contact TEXT NOT NULL,"
               "notes TEXT,"
               "signup_date DATETIME DEFAULT CURRENT_TIMESTAMP)");

    query.exec("CREATE TABLE IF NOT EXISTS meal_requests ("
               "id INTEGER PRIMARY KEY AUTOINCREMENT,"
               "person_id INTEGER,"
               "meal_count INTEGER NOT NULL,"
               "request_date DATETIME DEFAULT CURRENT_TIMESTAMP,"
               "FOREIGN KEY(person_id) REFERENCES people_sign(id))");

    query.exec("CREATE TABLE IF NOT EXISTS users ("
               "id INTEGER PRIMARY KEY AUTOINCREMENT, "
               "name TEXT, "
               "national_id TEXT, "
               "age TEXT, "
               "city TEXT, "
               "vehicle TEXT)");

    query.exec("CREATE TABLE IF NOT EXISTS providers ("
               "id INTEGER PRIMARY KEY AUTOINCREMENT, "
               "role TEXT, "
               "name TEXT, "
               "location TEXT, "
               "food_type TEXT, "
               "contact_info TEXT, "
               "organization_type TEXT, "
               "purpose TEXT)");

    return true;
}

int main(int argc, char* argv[]) {
    QApplication app (argc, argv);

    if (!initDatabase()) {
        return -1;
    }

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

    QPushButton* organizations = new QPushButton("\nOrganizations\n and \n restaurants\nShare your surplus \n food with the\n community and \nminimize your\n environmental impact.");
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
    btnlayout->addWidget(people);
    mainlayout->addLayout(btnlayout);

    mainlayout->addStretch();
    mainlayout->addWidget(footer);

    QMainWindow* delivarywindow = new QMainWindow();
    delivarywindow->setWindowTitle("Delivery");
    delivarywindow->setMinimumSize(800, 600);
    delivarywindow->setStyleSheet("QMainWindow { background-color: #efe4d0; }");

    QWidget* delivarycentral = new QWidget();
    delivarywindow->setCentralWidget(delivarycentral);

    QVBoxLayout* delivarylayout = new QVBoxLayout(delivarycentral);
    delivarylayout->setAlignment(Qt::AlignHCenter); // horizontal center
    delivarylayout->setSpacing(40);               // spacing between widgets

    // Header
    QLabel* delivaryheader = new QLabel("Delivery System");
    delivaryheader->setFont(QFont("Arial", 40, QFont::Bold));
    delivaryheader->setAlignment(Qt::AlignCenter);
    delivaryheader->setStyleSheet("color: #813e15; background-color: transparent; padding: 20px;");

    // Buttons
    QPushButton* Btn1 = new QPushButton("Sign up");
    QPushButton* Btn2 = new QPushButton("Delivery notifications");
    QPushButton* Btn3 = new QPushButton("Pickup");

    Btn1->setObjectName("btn1");
    Btn2->setObjectName("btn2");
    Btn3->setObjectName("btn3");
    Btn1->setStyleSheet(
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
    Btn2->setStyleSheet(
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
    Btn3->setStyleSheet(
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
    // Add vertical centering using stretch
    delivarylayout->addStretch();           // top space
    delivarylayout->addWidget(delivaryheader);
    delivarylayout->addWidget(Btn1);
    delivarylayout->addWidget(Btn2);
    delivarylayout->addWidget(Btn3);
    delivarylayout->addStretch();           // bottom space


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

        QSqlQuery query;
        query.prepare("INSERT INTO people_sign (name, people_id, address, contact, notes) "
                      "VALUES (:name, :id, :address, :contact, :notes)");
        query.bindValue(":name", name);
        query.bindValue(":id", id);
        query.bindValue(":address", address);
        query.bindValue(":contact", contact);
        query.bindValue(":notes", note);

        if (query.exec()) {
            QMessageBox::information(signwindow, "Request Submitted", "Your sign-up information has been submitted.");
            peoplename->clear();
            peopleid->clear();
            peopleaddress->clear();
            peoplecontact->clear();
            peoplenote->clear();
            signwindow->close();
        } else {
            QMessageBox::critical(signwindow, "Database Error", "Could not save data: " + query.lastError().text());
        }
    });

    QObject::connect(peoplerequest, &QPushButton::clicked, [=]() { mealDialog->exec(); });

    QObject::connect(submitMealRequest, &QPushButton::clicked, [=]() {
        QString meals = mealNumberLine->text();
        if (meals.isEmpty() || meals.toInt() <= 0) {
            QMessageBox::warning(peoplewindow, "Input Error", "Please enter the number of meals you need.");
            return;
        }

        QSqlQuery query;
        query.exec("SELECT id FROM people_sign ORDER BY id DESC LIMIT 1");

        int personId = -1;
        if (query.next()) {
            personId = query.value(0).toInt();
        }

        if (personId == -1) {
            QMessageBox::warning(peoplewindow, "No Sign-up Found", "Please sign up first before requesting meals.");
            mealNumberLine->clear();
            mealDialog->close();
            return;
        }
query.finish();
        query.prepare("INSERT INTO meal_requests (person_id, meal_count) VALUES (:person_id, :meal_count)");
        query.bindValue(":person_id", personId);
        query.bindValue(":meal_count", meals.toInt());

        if (query.exec()) {
            QMessageBox::information(peoplewindow, "Request Submitted", "Your meal request for " + meals + " meals has been submitted.");
            mealNumberLine->clear();
            mealDialog->close();
        } else {
            QMessageBox::critical(peoplewindow, "Database Error", "Could not save meal request: " + query.lastError().text());
        }
    });

    QObject::connect(people, &QPushButton::clicked, [=]() { peoplewindow->show(); });


    QObject::connect(organizations, &QPushButton::clicked, [&]() {
        QDialog dialog(&window);  // Use the main window as parent
        dialog.setWindowTitle("Provider Registration");
        dialog.setMinimumSize(520, 560);

dialog.setStyleSheet(
    "QDialog { background-color: #efe4d0; }"

    "QLabel { color: #813e15; font-size: 14px; }"

    "QLineEdit, QTextEdit, QComboBox {"
    " background-color: white;"
    " color: black;"
    " border: 2px solid #813e15;"
    " border-radius: 10px;"
    " padding: 6px;"
    "}"

    "QPushButton {"
    " background-color: #813e15;"
    " color: white;"
    " border-radius: 12px;"
    " padding: 10px;"
    " font-weight: bold;"
    "}"
);

        QVBoxLayout *mainLayout = new QVBoxLayout(&dialog);

        QLabel *title = new QLabel("Community Food Connect", &dialog);
        title->setAlignment(Qt::AlignCenter);
        title->setStyleSheet("font-size: 24px; font-weight: 700; color: white;");
        mainLayout->addWidget(title);

        // Switch buttons
        QHBoxLayout *switchLayout = new QHBoxLayout();
        QPushButton *restaurantButton = new QPushButton("Restaurant", &dialog);
        QPushButton *organizationButton = new QPushButton("Organization", &dialog);
        switchLayout->addWidget(restaurantButton);
        switchLayout->addWidget(organizationButton);
        mainLayout->addLayout(switchLayout);

        // Stacked widget
        QStackedWidget *stack = new QStackedWidget(&dialog);
        mainLayout->addWidget(stack);

        // --- Restaurant Page ---
        QWidget *restaurantPage = new QWidget();
        QVBoxLayout *restaurantPageLayout = new QVBoxLayout(restaurantPage);
        QFormLayout *restaurantForm = new QFormLayout();

        QLineEdit *resName = new QLineEdit();
        QLineEdit *resLocation = new QLineEdit();
        QComboBox *resFoodType = new QComboBox();
        QLineEdit *resContact = new QLineEdit();
        resFoodType->addItems({"Bakery", "Fast Food", "Groceries", "Home Meals", "Restaurant Meals", "Vegetarian", "Other"});

        restaurantForm->addRow("Restaurant Name:", resName);
        restaurantForm->addRow("Location:", resLocation);
        restaurantForm->addRow("Food Type:", resFoodType);
        restaurantForm->addRow("Contact Info:", resContact);

        QPushButton *restaurantSubmit = new QPushButton("Register Restaurant");
        restaurantPageLayout->addLayout(restaurantForm);
        restaurantPageLayout->addWidget(restaurantSubmit);
        stack->addWidget(restaurantPage);

        // --- Organization Page ---
        QWidget *organizationPage = new QWidget();
        QVBoxLayout *organizationPageLayout = new QVBoxLayout(organizationPage);
        QFormLayout *organizationForm = new QFormLayout();

        QLineEdit *orgName = new QLineEdit();
        QComboBox *orgType = new QComboBox();
        QLineEdit *orgLocation = new QLineEdit();
        QTextEdit *orgPurpose = new QTextEdit();
        QLineEdit *orgContact = new QLineEdit();
        orgType->addItems({"Charity", "Community Kitchen", "Food Bank", "Mosque", "NGO", "Shelter", "Other"});

        organizationForm->addRow("Organization Name:", orgName);
        organizationForm->addRow("Organization Type:", orgType);
        organizationForm->addRow("Location:", orgLocation);
        organizationForm->addRow("Purpose:", orgPurpose);
        organizationForm->addRow("Contact Info:", orgContact);

        QPushButton *organizationSubmit = new QPushButton("Register Organization");
        organizationPageLayout->addLayout(organizationForm);
        organizationPageLayout->addWidget(organizationSubmit);
        stack->addWidget(organizationPage);

        // Switch pages
        QObject::connect(restaurantButton, &QPushButton::clicked, [=]() { stack->setCurrentIndex(0); });
        QObject::connect(organizationButton, &QPushButton::clicked, [=]() { stack->setCurrentIndex(1); });

        // Submissions
        QObject::connect(restaurantSubmit, &QPushButton::clicked, [&]() {
            if(resName->text().trimmed().isEmpty() ||
                resLocation->text().trimmed().isEmpty() ||
                resContact->text().trimmed().isEmpty()) {
                QMessageBox::warning(&dialog, "Missing Information", "Please fill in all restaurant fields.");
                return;
            }
            QMessageBox::information(&dialog, "Success", "Restaurant registered successfully.");
            dialog.accept();
        });

        QObject::connect(organizationSubmit, &QPushButton::clicked, [&]() {
            if(orgName->text().trimmed().isEmpty() ||
                orgLocation->text().trimmed().isEmpty() ||
                orgPurpose->toPlainText().trimmed().isEmpty() ||
                orgContact->text().trimmed().isEmpty()) {
                QMessageBox::warning(&dialog, "Missing Information", "Please fill in all organization fields.");
                return;
            }
            QMessageBox::information(&dialog, "Success", "Organization registered successfully.");
            dialog.accept();
        });

        dialog.exec();
    });
QObject::connect(Btn1, &QPushButton::clicked, [&]() {
    QDialog dialog(&window);  // Correct parent
    dialog.setWindowTitle("User Questions");
    dialog.setMinimumSize(300, 400);
    dialog.setStyleSheet("QDialog { background-color: #1a1a2e; } QLabel { color: white; } QLineEdit { padding: 5px; border-radius: 3px; }");

    QVBoxLayout *dialogLayout = new QVBoxLayout(&dialog);
    QStringList questions = {"Name", "National ID number", "Age", "Preferred City/Town", "Vehicle type"};
    QList<QLineEdit*> inputFields;

    for (const QString &qText : questions) {
        dialogLayout->addWidget(new QLabel(qText, &dialog));
        QLineEdit* edit = new QLineEdit(&dialog);
        dialogLayout->addWidget(edit);
        inputFields.append(edit);
    }

    QPushButton* submitBtn = new QPushButton("Submit", &dialog);
    submitBtn->setStyleSheet("background-color: #4facfe; color: white; padding: 8px; border-radius: 5px;");
    dialogLayout->addWidget(submitBtn);

    QObject::connect(submitBtn, &QPushButton::clicked, &dialog, &QDialog::accept);

    if (dialog.exec() == QDialog::Accepted) {
        QSqlQuery query;
        query.prepare("INSERT INTO users (name, national_id, age, city, vehicle) VALUES (:name, :nid, :age, :city, :vehicle)");
        query.bindValue(":name", inputFields[0]->text());
        query.bindValue(":nid", inputFields[1]->text());
        query.bindValue(":age", inputFields[2]->text());
        query.bindValue(":city", inputFields[3]->text());
        query.bindValue(":vehicle", inputFields[4]->text());

        if (query.exec()) {
            QMessageBox::information(&window, "Success", "Data Saved to Database!");
        } else {
            QMessageBox::critical(&window, "Error", "Database Error: " + query.lastError().text());
        }
    }
});

    QObject::connect(delivery, &QPushButton::clicked, [=]() {
        delivarywindow->show();  // show the delivery window
        delivarywindow->raise(); // bring it to front if already open
        delivarywindow->activateWindow(); // focus on it
    });



    window.show();
    return app.exec();
}
