#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class DeliveryModule;
class PeopleModule;
class OrgModule;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onDeliveryClicked();
    void onOrganizationsClicked();
    void onPeopleClicked();

private:
    void setupUI();
    
    // Modules
    DeliveryModule *deliveryModule;
    PeopleModule *peopleModule;
    OrgModule *orgModule;
};

#endif // MAINWINDOW_H
