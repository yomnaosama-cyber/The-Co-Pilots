#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QBoxLayout>
#include <QFrame>
#include <QLabel>

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
};

#endif // MAINWINDOW_H
