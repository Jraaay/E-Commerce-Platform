#include "product.h"
#include "ui_product.h"
#include <QtDebug>
#include <QDebug>
#include <QCheckBox>
#include "ui_productlistui.h"
#include <stdio.h>


product::product(userClass *curUserFromWidget, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::product)
{
    curUser = curUserFromWidget;
    init();
}

void product::init()
{
    ui->setupUi(this);
    connect(ui->logout, &QPushButton::clicked, this, &product::logoutFun);
    connect(ui->addProduct, &QPushButton::clicked, this, &product::test);
    connect(ui->nextPhoto, &QPushButton::clicked, this, &product::nextPhoto);
    connect(ui->prePhoto, &QPushButton::clicked, this, &product::prePhoto);
    connect(ui->mainPhoto, &QPushButton::clicked, this, &product::showBigPhoto);
    connect(ui->search, &QLineEdit::textChanged, this, &product::search);
    connect(ui->defaultSort, &QPushButton::clicked, this, &product::defaultSort);
    connect(ui->priceAscend, &QPushButton::clicked, this, &product::priceAscendSort);
    connect(ui->priceDescend, &QPushButton::clicked, this, &product::priceDescendSort);
    void(product:: *slotFun)(QListWidgetItem*) = &product::onListMailItemClicked;
    void(QListWidget:: *signal)(QListWidgetItem*) = &QListWidget::itemClicked;
    connect(ui->listWidget, signal,this, slotFun);
    ui->image_1->installEventFilter(this);
    ui->image_2->installEventFilter(this);
    ui->image_3->installEventFilter(this);
    ui->image_4->installEventFilter(this);
    ui->image_5->installEventFilter(this);
    ui->scrollArea->verticalScrollBar()->setStyleSheet("QScrollBar:vertical"
                                                       "{"
                                                       "width:8px;"
                                                       "background:rgba(0,0,0,0%);"
                                                       "margin:0px,0px,0px,0px;"
                                                       "padding-top:9px;"
                                                       "padding-bottom:9px;"
                                                       "}"
                                                       "QScrollBar::handle:vertical"
                                                       "{"
                                                       "width:8px;"
                                                       "background:rgba(0,0,0,25%);"
                                                       " border-radius:4px;"
                                                       "min-height:20;"
                                                       "}"
                                                       "QScrollBar::handle:vertical:hover"
                                                       "{"
                                                       "width:8px;"
                                                       "background:rgba(0,0,0,50%);"
                                                       " border-radius:4px;"
                                                       "min-height:20;"
                                                       "}"
                                                       "QScrollBar::add-line:vertical"
                                                       "{"
                                                       "height:9px;width:8px;"
                                                       "border-image:url(:/images/a/3.png);"
                                                       "subcontrol-position:bottom;"
                                                       "}"
                                                       "QScrollBar::sub-line:vertical"
                                                       "{"
                                                       "height:9px;width:8px;"
                                                       "border-image:url(:/images/a/1.png);"
                                                       "subcontrol-position:top;"
                                                       "}"
                                                       "QScrollBar::add-line:vertical:hover"
                                                       "{"
                                                       "height:9px;width:8px;"
                                                       "border-image:url(:/images/a/4.png);"
                                                       "subcontrol-position:bottom;"
                                                       "}"
                                                       "QScrollBar::sub-line:vertical:hover"
                                                       "{"
                                                       "height:9px;width:8px;"
                                                       "border-image:url(:/images/a/2.png);"
                                                       "subcontrol-position:top;"
                                                       "}"
                                                       "QScrollBar::add-page:vertical,QScrollBar::sub-page:vertical"
                                                       "{"
                                                       "background:rgba(0,0,0,10%);"
                                                       "border-radius:4px;"
                                                       "}"
                                                       );
    ui->place->setFixedWidth(626);
    if (curUser->getUserType() == CONSUMERTYPE)
    {
        QImage img;
        img.load(":/image/consumer.png");
        ui->userTypeLabal->setPixmap(QPixmap::fromImage(img));
        ui->addProduct->setText("");
        ui->addProduct->setEnabled(false);
    }
    else if (curUser->getUserType() == GUESTTYPE)
    {
        QImage img;
        img.load(":/image/guest.png");
        ui->userTypeLabal->setPixmap(QPixmap::fromImage(img));
        ui->addProduct->setText("");
        ui->addProduct->setEnabled(false);
    }
    else
    {
        ui->addProduct->setEnabled(true);
        QImage img;
        img.load(":/image/seller.png");
        ui->userTypeLabal->setPixmap(QPixmap::fromImage(img));
        ui->userCenter->setText(curUser->name.c_str());
    }
    ui->userCenter->setText(curUser->name.c_str());
    db = new sqlite;
//    db->openDb();
//    productList = db->queryTable();
//    db->closeDb();
//    qDebug() << 1 << endl;
    db->openDb();
    productList = db->queryTable();
    db->closeDb();
    showProduct();
}

void product::logoutFun()
{
    Widget *w;
    w = new Widget;
    w->show();
    this->close();
}

void product::test()
{
//    for (int i = 0; i < 10; i++)
//    {
//        vector<QImage> tmpImage;
//        productItem tmp(to_string(i), to_string(i), i, i, tmpImage, -1, i);
//        productList.push_back(tmp);
//    }
//    db->openDb();
//    QString tableName = "productItem";
//    if (!db->isTableExist(tableName))
//    {
//        db->createTable();
//    }
//    vector<QImage> tmpImage;
//    productItem tmp(to_string(1), to_string(1), 1, 1, tmpImage, -1, 1);
//    db->singleInsertData(tmp);
//    db->closeDb();
//    showProduct();
    addProduct *ap;
    ap = new addProduct();
    ap->father = this;
    ap->show();
}

void product::showProduct()
{
    ui->scrollArea->hide();
    ui->place->show();
    ui->listWidget->clear();
    ui->listWidget->verticalScrollBar()->setSingleStep(16);
    for (int i = 0; i < (int)productList.size(); i++)
    {
        QListWidgetItem *tmp = new QListWidgetItem();
        ui->listWidget->addItem(tmp);
        tmp->setSizeHint(QSize(626,160));
        productListUi *w = new productListUi(ui->listWidget);
        w->ui->name->setText(geteElidedText(w->ui->name->font(), productList[i].name.c_str(), w->ui->name->width()));
        char priceText[] = "";
        sprintf(priceText, "%.2lf", productList[i].price);
        w->ui->price->setText(priceText);
        string remainText = "剩余：" + to_string(productList[i].remaining);
        w->ui->remain->setText(remainText.c_str());
        QImage img;
        if (productList[i].photo.size() > 0)
        {
            img.load(productList[i].photo[productList[i].mainPhoto]);
        }
        else
        {
            img.load(":/image/noImage.png");
        }
        QPixmap pixmap = QPixmap::fromImage(img);
        int width = 110;
        int height = 130;
        pixmap = pixmap.scaled(width*2, height*2, Qt::KeepAspectRatio, Qt::FastTransformation);  // 按比例缩放
        pixmap = pixmap.scaled(width, height, Qt::KeepAspectRatio, Qt::SmoothTransformation);  // 按比例缩放
        w->ui->mainPhoto->setPixmap(pixmap);
        ui->listWidget->setItemWidget(tmp,w);
    }
}

QString product::geteElidedText(QFont font, QString str, int MaxWidth)
{
    QFontMetrics fontWidth(font);
    int width = fontWidth.horizontalAdvance(str);  //计算字符串宽度
    if(width>=MaxWidth)  //当字符串宽度大于最大宽度时进行转换
    {
        str = fontWidth.elidedText(str,Qt::ElideRight,MaxWidth);  //右部显示省略号
    }
    return str;   //返回处理后的字符串
}

void product::onListMailItemClicked(QListWidgetItem* item)
{
    ui->place->hide();
    ui->scrollArea->show();
    int curItem = 0;
    while (curItem < ui->listWidget->count())
    {
        if (ui->listWidget->item(curItem) == item)
        {
            break;
        }
        else
        {
            curItem++;
        }
    }
    ui->name->setText(productList[curItem].name.c_str());
    char priceText[] = "";
    sprintf(priceText, "%.2lf", productList[curItem].price);
    ui->price->setText(priceText);
    string remainText = "剩余：" + to_string(productList[curItem].remaining);
    ui->remain->setText(remainText.c_str());
    ui->description->setText(productList[curItem].description.c_str());
    curFirstPhoto = 0;
    mainPhoto = productList[curItem].mainPhoto;
    curProduct = curItem;
    showPhoto();
}

void product::showPhoto()
{

    productItem itemToShow = productList[curProduct];
    QImage img;
    if (itemToShow.photo.size() > 0)
    {
        img.load(itemToShow.photo[mainPhoto]);
    }
    else
    {
        img.load(":/image/noImage.png");
    }
    QPixmap pixmap = QPixmap::fromImage(img);
    int width = 155;
    int height = 195;
    pixmap = pixmap.scaled(width*2, height*2, Qt::KeepAspectRatio, Qt::FastTransformation);  // 按比例缩放
    pixmap = pixmap.scaled(width, height, Qt::KeepAspectRatio, Qt::SmoothTransformation);  // 按比例缩放
    ui->mainPhoto->setIconSize(QSize(155, 195));
    ui->mainPhoto->setIcon(pixmap);
    QLabel *photoLabelList[5] = {ui->image_1, ui->image_2, ui->image_3, ui->image_4, ui->image_5};
    for (int i = 0; i < 5; i++)
    {
        QPixmap photo;
        if (i + curFirstPhoto < int(itemToShow.photo.size()))
        {
            photo = QPixmap::fromImage(QImage(itemToShow.photo[i + curFirstPhoto]));
            photoLabelList[i]->setScaledContents(false);
        }
        else
        {
            photo = QPixmap::fromImage(QImage(":/image/noImage.png"));
            photoLabelList[i]->setScaledContents(true);
        }
        int width = 60;
        int height = 75;
        photo = photo.scaled(width*2, height*2, Qt::KeepAspectRatio, Qt::FastTransformation);  // 按比例缩放
        photo = photo.scaled(width, height, Qt::KeepAspectRatio, Qt::SmoothTransformation);  // 按比例缩放
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

    if (mainPhoto > curFirstPhoto + 4)
    {
        ui->nextPhoto->setIcon(QIcon(":/image/right_blue.png"));
    }
    else
    {
        ui->nextPhoto->setIcon(QIcon(":/image/right.png"));
    }
}

bool product::eventFilter(QObject *obj, QEvent *event)
{
    if (obj == ui->image_1)//指定某个QLabel
    {
        if (event->type() == QEvent::MouseButtonPress) //鼠标点击
        {
            QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(event); // 事件转换
            if(mouseEvent->button() == Qt::LeftButton)
            {
                setMainPhoto(0);
                return true;
            }
            return false;
        }
    }
    else if (obj == ui->image_2)//指定某个QLabel
    {
        if (event->type() == QEvent::MouseButtonPress) //鼠标点击
        {
            QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(event); // 事件转换
            if(mouseEvent->button() == Qt::LeftButton)
            {
                setMainPhoto(1);
                return true;
            }
            return false;
        }
    }
    else if (obj == ui->image_3)//指定某个QLabel
    {
        if (event->type() == QEvent::MouseButtonPress) //鼠标点击
        {
            QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(event); // 事件转换
            if(mouseEvent->button() == Qt::LeftButton)
            {
                setMainPhoto(2);
                return true;
            }
            return false;
        }
    }
    else if (obj == ui->image_4)//指定某个QLabel
    {
        if (event->type() == QEvent::MouseButtonPress) //鼠标点击
        {
            QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(event); // 事件转换
            if(mouseEvent->button() == Qt::LeftButton)
            {
                setMainPhoto(3);
                return true;
            }
            return false;
        }
    }
    else if (obj == ui->image_5)//指定某个QLabel
    {
        if (event->type() == QEvent::MouseButtonPress) //鼠标点击
        {
            QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(event); // 事件转换
            if(mouseEvent->button() == Qt::LeftButton)
            {
                setMainPhoto(4);
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

void product::setMainPhoto(int mainPhotoNo)
{
    if (mainPhotoNo + curFirstPhoto < int(productList[curProduct].photo.size()))
    {
        mainPhoto = mainPhotoNo + curFirstPhoto;
    }
    else
    {
        promptBox *prompt = new promptBox(nullptr, "这里没有图片，不能设置为主图片哦");
        prompt->show();
    }
    showPhoto();
}


void product::prePhoto()
{
    if (curFirstPhoto > 0)
    {
        curFirstPhoto--;
        showPhoto();
    }
    else
    {
        promptBox *prompt = new promptBox(nullptr, "已经是第一张图片了");
        prompt->show();
    }
}

void product::nextPhoto()
{
    if (curFirstPhoto < int(productList[curProduct].photo.size()) - 5)
    {
        curFirstPhoto++;
        showPhoto();
    }
    else
    {
        promptBox *prompt = new promptBox(nullptr, "已经是最后一张图片了");
        prompt->show();
    }
}

void product::showBigPhoto()
{
    QDialog *a = new QDialog;
    a->setWindowIcon(QIcon(":/image/logo.png"));
    a->setWindowTitle("查看图片");
    a->setWindowFlags(Qt::CustomizeWindowHint | Qt::WindowCloseButtonHint);
    QPixmap pixmap;
    QImage img(productList[curProduct].photo[mainPhoto]);
    pixmap.fromImage(img);
    int width = img.width();
    int height = img.height();
    if (width > 1920)
    {
        height = (double)height / (double)width * 1920;
        width = 1920;
    }
    if (height > 1080)
    {
        width = (double)width / (double)height * 1920;
        height = 1920;
    }
    a->setMinimumSize(width, height);
    a->setMaximumSize(width, height);
    string stylesheet = "background-image:url(" + productList[curProduct].photo[mainPhoto].toStdString() + ");background-position: center;background-repeat: no-repeat;";
    a->setStyleSheet(stylesheet.c_str());
    a->show();
}

void product::search()
{
    db->openDb();
    productList.clear();
    productList = db->queryTable(ui->search->text().toStdString());
    db->closeDb();
    showProduct();
}

void product::defaultSort()
{
    db->openDb();
    productList.clear();
    productList = db->queryTable(ui->search->text().toStdString());
    db->closeDb();
    showProduct();
    ui->defaultSort->setStyleSheet("background-color: rgb(255,255,255);border:none;padding: -1;color:rgb(28, 135, 255)");
    ui->priceDescend->setStyleSheet("background-color: rgb(255,255,255);border:none;padding: -1;");
    ui->priceAscend->setStyleSheet("background-color: rgb(255,255,255);border:none;padding: -1;");
}

void product::priceDescendSort()
{
    db->openDb();
    productList.clear();
    productList = db->queryTable(ui->search->text().toStdString(), " ORDER BY `price` DESC");
    db->closeDb();
    showProduct();
    ui->priceDescend->setStyleSheet("background-color: rgb(255,255,255);border:none;padding: -1;color:rgb(28, 135, 255)");
    ui->defaultSort->setStyleSheet("background-color: rgb(255,255,255);border:none;padding: -1;");
    ui->priceAscend->setStyleSheet("background-color: rgb(255,255,255);border:none;padding: -1;");
}

void product::priceAscendSort()
{
    db->openDb();
    productList.clear();
    productList = db->queryTable(ui->search->text().toStdString(), " ORDER BY `price` ASC");
    db->closeDb();
    showProduct();
    ui->priceAscend->setStyleSheet("background-color: rgb(255,255,255);border:none;padding: -1;color:rgb(28, 135, 255)");
    ui->priceDescend->setStyleSheet("background-color: rgb(255,255,255);border:none;padding: -1;");
    ui->defaultSort->setStyleSheet("background-color: rgb(255,255,255);border:none;padding: -1;");
}

product::~product()
{
    delete ui;
}
