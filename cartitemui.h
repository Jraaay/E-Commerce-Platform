#ifndef CARTITEMUI_H
#define CARTITEMUI_H

#include <QWidget>

namespace Ui {
class CartItemUi;
}

class CartItemUi : public QWidget
{
    Q_OBJECT

public:
    explicit CartItemUi(QWidget *parent = nullptr);
    ~CartItemUi();

private:
    Ui::CartItemUi *ui;
};

#endif // CARTITEMUI_H
