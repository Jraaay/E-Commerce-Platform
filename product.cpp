#include "product.h"
#include "ui_product.h"
#include <QtDebug>
#include <QDebug>
#include <QCheckBox>
#include "ui_productlistui.h"
#include <stdio.h>
#include "usercenter.h"

product::product(QTranslator *translator, userClass *curUserFromWidget, QWidget *parent) : QWidget(parent),
                                                                                           ui(new Ui::product)
{
    translatorPtr = translator;
    curUser = curUserFromWidget;
    init();
}

/* 初始化 */
void product::init()
{
    sortMethod = 0;
    ui->setupUi(this);
    connect(ui->logout, &QPushButton::clicked, this, &product::logoutFun);
    connect(ui->addProduct, &QPushButton::clicked, this, &product::openAddProduct);
    connect(ui->nextPhoto, &QPushButton::clicked, this, &product::nextPhoto);
    connect(ui->prePhoto, &QPushButton::clicked, this, &product::prePhoto);
    connect(ui->mainPhoto, &QPushButton::clicked, this, &product::showBigPhoto);
    connect(ui->search, &QLineEdit::textChanged, this, &product::search);
    connect(ui->defaultSort, &QPushButton::clicked, this, &product::defaultSort);
    connect(ui->priceAscend, &QPushButton::clicked, this, &product::priceAscendSort);
    connect(ui->priceDescend, &QPushButton::clicked, this, &product::priceDescendSort);
    connect(ui->userCenter, &QPushButton::clicked, this, &product::openUserCenter);
    connect(ui->purchase, &QPushButton::clicked, this, &product::purchase);
    connect(ui->manage, &QPushButton::clicked, this, &product::manage);
    connect(ui->refresh, &QPushButton::clicked, this, &product::refresh);
    connect(ui->cart, &QPushButton::clicked, this, &product::openCart);
    connect(ui->addToCart, &QPushButton::clicked, this, &product::addToCart);
    void (product::*slotFun)(QListWidgetItem *) = &product::onListMailItemClicked;
    void (QListWidget::*signal)(QListWidgetItem *) = &QListWidget::itemClicked;
    connect(ui->listWidget, signal, this, slotFun);
    ui->image_1->installEventFilter(this);
    ui->image_2->installEventFilter(this);
    ui->image_3->installEventFilter(this);
    ui->image_4->installEventFilter(this);
    ui->image_5->installEventFilter(this);
    ui->listWidget->verticalScrollBar()->setStyleSheet("QScrollBar:vertical { width:8px; background:rgba(0,0,0,0%); margin:0px,0px,0px,0px; padding-top:9px; padding-bottom:9px; } QScrollBar::handle:vertical { width:8px; background:rgba(0,0,0,25%);  border-radius:4px; min-height:20; } QScrollBar::handle:vertical:hover { width:8px; background:rgba(0,0,0,50%);  border-radius:4px; min-height:20; } QScrollBar::add-line:vertical { height:9px;width:8px; border-image:url(:/images/a/3.png); subcontrol-position:bottom; } QScrollBar::sub-line:vertical { height:9px;width:8px; border-image:url(:/images/a/1.png); subcontrol-position:top; } QScrollBar::add-line:vertical:hover { height:9px;width:8px; border-image:url(:/images/a/4.png); subcontrol-position:bottom; } QScrollBar::sub-line:vertical:hover { height:9px;width:8px; border-image:url(:/images/a/2.png); subcontrol-position:top; } QScrollBar::add-page:vertical,QScrollBar::sub-page:vertical { background:rgba(0,0,0,10%); border-radius:4px; }");
    ui->scrollArea->verticalScrollBar()->setStyleSheet("QScrollBar:vertical { width:8px; background:rgba(0,0,0,0%); margin:0px,0px,0px,0px; padding-top:9px; padding-bottom:9px; } QScrollBar::handle:vertical { width:8px; background:rgba(0,0,0,25%);  border-radius:4px; min-height:20; } QScrollBar::handle:vertical:hover { width:8px; background:rgba(0,0,0,50%);  border-radius:4px; min-height:20; } QScrollBar::add-line:vertical { height:9px;width:8px; border-image:url(:/images/a/3.png); subcontrol-position:bottom; } QScrollBar::sub-line:vertical { height:9px;width:8px; border-image:url(:/images/a/1.png); subcontrol-position:top; } QScrollBar::add-line:vertical:hover { height:9px;width:8px; border-image:url(:/images/a/4.png); subcontrol-position:bottom; } QScrollBar::sub-line:vertical:hover { height:9px;width:8px; border-image:url(:/images/a/2.png); subcontrol-position:top; } QScrollBar::add-page:vertical,QScrollBar::sub-page:vertical { background:rgba(0,0,0,10%); border-radius:4px; }");
    ui->listWidget->verticalScrollBar()->setContextMenuPolicy(Qt::NoContextMenu);
    ui->scrollArea->verticalScrollBar()->setContextMenuPolicy(Qt::NoContextMenu);
    ui->place->setFixedWidth(626);
    if (curUser->getUserType() == CONSUMERTYPE)
    {
        QImage img;
        img.load(":/image/consumer.png");
        ui->userTypeLabal->setPixmap(QPixmap::fromImage(img));
        ui->addProduct->setText("");
        ui->addProduct->setEnabled(false);
        ui->manage->setText("");
        ui->manage->setEnabled(false);
        ui->manage->hide();
    }
    else if (curUser->getUserType() == GUESTTYPE)
    {
        QImage img;
        img.load(":/image/guest.png");
        ui->userTypeLabal->setPixmap(QPixmap::fromImage(img));
        ui->addProduct->setText("");
        ui->addProduct->setEnabled(false);
        ui->manage->setText("");
        ui->manage->setEnabled(false);
        ui->manage->hide();
        ui->userCenter->setEnabled(false);
        ui->cart->hide();
        ui->addToCart->hide();
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
    db->openDb();
    productList = db->queryTable();
    discount = db->getDiscount();
    db->closeDb();
    showProduct();
}

/* 登出 */
void product::logoutFun()
{
    Widget *w;
    w = new Widget(translatorPtr);
    w->show();
    this->close();
}

/* 打开用户中心 */
void product::openUserCenter()
{
    userCenter *w;
    w = new userCenter(curUser, this);
    w->show();
}

/* 添加商品 */
void product::openAddProduct()
{
    addProduct *ap;
    ap = new addProduct(curUser->uid);
    ap->father = this;
    ap->show();
}

/* 展示商品 */
void product::showProduct(bool getFromDB)
{
    for (int i = 0; i < (int)itemList.size(); i++)
    {
        delete itemList[i];
        delete uiList[i];
    }
    itemList.clear();
    uiList.clear();
    if (getFromDB)
    {
        db->openDb();
        for (int i = 0; i < (int)productList.size(); i++)
        {
            delete productList[i];
        }
        productList.clear();
        productList = db->queryTable();
        discount = db->getDiscount();
        db->closeDb();
    }
    ui->scrollArea->hide();
    ui->place->show();
    ui->listWidget->clear();
    ui->listWidget->verticalScrollBar()->setSingleStep(16);
    const string typeList[4] = {"", "食物", "衣服", "书籍"};
    const string typeListEn[4] = {"", "Food", "Clothes", "Book"};

    vector<sellerClass> sellerList = userManager::getSellerList();

    for (int i = 0; i < (int)productList.size(); i++) // 循环添加所有商品
    {
        QListWidgetItem *tmp = new QListWidgetItem();
        itemList.push_back(tmp);
        ui->listWidget->addItem(tmp);
        tmp->setSizeHint(QSize(626, 160));
        productListUi *w = new productListUi(ui->listWidget);
        uiList.push_back(w);
        w->ui->name->setText(geteElidedText(w->ui->name->font(), productList[i]->name.c_str(), w->ui->name->width()));
        char priceText[1000] = "";
        sprintf(priceText, "%.2lf", productList[i]->getPrice(discount));
        w->ui->price->setText(priceText);
        string remainText = w->ui->remain->text().toStdString() + to_string(productList[i]->remaining);
        w->ui->remain->setText(remainText.c_str());
        string typeText;
        if (w->ui->type->text() == "类型：")
        {
            typeText = w->ui->type->text().toStdString() + typeList[productList[i]->type];
        }
        else
        {
            typeText = w->ui->type->text().toStdString() + typeListEn[productList[i]->type];
        }
        w->ui->type->setText(typeText.c_str());

        if (productList[i]->getPrice(discount) != productList[i]->price)
        {
            char priceRawText[1000] = "";
            sprintf(priceRawText, "￥%.2lf", productList[i]->price);
            w->ui->priceRaw->setText(priceRawText);
        }
        else
        {
            w->ui->priceRaw->setText("");
        }

        int numToShow;
        for (int j = 0; j < (int)sellerList.size(); j++)
        {
            if (sellerList[j].uid == productList[i]->seller)
            {
                numToShow = j;
            }
        }
        string sellerText = w->ui->seller->text().toStdString() + sellerList[numToShow].name;
        w->ui->seller->setText(sellerText.c_str());

        QImage img;
        if (productList[i]->photo.size() > 0)
        {
            img.load(productList[i]->photo[productList[i]->mainPhoto]);
        }
        else
        {
            img.load(":/image/noImage.png");
        }
        QPixmap pixmap = QPixmap::fromImage(img);
        int width = 110;
        int height = 130;
        pixmap = pixmap.scaled(width * 2, height * 2, Qt::KeepAspectRatio, Qt::FastTransformation); // 按比例缩放
        pixmap = pixmap.scaled(width, height, Qt::KeepAspectRatio, Qt::SmoothTransformation);       // 按比例缩放
        w->ui->mainPhoto->setPixmap(pixmap);
        ui->listWidget->setItemWidget(tmp, w);
    }
}

/* 超长字符串处理 */
QString product::geteElidedText(QFont font, QString str, int MaxWidth) const
{
    const QFontMetrics fontWidth(font);
    int width = fontWidth.horizontalAdvance(str); //计算字符串宽度
    if (width >= MaxWidth)                        //当字符串宽度大于最大宽度时进行转换
    {
        str = fontWidth.elidedText(str, Qt::ElideRight, MaxWidth); //右部显示省略号
    }
    return str; //返回处理后的字符串
}

/* 点击商品 */
void product::onListMailItemClicked(QListWidgetItem *item)
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
    if (productList[curItem]->seller == curUser->uid && curUser->getUserType() == SELLERTYPE)
    {
        ui->manage->setEnabled(true);
        ui->manage->show();
    }
    else
    {
        ui->manage->setEnabled(false);
        ui->manage->hide();
    }
    ui->name->setText(productList[curItem]->name.c_str());
    char priceText[1000] = "";
    sprintf(priceText, "%.2lf", productList[curItem]->getPrice(discount));
    ui->price->setText(priceText);
    string remainText;
    if (ui->purchase->text() == "Buy now")
    {
        remainText = "Stock: " + to_string(productList[curItem]->remaining);
    }
    else
    {
        remainText = "剩余: " + to_string(productList[curItem]->remaining);
    }
    ui->remain->setText(remainText.c_str());
    ui->description->setText(productList[curItem]->description.c_str());
    curFirstPhoto = 0;
    mainPhoto = productList[curItem]->mainPhoto;
    curProduct = curItem;
    showPhoto();
}

/* 展示图片 */
void product::showPhoto() const
{

    const productItem itemToShow = *productList[curProduct];
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
    const int width = 155;
    const int height = 195;
    pixmap = pixmap.scaled(width * 2, height * 2, Qt::KeepAspectRatio, Qt::FastTransformation); // 按比例缩放
    pixmap = pixmap.scaled(width, height, Qt::KeepAspectRatio, Qt::SmoothTransformation);       // 按比例缩放
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
        const int width = 60;
        const int height = 75;
        photo = photo.scaled(width * 2, height * 2, Qt::KeepAspectRatio, Qt::FastTransformation); // 按比例缩放
        photo = photo.scaled(width, height, Qt::KeepAspectRatio, Qt::SmoothTransformation);       // 按比例缩放
        photoLabelList[i]->setPixmap(photo);
        if (i + curFirstPhoto == mainPhoto)
        {
            photoLabelList[i]->setStyleSheet(".QLabel\n{\n	border:2px dashed #1c87ff;\n}");
        }
        else
        {
            photoLabelList[i]->setStyleSheet(".QLabel\n{\n	border:2px dashed #D3D3D3;\n}");
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

/* 处理点击事件 */
bool product::eventFilter(QObject *obj, QEvent *event)
{
    if (obj == ui->image_1) //指定某个QLabel
    {
        if (event->type() == QEvent::MouseButtonPress) //鼠标点击
        {
            const QMouseEvent *mouseEvent = static_cast<QMouseEvent *>(event); // 事件转换
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
            const QMouseEvent *mouseEvent = static_cast<QMouseEvent *>(event); // 事件转换
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
            const QMouseEvent *mouseEvent = static_cast<QMouseEvent *>(event); // 事件转换
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
            const QMouseEvent *mouseEvent = static_cast<QMouseEvent *>(event); // 事件转换
            if (mouseEvent->button() == Qt::LeftButton)
            {
                setMainPhoto(3);
                return true;
            }
            return false;
        }
    }
    else if (obj == ui->image_5) //指定某个QLabel
    {
        if (event->type() == QEvent::MouseButtonPress) //鼠标点击
        {
            const QMouseEvent *mouseEvent = static_cast<QMouseEvent *>(event); // 事件转换
            if (mouseEvent->button() == Qt::LeftButton)
            {
                setMainPhoto(4);
                return true;
            }
            return false;
        }
    }
    else
    {
        return QWidget::eventFilter(obj, event);
    }
    return false;
}

/* 设置主图片 */
void product::setMainPhoto(int mainPhotoNo)
{
    if (mainPhotoNo + curFirstPhoto < int(productList[curProduct]->photo.size()))
    {
        mainPhoto = mainPhotoNo + curFirstPhoto;
    }
    else
    {
        promptBox *prompt = new promptBox(nullptr, "这里没有图片，不能查看哦");
        prompt->show();
    }
    showPhoto();
}

/* 上一张 */
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

/* 下一张 */
void product::nextPhoto()
{
    if (curFirstPhoto < int(productList[curProduct]->photo.size()) - 5)
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

/* 展示大图 */
void product::showBigPhoto() const
{
    if (productList[curProduct]->photo.size() > 0)
    {
        QDialog *a = new QDialog;
        a->setWindowIcon(QIcon(":/image/logo.png"));
        a->setWindowTitle("查看图片 Review image");
        a->setWindowFlags(Qt::CustomizeWindowHint | Qt::WindowCloseButtonHint);
        const QPixmap pixmap;
        const QImage img(productList[curProduct]->photo[mainPhoto]);
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
        const string stylesheet = "background-image:url(" + productList[curProduct]->photo[mainPhoto].toStdString() + ");background-position: center;background-repeat: no-repeat;";
        a->setStyleSheet(stylesheet.c_str());
        a->show();
    }
}

/* 搜索商品 */
void product::search()
{
    db->openDb();
    for (int i = 0; i < (int)productList.size(); i++)
    {
        delete productList[i];
    }
    productList.clear();
    if (sortMethod == DEFAULT_SORT)
    {
        productList = db->queryTable(ui->search->text().toStdString());
        discount = db->getDiscount();
    }
    else if (sortMethod == PRICE_DESCEND_SORT)
    {
        productList = db->queryTable(ui->search->text().toStdString(), " ORDER BY `price` DESC");
        for (int i = 0; i < (int)productList.size() - 1; i++)
        {
            for (int j = 0; j < (int)productList.size() - 1 - i; j++)
            {
                if (productList[j + 1]->getPrice(discount) > productList[j]->getPrice(discount))
                {
                    productItem *tmp = productList[j + 1];
                    productList[j + 1] = productList[j];
                    productList[j] = tmp;
                }
            }
        }
        discount = db->getDiscount();
    }
    else
    {
        productList = db->queryTable(ui->search->text().toStdString(), " ORDER BY `price` ASC");
        for (int i = 0; i < (int)productList.size() - 1; i++)
        {
            for (int j = 0; j < (int)productList.size() - 1 - i; j++)
            {
                if (productList[j + 1]->getPrice(discount) < productList[j]->getPrice(discount))
                {
                    productItem *tmp = productList[j + 1];
                    productList[j + 1] = productList[j];
                    productList[j] = tmp;
                }
            }
        }
        discount = db->getDiscount();
    }
    db->closeDb();
    showProduct();
}

/* 默认排序 */
void product::defaultSort()
{
    sortMethod = DEFAULT_SORT;
    db->openDb();
    for (int i = 0; i < (int)productList.size(); i++)
    {
        delete productList[i];
    }
    productList.clear();
    productList = db->queryTable(ui->search->text().toStdString());
    discount = db->getDiscount();
    db->closeDb();
    showProduct();
    ui->defaultSort->setStyleSheet("background-color: rgb(255,255,255);border:none;padding: -1;color:rgb(28, 135, 255)");
    ui->priceDescend->setStyleSheet("background-color: rgb(255,255,255);border:none;padding: -1;");
    ui->priceAscend->setStyleSheet("background-color: rgb(255,255,255);border:none;padding: -1;");
}

/* 价格降序 */
void product::priceDescendSort()
{
    sortMethod = PRICE_DESCEND_SORT;
    db->openDb();
    for (int i = 0; i < (int)productList.size(); i++)
    {
        delete productList[i];
    }
    productList.clear();
    productList = db->queryTable(ui->search->text().toStdString(), " ORDER BY `price` DESC");
    discount = db->getDiscount();
    for (int i = 0; i < (int)productList.size() - 1; i++)
    {
        for (int j = 0; j < (int)productList.size() - 1 - i; j++)
        {
            if (productList[j + 1]->getPrice(discount) > productList[j]->getPrice(discount))
            {
                productItem *tmp = productList[j + 1];
                productList[j + 1] = productList[j];
                productList[j] = tmp;
            }
        }
    }
    db->closeDb();
    showProduct();
    ui->priceDescend->setStyleSheet("background-color: rgb(255,255,255);border:none;padding: -1;color:rgb(28, 135, 255)");
    ui->defaultSort->setStyleSheet("background-color: rgb(255,255,255);border:none;padding: -1;");
    ui->priceAscend->setStyleSheet("background-color: rgb(255,255,255);border:none;padding: -1;");
}

/* 价格升序 */
void product::priceAscendSort()
{
    sortMethod = PRICE_ASCEND_SORT;
    db->openDb();
    for (int i = 0; i < (int)productList.size(); i++)
    {
        delete productList[i];
    }
    productList.clear();
    productList = db->queryTable(ui->search->text().toStdString(), " ORDER BY `price` ASC");
    discount = db->getDiscount();
    for (int i = 0; i < (int)productList.size() - 1; i++)
    {
        for (int j = 0; j < (int)productList.size() - 1 - i; j++)
        {
            if (productList[j + 1]->getPrice(discount) < productList[j]->getPrice(discount))
            {
                productItem *tmp = productList[j + 1];
                productList[j + 1] = productList[j];
                productList[j] = tmp;
            }
        }
    }
    db->closeDb();
    showProduct();
    ui->priceAscend->setStyleSheet("background-color: rgb(255,255,255);border:none;padding: -1;color:rgb(28, 135, 255)");
    ui->priceDescend->setStyleSheet("background-color: rgb(255,255,255);border:none;padding: -1;");
    ui->defaultSort->setStyleSheet("background-color: rgb(255,255,255);border:none;padding: -1;");
}

/* 点击购买 */
void product::purchase()
{
    if (curUser->getUserType() == GUESTTYPE)
    {
        promptBox *prompt = new promptBox(nullptr, "游客模式无法购买\nGuest can't purchase");
        prompt->show();
        return;
    }
    db->openDb();
    const vector<productItem *> purchaseProductList = db->queryTable();
    discount = db->getDiscount();
    db->closeDb();
    int productToPurchase = 0;
    for (int i = 0; i < (int)purchaseProductList.size(); i++)
    {
        if (purchaseProductList[i]->id == productList[curProduct]->id)
        {
            productToPurchase = i;
            break;
        }
    }
    if (purchaseProductList[productToPurchase]->remaining > 0 && curUser->balance >= purchaseProductList[productToPurchase]->getPrice(discount))
    {
        purchaseProductList[productToPurchase]->remaining--;
        db->openDb();
        db->modifyData(*purchaseProductList[productToPurchase], 0);
        db->closeDb();

        vector<sellerClass> sellerList = userManager::getSellerList();
        int numToChange;
        for (int i = 0; i < (int)sellerList.size(); i++)
        {
            if (sellerList[i].uid == productList[i]->seller)
            {
                numToChange = i;
            }
        }
        sellerList[numToChange].balance += purchaseProductList[productToPurchase]->getPrice(discount);
        vector<string> sellerJsonList;
        for (int i = 0; i < (int)sellerList.size(); i++)
        {
            sellerJsonList.push_back(sellerList[i].getJson());
        }
        json jTmp;
        jTmp["data"] = sellerJsonList;
        ofstream outFile;
        outFile.open("sellerFile.json");
        outFile << jTmp.dump();
        outFile.close();

        vector<productItem> a;
        vector<int> b;
        vector<double> c;
        productItem tmp;
        tmp.id = purchaseProductList[productToPurchase]->id;
        a.push_back(tmp);
        b.push_back(1);
        c.push_back(purchaseProductList[productToPurchase]->getPrice(discount));
        db->openDb();
        int orderId = db->generateOrder(curUser->uid, a, b, c, purchaseProductList[productToPurchase]->getPrice(discount));
        db->payOrder(orderId);
        db->closeDb();

        if (curUser->getUserType() == SELLERTYPE)
        {
            vector<sellerClass> sellerList = userManager::getSellerList();

            int numToChange;
            for (int i = 0; i < (int)sellerList.size(); i++)
            {
                if (sellerList[i].uid == curUser->uid)
                {
                    numToChange = i;
                }
            }
            sellerList[numToChange].balance -= purchaseProductList[productToPurchase]->getPrice(discount);
            vector<string> sellerJsonList;
            for (int i = 0; i < (int)sellerList.size(); i++)
            {
                sellerJsonList.push_back(sellerList[i].getJson());
            }
            json jTmp;
            jTmp["data"] = sellerJsonList;
            ofstream outFile;
            outFile.open("sellerFile.json");
            outFile << jTmp.dump();
            outFile.close();
            promptBox *prompt = new promptBox(nullptr, "购买成功\nBuy successfully");
            prompt->show();
        }
        else
        {
            vector<consumerClass> consumerList = userManager::getConsumerList();

            int numToChange;
            for (int i = 0; i < (int)consumerList.size(); i++)
            {
                if (consumerList[i].uid == curUser->uid)
                {
                    numToChange = i;
                }
            }
            consumerList[numToChange].balance -= purchaseProductList[productToPurchase]->getPrice(discount);
            vector<string> consumerJsonList;
            for (int i = 0; i < (int)consumerList.size(); i++)
            {
                consumerJsonList.push_back(consumerList[i].getJson());
            }
            json jTmp;
            jTmp["data"] = consumerJsonList;
            ofstream outFile;
            outFile.open("consumerFile.json");
            outFile << jTmp.dump();
            outFile.close();
            promptBox *prompt = new promptBox(nullptr, "购买成功\nBuy successfully");
            prompt->show();
        }
    }
    else if (purchaseProductList[productToPurchase]->remaining <= 0)
    {
        promptBox *prompt = new promptBox(nullptr, "商品数量不足\nOut of stock");
        prompt->show();
    }
    else if (curUser->balance < purchaseProductList[productToPurchase]->getPrice(discount))
    {
        promptBox *prompt = new promptBox(nullptr, "余额不足\nLack of balance");
        prompt->show();
    }
    db->openDb();
    for (int i = 0; i < (int)productList.size(); i++)
    {
        delete productList[i];
    }
    productList.clear();
    if (sortMethod == DEFAULT_SORT)
    {
        productList = db->queryTable(ui->search->text().toStdString());
        discount = db->getDiscount();
    }
    else if (sortMethod == PRICE_DESCEND_SORT)
    {
        productList = db->queryTable(ui->search->text().toStdString(), " ORDER BY `price` DESC");
        for (int i = 0; i < (int)productList.size() - 1; i++)
        {
            for (int j = 0; j < (int)productList.size() - 1 - i; j++)
            {
                if (productList[j + 1]->getPrice(discount) > productList[j]->getPrice(discount))
                {
                    productItem *tmp = productList[j + 1];
                    productList[j + 1] = productList[j];
                    productList[j] = tmp;
                }
            }
        }
        discount = db->getDiscount();
    }
    else
    {
        productList = db->queryTable(ui->search->text().toStdString(), " ORDER BY `price` ASC");
        for (int i = 0; i < (int)productList.size() - 1; i++)
        {
            for (int j = 0; j < (int)productList.size() - 1 - i; j++)
            {
                if (productList[j + 1]->getPrice(discount) < productList[j]->getPrice(discount))
                {
                    productItem *tmp = productList[j + 1];
                    productList[j + 1] = productList[j];
                    productList[j] = tmp;
                }
            }
        }
        discount = db->getDiscount();
    }
    db->closeDb();
    showProduct();

    for (int i = 0; i < (int)purchaseProductList.size(); i++)
    {
        delete purchaseProductList[i];
    }
}

/* 刷新 */
void product::refresh()
{
    db->openDb();
    for (int i = 0; i < (int)productList.size(); i++)
    {
        delete productList[i];
    }
    productList.clear();
    if (sortMethod == DEFAULT_SORT)
    {
        productList = db->queryTable(ui->search->text().toStdString());
        discount = db->getDiscount();
    }
    else if (sortMethod == PRICE_DESCEND_SORT)
    {
        productList = db->queryTable(ui->search->text().toStdString(), " ORDER BY `price` DESC");
        discount = db->getDiscount();
        for (int i = 0; i < (int)productList.size() - 1; i++)
        {
            for (int j = 0; j < (int)productList.size() - 1 - i; j++)
            {
                if (productList[j + 1]->getPrice(discount) > productList[j]->getPrice(discount))
                {
                    productItem *tmp = productList[j + 1];
                    productList[j + 1] = productList[j];
                    productList[j] = tmp;
                }
            }
        }
    }
    else
    {
        productList = db->queryTable(ui->search->text().toStdString(), " ORDER BY `price` ASC");
        discount = db->getDiscount();
        for (int i = 0; i < (int)productList.size() - 1; i++)
        {
            for (int j = 0; j < (int)productList.size() - 1 - i; j++)
            {
                if (productList[j + 1]->getPrice(discount) < productList[j]->getPrice(discount))
                {
                    productItem *tmp = productList[j + 1];
                    productList[j + 1] = productList[j];
                    productList[j] = tmp;
                }
            }
        }
    }
    db->closeDb();
    showProduct();
}

/* 管理商品 */
void product::manage()
{
    addProduct *ap; // 复用添加商品界面
    ap = new addProduct(*productList[curProduct]);
    ap->father = this;
    ap->show();
}

product::~product()
{
    for (int i = 0; i < (int)productList.size(); i++)
    {
        delete productList[i];
    }
    delete db;
    delete ui;
    delete curUser;
}

/* 修改用户名后刷新 */
void product::refreshUser() const
{
    ui->userCenter->setText(curUser->name.c_str());
}

/* 打开购物车 */
void product::openCart()
{
    Cart *cartPage = new Cart(curUser);
    cartPage->show();
}

void product::addToCart()
{
    db->openDb();
    db->modifyItemInCart(productList[curProduct]->id, curUser->uid);
    db->closeDb();
    promptBox *pb = new promptBox(nullptr, "成功加入购物车\nSuccessfully add to cart");
    pb->show();
}
