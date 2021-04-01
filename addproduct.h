#ifndef ADDPRODUCT_H
#define ADDPRODUCT_H

#include <QWidget>
#include <QListView>
#include <QFileDialog>
#include <QMouseEvent>
#include "sqlite.h"
#include "productitem.h"


namespace Ui {
class addProduct;
}

using namespace  std;

class addProduct : public QWidget
{
    Q_OBJECT

public:
    explicit addProduct(QWidget *parent = nullptr);
    ~addProduct();
    void selectPhotoFun();
    void photoShow();
    void nextPhoto();
    void prePhoto();
    void delPhoto();
    void saveProduct();
    void setMainPhoto(int mainPhotoNo);
    QFileDialog *selectFile;
    vector<QString> photosList;
    vector<QPixmap> photosImageList;
    int mainPhoto;
    int curFirstPhoto;
    bool eventFilter(QObject *obj, QEvent *event);	// 添加时间过滤器声明

private:
    Ui::addProduct *ui;
};

#endif // ADDPRODUCT_H
