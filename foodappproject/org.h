#ifndef ORG_H
#define ORG_H

#include <QDialog>
#include <QString>

class OrgModule : public QDialog {
    Q_OBJECT

public:
    OrgModule(QWidget *parent = nullptr);
    ~OrgModule();
    void restoreFromData(const QString& data);
    void saveOrganizationState(const QString& orgName);

private slots:
    void onRestaurantTab();
    void onOrganizationTab();
    void onDonationTab();
    void submitRestaurant();
    void submitOrganization();
    void submitDonation();
    void handleLogout();

private:
    void setupUI();
    void setupRestaurantPage();
    void setupOrganizationPage();
    void setupDonationPage();
    void addLogoutButton();
    
    // Pimpl pattern for encapsulation
    class OrgModulePrivate *d;
};

#endif // ORG_H

