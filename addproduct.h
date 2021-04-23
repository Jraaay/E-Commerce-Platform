#ifndef ADDPRODUCT_H
#define ADDPRODUCT_H

#include <QWidget>
#include <QListView>
#include <QFileDialog>
#include <QMouseEvent>
#include "sqlite.h"
#include "productitem.h"
#include "product.h"
#include "promptbox.h"
#include "promptbox.h"


namespace Ui {
class addProduct;
}

using namespace  std;

class addProduct : public QWidget
{
    Q_OBJECT

public:
    explicit addProduct(int seller, QWidget *parent = nullptr);
    explicit addProduct(productItem productToModify, QWidget *parent = nullptr);
    ~addProduct();
    void init(productItem productToModify = productItem());
    void selectPhotoFun();
    void photoShow();
    void nextPhoto();
    void prePhoto();
    void delPhoto();
    void saveProduct();
    void delProduct();
    void setMainPhoto(int mainPhotoNo);
    QFileDialog *selectFile;
    vector<QString> photosList;
    vector<QPixmap> photosImageList;
    int mainPhoto;
    int curFirstPhoto;
    bool eventFilter(QObject *obj, QEvent *event);	// 添加时间过滤器声明
    void *father = nullptr;
    int modifyId = -1;
    int sellerId = -1;

private:
    Ui::addProduct *ui;
};

#endif // ADDPRODUCT_H
