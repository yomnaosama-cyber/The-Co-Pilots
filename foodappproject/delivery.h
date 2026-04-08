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
private slots:
    void handleSignUp();
    void handleNotifications();
    void handlePickup();
private:
    void setupUI();
    void setupSignUpDialog();
    void setupNotificationsDialog();
    void setupLoginDialog();
    class DeliveryModulePrivate *d;
};
#endif

