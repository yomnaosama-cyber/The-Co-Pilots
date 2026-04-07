#ifndef PEOPLE_H
#define PEOPLE_H

#include <QMainWindow>
#include <QString>

class PeopleModule : public QMainWindow {
    Q_OBJECT

public:
    PeopleModule(QWidget *parent = nullptr);
    ~PeopleModule();

private slots:
    void handleSignUp();
    void handleMealRequest();

private:
    void setupUI();
    void setupSignUpDialog();
    void setupMealRequestDialog();
    void submitSignUp();
    void submitMealRequest();
    
    // Pimpl pattern for encapsulation
    class PeopleModulePrivate *d;
};

#endif // PEOPLE_H

