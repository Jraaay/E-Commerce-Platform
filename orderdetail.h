#ifndef ORDERDETAIL_H
#define ORDERDETAIL_H

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
#include "promptbox.h"
#include "productlistui.h"
#include "ui_productlistui.h"
#include "usermanager.h"
#include <QJsonArray>

namespace Ui {
class OrderDetail;
}

class OrderDetail : public QWidget
{
    Q_OBJECT

public:
    explicit OrderDetail(userClass *curUserFromFather, int orderId, void* father = nullptr, QWidget *parent = nullptr);
    ~OrderDetail();
    void init();
    void showProduct(bool getFromDB = false);
    QString geteElidedText(QFont font, QString str, int MaxWidth);
    userClass *curUser;                 // 当前用户
    int curFirstPhoto;                  // 当前的第一张图片
    int mainPhoto;                      // 主图片
    int curProduct;                     // 当前商品
    bool paid;
    bool canceled;
    long long time;
    vector<productItem *> productList;  // 商品列表
    vector<int> numberList;             // 购买量列表
    vector<bool> checkedList;           // 选中列表
    sqlite *db;                         // 数据库
    double priceSum;
    vector<int> count;
    vector<double> price;
    vector<QListWidgetItem *> itemList; // item列表
    vector<productListUi *> uiList;        // item的ui的列表
    void payForOrder();
    void cancelOrder();

private:
    Ui::OrderDetail *ui;
    int _orderId;
    void *_father;
};

#endif // ORDERDETAIL_H
