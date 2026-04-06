#ifndef ORG_H
#define ORG_H

#include <QDialog>
#include <QString>

class OrgModule : public QDialog {
    Q_OBJECT

public:
    OrgModule(QWidget *parent = nullptr);
    ~OrgModule();

private slots:
    void onRestaurantTab();
    void onOrganizationTab();
    void onDonationTab();
    void submitRestaurant();
    void submitOrganization();
    void submitDonation();

private:
    void setupUI();
    void setupRestaurantPage();
    void setupOrganizationPage();
    void setupDonationPage();
    
    // Pimpl pattern for encapsulation
    class OrgModulePrivate *d;
};

#endif // ORG_H

