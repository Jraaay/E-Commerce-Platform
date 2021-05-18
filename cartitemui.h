#ifndef CARTITEMUI_H
#define CARTITEMUI_H

#include <QWidget>
#include <QRegExpValidator>
#include "sqlite.h"

namespace Ui {
class CartItemUi;
}

class CartItemUi : public QWidget
{
    Q_OBJECT

public:
    explicit CartItemUi(void *father, int userId, int productId, QWidget *parent = nullptr);
    ~CartItemUi();
    Ui::CartItemUi *ui;
    void plus();
    void sub();
    void setValidator();
    void numberChange();
    void deleteCart();
    void checkedChange();

private:
    int _userId;
    int _productId;
    void *_father;
};

#endif // CARTITEMUI_H
