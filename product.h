#ifndef PRODUCT_H
#define PRODUCT_H

#include <QWidget>
#include "user.h"
#include "widget.h"
#include "productitem.h"
#include <QScrollBar>
#include "productlistui.h"
#include "sqlite.h"
#include "addproduct.h"
#include <QListWidgetItem>
#include <QTranslator>

#define DEFAULT_SORT 0
#define PRICE_DESCEND_SORT 1
#define PRICE_ASCEND_SORT 2

namespace Ui
{
    class product;
}

class product : public QWidget
{
    Q_OBJECT

public:
    product(QTranslator *translator, userClass *curUserFromWidget, QWidget *parent = nullptr);
    ~product();
    void logoutFun();
    void init();
    void openAddProduct();
    void onListMailItemClicked(QListWidgetItem *item);
    void showPhoto() const;
    void prePhoto();
    void nextPhoto();
    QString geteElidedText(QFont font, QString str, int MaxWidth) const;
    bool eventFilter(QObject *obj, QEvent *event);
    void setMainPhoto(int mainPhotoNo);
    void openUserCenter();
    void search();
    void showBigPhoto() const;
    void defaultSort();
    void priceDescendSort();
    void priceAscendSort();
    void purchase();
    void refresh();
    void manage();
    void refreshUser() const;
    void showProduct(bool getFromDB = false);
    userClass *curUser;                 // 当前用户
    int curFirstPhoto;                  // 当前的第一张图片
    int mainPhoto;                      // 主图片
    int curProduct;                     // 当前商品
    int sortMethod;                     // 排序方法
    vector<productItem *> productList;  // 商品列表
    sqlite *db;                         // 数据库
    vector<vector<double>> discount;    // 折扣列表
    QTranslator *translatorPtr;         // 翻译器指针
    vector<QListWidgetItem *> itemList; // item列表
    vector<productListUi *> uiList;     // item的ui的列表

private:
    Ui::product *ui;
};

#endif // PRODUCT_H
