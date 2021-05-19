#ifndef RECHARGEPAGE_H
#define RECHARGEPAGE_H

#include <QWidget>
#include <QRegExpValidator>
#include "promptbox.h"
#include "usercenter.h"

namespace Ui
{
    class rechargePage;
}

class rechargePage : public QWidget
{
    Q_OBJECT

public:
    explicit rechargePage(QWidget *parent = nullptr, QObject *uc = nullptr);
    ~rechargePage();
    void init() const;
    void buy6();
    void buy30();
    void buy98();
    void buy198();
    void buy328();
    void buy648();
    void buyOther();
    void rechargeConfirm();
    QObject *parentuserCenter; // 父亲指针
    double price = 0;          // 充值金额

private:
    Ui::rechargePage *ui;
};

#endif // RECHARGEPAGE_H
