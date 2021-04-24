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
    int curType;
    userClass *curUser;
    void logoutFun();
    void init();
    void test();
    void onListMailItemClicked(QListWidgetItem *item);
    int curFirstPhoto;
    int mainPhoto;
    int curProduct;
    void showPhoto();
    void prePhoto();
    void nextPhoto();
    QString geteElidedText(QFont font, QString str, int MaxWidth);
    bool eventFilter(QObject *obj, QEvent *event);
    void setMainPhoto(int mainPhotoNo);
    void openUserCenter();
    void search();
    void showBigPhoto();
    void defaultSort();
    void priceDescendSort();
    void priceAscendSort();
    void purchase();
    void refresh();
    int curPage;
    int sortMethod;
    void manage();
    void refreshUser();
    vector<productItem *> productList;
    void showProduct(bool getFromDB = false);
    sqlite *db;
    vector<vector<double>> discount;
    QTranslator *translatorPtr;

private:
    Ui::product *ui;
};

#endif // PRODUCT_H
