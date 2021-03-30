#ifndef PRODUCT_H
#define PRODUCT_H

#include <QWidget>
#include "user.h"
#include "widget.h"
#include "productitem.h"
#include <QScrollBar>
#include "productlistui.h"
#include "sqlite.h"

namespace Ui {
class product;
}

class product : public QWidget
{
    Q_OBJECT

public:
    product(userClass *curUserFromWidget, QWidget *parent = nullptr);
    ~product();
    int curType;
    userClass *curUser;
    void logoutFun();
    void init();
    void test();
    int curPage;
    vector<productItem> productList;
    void showProduct();
    sqlite *db;

private:
    Ui::product *ui;
};

#endif // PRODUCT_H
