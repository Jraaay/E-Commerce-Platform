#ifndef USERCENTER_H
#define USERCENTER_H

#include <QWidget>
#include "product.h"
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
    explicit userCenter(userClass *curUserFromWidget, product *father, QWidget *parent = nullptr);
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
    userClass *curUser;              // 当前用户
    vector<vector<double>> discount; // 折扣列表
    int discountPlace = -1;          // 用于存哪一个商家
    product *fatherPtr;              // 父亲指针

private:
    Ui::userCenter *ui;
};

#endif // USERCENTER_H
