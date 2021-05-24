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
#include "orderitem.h"
#include "ui_orderitem.h"

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
    void toBalancePage() const;
    void toAccount() const;
    void toDiscuss() const;
    void toOrders();
    void recharge();
    void rechargeConfirm(double moneyToCharge);
    void resetDiscount() const;
    void saveDiscount();
    void showOrders();
    vector<int> orderId;
    userClass *curUser;              // 当前用户

    int discountPlace = -1;          // 用于存哪一个商家
    product *fatherPtr;              // 父亲指针
    vector<QListWidgetItem *> itemList; // item列表
    vector<OrderItem *> uiList;     // item的ui的列表
    void onListMailItemClicked(QListWidgetItem *item);
    double fooddiscount, clothesdiscount, bookdiscount;

private:
    Ui::userCenter *ui;
};

#endif // USERCENTER_H
