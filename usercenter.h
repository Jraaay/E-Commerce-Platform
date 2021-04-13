#ifndef USERCENTER_H
#define USERCENTER_H

#include <QWidget>
#include "user.h"
#include "promptbox.h"
#include <fstream>
#include "ui_usercenter.h"
#include "ui_usercenterbalance.h"
#include <QTabBar>

namespace Ui {
class userCenter;
class userCenterBalance;
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
    userClass *curUser;

private:
    Ui::userCenter *ui;
    Ui::userCenterBalance *uiBalance;
};

#endif // USERCENTER_H
