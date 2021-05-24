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

namespace Ui
{
    class addProduct;
}

using namespace std;

class addProduct : public QWidget
{
    Q_OBJECT

public:
    explicit addProduct(int seller, QWidget *parent = nullptr);
    explicit addProduct(productItem productToModify, QWidget *parent = nullptr);
    ~addProduct();
    void init(productItem productToModify = productItem());
    void selectPhotoFun();
    void photoShow() const;
    void nextPhoto();
    void prePhoto();
    void delPhoto();
    void saveProduct();
    void delProduct();
    void setMainPhoto(int mainPhotoNo);
    bool eventFilter(QObject *obj, QEvent *event); // 添加时间过滤器声明
    int mainPhoto;                                 // 主图片
    int curFirstPhoto;                             // 当前的第一张图片
    QFileDialog *selectFile;                       // 文件选择器
    vector<QByteArray> photosList;                 // 图片文件列表
    vector<QPixmap> photosImageList;               //图片列表
    void *father = nullptr;                        // 父亲指针
    int modifyId = -1;                             // 修改的商品id
    int sellerId = -1;                             // 商家id

private:
    Ui::addProduct *ui;
};

#endif // ADDPRODUCT_H
