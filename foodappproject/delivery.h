#ifndef DELIVERY_H
#define DELIVERY_H

#include <QMainWindow>
#include <QString>

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
    
    // Pimpl pattern for encapsulation
    class DeliveryModulePrivate *d;
};

#endif // DELIVERY_H

