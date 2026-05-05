#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QBoxLayout>
#include <QFrame>
#include <QLabel>

class QGridLayout;
class QPushButton;
class DeliveryModule;
class PeopleModule;
class OrgModule;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void showModuleDirectly(const QString& module, const QString& moduleData);
    void clearSavedModule(); 

private slots:
    void onDeliveryClicked();
    void onOrganizationsClicked();
    void onPeopleClicked();

private:
    void setupUI();
    void resizeEvent(QResizeEvent* event) override;
    
    // Modules
    DeliveryModule *deliveryModule;
    PeopleModule *peopleModule;
    OrgModule *orgModule;

    // Responsive helpers
    QBoxLayout* m_heroLayout = nullptr;
    QFrame* m_riderScene = nullptr;
    QLabel* m_heroTitle = nullptr;
    QGridLayout* m_roleGrid = nullptr;
    QPushButton* m_deliveryButton = nullptr;
    QPushButton* m_organizationsButton = nullptr;
    QPushButton* m_peopleButton = nullptr;
    QPushButton* m_howItWorksNav = nullptr;
    QPushButton* m_aboutNav = nullptr;
    QPushButton* m_contactNav = nullptr;
};

#endif // MAINWINDOW_H
