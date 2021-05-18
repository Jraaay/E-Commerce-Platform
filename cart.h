#ifndef CART_H
#define CART_H

#include <QWidget>
#include <vector>
#include "user.h"
#include "productitem.h"
#include "sqlite.h"
#include <QListWidgetItem>
#include "cartitemui.h"
#include <QScrollBar>
#include <stdio.h>
#include <fstream>
#include <QMouseEvent>
#include "promptbox.h"
#include <QDialog>
#include "cartitemui.h"
#include "ui_cartitemui.h"

namespace Ui {
class Cart;
}

class Cart : public QWidget
{
    Q_OBJECT

public:
    explicit Cart(userClass *curUserFromWidget, QWidget *parent = nullptr);
    ~Cart();
    void init();
    void showProduct(bool getFromDB = false);
    QString geteElidedText(QFont font, QString str, int MaxWidth);
    void onListMailItemClicked(QListWidgetItem *item);
    void showPhoto();
    bool eventFilter(QObject *obj, QEvent *event);
    void setMainPhoto(int mainPhotoNo);
    void prePhoto();
    void nextPhoto();
    void showBigPhoto();
    void refresh();
    void countPrice();
    void selectAll();
    userClass *curUser;                 // 当前用户
    int curFirstPhoto;                  // 当前的第一张图片
    int mainPhoto;                      // 主图片
    int curProduct;                     // 当前商品
    int sortMethod;                     // 排序方法
    vector<productItem *> productList;  // 商品列表
    vector<int> numberList;             // 购买量列表
    vector<bool> checkedList;           // 选中列表
    sqlite *db;                         // 数据库
    vector<vector<double>> discount;    // 折扣列表
    vector<QListWidgetItem *> itemList; // item列表
    vector<CartItemUi *> uiList;        // item的ui的列表

private:
    Ui::Cart *ui;
};

#endif // CART_H
