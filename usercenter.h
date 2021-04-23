#ifndef USERCENTER_H
#define USERCENTER_H

#include <QWidget>
#include "user.h"
#include "promptbox.h"
#include <fstream>
#include "ui_usercenter.h"
#include <QTabBar>
#include "rechargepage.h"
#include "sqlite.h"

namespace Ui
{
    class userCenter;
}

class userCenter : public QWidget
{
    Q_OBJECT

public:
    explicit userCenter(userClass *curUserFromWidget, QWidget *parent = nullptr);
    ~userCenter();
    void init();
    void changeUserName();
    void changePassword();
    void toBalancePage();
    void toAccount();
    void toDiscuss();
    void recharge();
    void rechargeConfirm(double moneyToCharge);
    void resetDiscount();
    void saveDiscount();
    userClass *curUser;
    vector<vector<double>> discount;
    int discountPlace = -1;

private:
    Ui::userCenter *ui;
};

#endif // USERCENTER_H
