#ifndef DELIVERY_H
#define DELIVERY_H

#include <QMainWindow>
#include <QString>
#include <QVector>

struct MatchedDelivery {
    int deliveryId;
    QString pickupLocation;
    QString deliveryLocation;
    QString providerName;
    QString personName;
    int matchScore;
};
class DeliveryModule : public QMainWindow {
    Q_OBJECT
public:
    DeliveryModule(QWidget *parent = nullptr);
    ~DeliveryModule();
    void saveLoginState();
private slots:
    void handleSignUp();
    void handleNotifications();
    void handlePickup();
    void handleLogout();
private:
    void setupUI();
    void setupSignUpDialog();
    void setupNotificationsDialog();
    void setupLoginDialog();
    void addLogoutButton();
    class DeliveryModulePrivate *d;
};
#endif

