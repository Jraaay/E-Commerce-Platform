#include "addproduct.h"
#include "ui_addproduct.h"
#include <QDebug>

addProduct::addProduct(int seller, QWidget *parent) : QWidget(parent),
                                                      ui(new Ui::addProduct)
{
    ui->setupUi(this);
    sellerId = seller;
    init();
}

addProduct::addProduct(productItem productToModify, QWidget *parent) : QWidget(parent),
                                                                       ui(new Ui::addProduct)
{
    ui->setupUi(this);
    modifyId = productToModify.id;
    init(productToModify);
}

void addProduct::init(productItem productToModify)
{
    ui->name->setTextMargins(5, 0, 0, 0);
    ui->price->setTextMargins(5, 0, 0, 0);
    QRegExp regx1("^(([1-9]{1}\\d*)|([0]{1}))(\\.(\\d){0,2})?$");
    QValidator *validator1 = new QRegExpValidator(regx1, ui->price);
    ui->price->setValidator(validator1);
    ui->remain->setTextMargins(5, 0, 0, 0);
    QRegExp regx2("^\\d+$");
    QValidator *validator2 = new QRegExpValidator(regx2, ui->remain);
    ui->remain->setValidator(validator2);
    ui->description->setTextMargins(5, 5, 0, 0);
    connect(ui->cancel, &QPushButton::clicked, this, &QWidget::close);
    connect(ui->addPhoto, &QPushButton::clicked, this, &addProduct::selectPhotoFun);
    connect(ui->nextPhoto, &QPushButton::clicked, this, &addProduct::nextPhoto);
    connect(ui->prePhoto, &QPushButton::clicked, this, &addProduct::prePhoto);
    connect(ui->delImage_1, &QPushButton::clicked, this, &addProduct::delPhoto);
    connect(ui->delImage_2, &QPushButton::clicked, this, &addProduct::delPhoto);
    connect(ui->delImage_3, &QPushButton::clicked, this, &addProduct::delPhoto);
    connect(ui->delImage_4, &QPushButton::clicked, this, &addProduct::delPhoto);
    connect(ui->add, &QPushButton::clicked, this, &addProduct::saveProduct);
    connect(ui->deleteButton, &QPushButton::clicked, this, &addProduct::delProduct);
    ui->image_1->installEventFilter(this);
    ui->image_2->installEventFilter(this);
    ui->image_3->installEventFilter(this);
    ui->image_4->installEventFilter(this);
    curFirstPhoto = 0;
    mainPhoto = 0;
    if (modifyId != -1)
    {
        ui->name->setText(productToModify.name.c_str());
        ui->price->setText(QString::number(productToModify.price));
        ui->remain->setText(QString::number(productToModify.remaining));
        ui->type->setCurrentIndex(productToModify.type - 1);
        ui->description->setText(productToModify.description.c_str());
        mainPhoto = productToModify.mainPhoto;
        photosList = productToModify.photo;
        for (int i = 0; i < (int)photosList.size(); i++)
        {
            QImage img;
            img.load(photosList[i]);
            QPixmap pixmap = QPixmap::fromImage(img);
            int width = 80;
            int height = 100;
            pixmap = pixmap.scaled(width * 2, height * 2, Qt::KeepAspectRatio, Qt::FastTransformation); // 按比例缩放
            pixmap = pixmap.scaled(width, height, Qt::KeepAspectRatio, Qt::SmoothTransformation);       // 按比例缩放
            photosImageList.push_back(pixmap);
        }
        photoShow();
    }
    else
    {
        ui->deleteButton->hide();
        ui->deleteButton->setEnabled(false);
    }
}

void addProduct::selectPhotoFun()
{
    selectFile = new QFileDialog;
    selectFile->setOption(selectFile->DontUseNativeDialog, false);
    QStringList photosNameTmp = selectFile->getOpenFileNames(0, "添加图片 Add photos", "‪C:\\Users\\Jray\\Pictures\\123456\\", "Images (*.jpg *.jpeg *.png *.gif);; All Files (*.*)", 0, 0);
    for (int i = 0; i < photosNameTmp.size(); i++)
    {
        QString photoNameTmp = photosNameTmp[i];
        if (photoNameTmp != "")
        {
            photosList.push_back(photoNameTmp);
            QImage img;
            img.load(photoNameTmp);
            QPixmap pixmap = QPixmap::fromImage(img);
            int width = 80;
            int height = 100;
            pixmap = pixmap.scaled(width * 2, height * 2, Qt::KeepAspectRatio, Qt::FastTransformation); // 按比例缩放
            pixmap = pixmap.scaled(width, height, Qt::KeepAspectRatio, Qt::SmoothTransformation);       // 按比例缩放
            photosImageList.push_back(pixmap);
            photoShow();
        }
    }
    delete selectFile;
}

void addProduct::photoShow()
{
    for (int i = 0; i < 4; i++)
    {
        QPixmap photo = QPixmap::fromImage(QImage(":/image/noImage.png"));
        if (i + curFirstPhoto < int(photosList.size()))
        {
            photo = photosImageList[i + curFirstPhoto];
        }
        QLabel *photoLabelList[4] = {ui->image_1, ui->image_2, ui->image_3, ui->image_4};
        photoLabelList[i]->setPixmap(photo);
        if (i + curFirstPhoto == mainPhoto)
        {
            photoLabelList[i]->setStyleSheet("border:2px dashed #1c87ff;");
        }
        else
        {
            photoLabelList[i]->setStyleSheet("border:2px dashed #D3D3D3;");
        }
    }
    if (mainPhoto < curFirstPhoto)
    {
        ui->prePhoto->setIcon(QIcon(":/image/left_blue.png"));
    }
    else
    {
        ui->prePhoto->setIcon(QIcon(":/image/left.png"));
    }

    if (mainPhoto > curFirstPhoto + 3)
    {
        ui->nextPhoto->setIcon(QIcon(":/image/right_blue.png"));
    }
    else
    {
        ui->nextPhoto->setIcon(QIcon(":/image/right.png"));
    }
    if (mainPhoto >= 0 && mainPhoto < int(photosList.size()) && int(photosList.size()) > 0)
    {
        QImage img;
        img.load(photosList[mainPhoto]);
        QPixmap pixmap = QPixmap::fromImage(img);
        int width = 155;
        int height = 200;
        pixmap = pixmap.scaled(width * 2, height * 2, Qt::KeepAspectRatio, Qt::FastTransformation); // 按比例缩放
        pixmap = pixmap.scaled(width, height, Qt::KeepAspectRatio, Qt::SmoothTransformation);       // 按比例缩放
        ui->addPhoto->setIconSize(QSize(155, 195));
        ui->addPhoto->setIcon(pixmap);
    }
    else
    {
        ui->addPhoto->setIcon(QIcon());
    }
}

void addProduct::prePhoto()
{
    if (curFirstPhoto > 0)
    {
        curFirstPhoto--;
        photoShow();
    }
    else
    {
        promptBox *prompt = new promptBox(nullptr, "已经是第一张图片了\nAlready first photo");
        prompt->show();
    }
}

void addProduct::nextPhoto()
{
    if (curFirstPhoto < int(photosList.size()) - 4)
    {
        curFirstPhoto++;
        photoShow();
    }
    else
    {
        promptBox *prompt = new promptBox(nullptr, "已经是最后一张图片了\nAlready last photo");
        prompt->show();
    }
}

void addProduct::delPhoto()
{
    int delPhotoNo = 0;
    if (sender() == ui->delImage_2)
    {
        delPhotoNo = 1;
    }
    else if (sender() == ui->delImage_3)
    {
        delPhotoNo = 2;
    }
    else if (sender() == ui->delImage_4)
    {
        delPhotoNo = 3;
    }
    if (delPhotoNo + curFirstPhoto < int(photosList.size()))
    {
        if (delPhotoNo + curFirstPhoto == mainPhoto)
        {
            mainPhoto = 0;
        }
        else if (delPhotoNo + curFirstPhoto < mainPhoto)
        {
            mainPhoto--;
        }
        photosList.erase(photosList.begin() + delPhotoNo + curFirstPhoto);
        photosImageList.erase(photosImageList.begin() + delPhotoNo + curFirstPhoto);
        if (curFirstPhoto >= int(photosList.size()) - 4 && curFirstPhoto > 0)
        {
            curFirstPhoto--;
        }
        photoShow();
    }
    else
    {
        promptBox *prompt = new promptBox(nullptr, "这里没有图片，不能删除哦\nHere is not a photo");
        prompt->show();
    }
}

void addProduct::setMainPhoto(int mainPhotoNo)
{
    if (mainPhotoNo + curFirstPhoto < int(photosList.size()))
    {
        mainPhoto = mainPhotoNo + curFirstPhoto;
    }
    else
    {
        promptBox *prompt = new promptBox(nullptr, "这里没有图片，不能设置为主图片哦\nHere is not a photo");
        prompt->show();
    }
    photoShow();
}

bool addProduct::eventFilter(QObject *obj, QEvent *event)
{
    if (obj == ui->image_1) //指定某个QLabel
    {
        if (event->type() == QEvent::MouseButtonPress) //鼠标点击
        {
            QMouseEvent *mouseEvent = static_cast<QMouseEvent *>(event); // 事件转换
            if (mouseEvent->button() == Qt::LeftButton)
            {
                setMainPhoto(0);
                return true;
            }
            return false;
        }
    }
    else if (obj == ui->image_2) //指定某个QLabel
    {
        if (event->type() == QEvent::MouseButtonPress) //鼠标点击
        {
            QMouseEvent *mouseEvent = static_cast<QMouseEvent *>(event); // 事件转换
            if (mouseEvent->button() == Qt::LeftButton)
            {
                setMainPhoto(1);
                return true;
            }
            return false;
        }
    }
    else if (obj == ui->image_3) //指定某个QLabel
    {
        if (event->type() == QEvent::MouseButtonPress) //鼠标点击
        {
            QMouseEvent *mouseEvent = static_cast<QMouseEvent *>(event); // 事件转换
            if (mouseEvent->button() == Qt::LeftButton)
            {
                setMainPhoto(2);
                return true;
            }
            return false;
        }
    }
    else if (obj == ui->image_4) //指定某个QLabel
    {
        if (event->type() == QEvent::MouseButtonPress) //鼠标点击
        {
            QMouseEvent *mouseEvent = static_cast<QMouseEvent *>(event); // 事件转换
            if (mouseEvent->button() == Qt::LeftButton)
            {
                setMainPhoto(3);
                return true;
            }
            return false;
        }
    }
    else
    {
        // pass the event on to the parent class
        return QWidget::eventFilter(obj, event);
    }
    return false;
}

void addProduct::saveProduct()
{
    productItem productToSave;
    if (ui->name->text().toStdString() == "")
    {
        promptBox *prompt = new promptBox(nullptr, "没有填写产品名字呢\nPlease input name");
        prompt->show();
        return;
    }
    productToSave.name = ui->name->text().toStdString();
    if (ui->price->text().toStdString() == "")
    {
        promptBox *prompt = new promptBox(nullptr, "没有填写产品价格呢\nPlease input price");
        prompt->show();
        return;
    }
    productToSave.price = ui->price->text().toDouble();
    if (ui->remain->text().toStdString() == "")
    {
        promptBox *prompt = new promptBox(nullptr, "没有填写产品余量呢\nPlease input stock");
        prompt->show();
        return;
    }
    productToSave.remaining = ui->remain->text().toInt();
    productToSave.description = ui->description->text().toStdString();
    productToSave.mainPhoto = mainPhoto;
    if (ui->type->currentIndex() == -1)
    {
        promptBox *prompt = new promptBox(nullptr, "没有选择产品类型呢\nPlease select type");
        prompt->show();
        return;
    }
    productToSave.type = ui->type->currentIndex() + 1;
    productToSave.photo = photosList;
    productToSave.id = modifyId;
    productToSave.seller = sellerId;
    sqlite db;
    db.openDb();
    if (modifyId != -1)
    {

        db.modifyData(productToSave);
    }
    else
    {
        db.singleInsertData(productToSave);
    }
    db.closeDb();
    if (father != nullptr)
    {
        ((product *)father)->showProduct(true);
    }
    this->close();
}

void addProduct::delProduct()
{
    sqlite db;
    db.openDb();
    db.deleteData(modifyId);
    db.closeDb();
    if (father != nullptr)
    {
        ((product *)father)->showProduct(true);
    }
    this->close();
}

addProduct::~addProduct()
{
    delete ui;
}
