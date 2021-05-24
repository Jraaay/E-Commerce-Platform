#include "cart.h"
#include "ui_cart.h"

Cart::Cart(userClass *curUserFromWidget, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Cart)
{
    curUser = curUserFromWidget;
    init();
}

Cart::~Cart()
{
    for (int i = 0; i < (int)productList.size(); i++)
    {
        delete productList[i];
    }
    delete db;
    delete ui;
    delete curUser;
}

void Cart::init()
{
    ui->setupUi(this);
    connect(ui->nextPhoto, &QPushButton::clicked, this, &Cart::nextPhoto);
    connect(ui->prePhoto, &QPushButton::clicked, this, &Cart::prePhoto);
    connect(ui->mainPhoto, &QPushButton::clicked, this, &Cart::showBigPhoto);
    connect(ui->refresh, &QPushButton::clicked, this, &Cart::refresh);
    connect(ui->selectall, &QPushButton::clicked, this, &Cart::selectAll);
    connect(ui->buy, &QPushButton::clicked, this, &Cart::generateOrder);
    void (Cart::*slotFun)(QListWidgetItem *) = &Cart::onListMailItemClicked;
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
    ui->listWidget->verticalScrollBar()->setSingleStep(16);
    ui->place->setFixedWidth(626);
    db = new sqlite;

    productList.clear();
    numberList.clear();
    checkedList.clear();
    db->queryCart(curUser->uid, productList, numberList, checkedList);


    showProduct();
}

void Cart::showProduct(bool getFromDB)
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

        for (int i = 0; i < (int)productList.size(); i++)
        {
            delete productList[i];
        }
        productList.clear();
        numberList.clear();
        checkedList.clear();
        db->queryCart(curUser->uid, productList, numberList, checkedList);


    }
    ui->scrollArea->hide();
    ui->place->show();
    ui->listWidget->clear();
    ui->listWidget->verticalScrollBar()->setSingleStep(16);
    const string typeList[4] = {"", "食物", "衣服", "书籍"};
    const string typeListEn[4] = {"", "Food", "Clothes", "Book"};


    for (int i = 0; i < (int)productList.size(); i++) // 循环添加所有商品
    {
        QListWidgetItem *tmp = new QListWidgetItem();
        itemList.push_back(tmp);
        ui->listWidget->addItem(tmp);
        tmp->setSizeHint(QSize(626, 160));
        CartItemUi *w = new CartItemUi((void *)this, curUser->uid, productList[i]->id, ui->listWidget);
        uiList.push_back(w);
        w->setValidator();
        w->ui->name->setText(geteElidedText(w->ui->name->font(), productList[i]->name.c_str(), w->ui->name->width()));
        char priceText[1000] = "";
        sprintf(priceText, "%.2lf", productList[i]->getPrice());
        w->ui->price->setText(priceText);
        string remainText = w->ui->remain->text().toStdString() + to_string(productList[i]->remaining);
        w->ui->remain->setText(remainText.c_str());
        w->ui->number->setText(QString::number(numberList[i]));
        w->ui->buyCheck->setChecked(checkedList[i]);
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

        if (productList[i]->getPrice() != productList[i]->price)
        {
            char priceRawText[1000] = "";
            sprintf(priceRawText, "￥%.2lf", productList[i]->price);
            w->ui->priceRaw->setText(priceRawText);
        }
        else
        {
            w->ui->priceRaw->setText("");
        }


        string sellerText = w->ui->seller->text().toStdString() + productList[i]->sellerName;
        w->ui->seller->setText(sellerText.c_str());

        QImage img;
        if (productList[i]->photo.size() > 0)
        {
            img.loadFromData(productList[i]->photo[productList[i]->mainPhoto]);
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
    countPrice();
}

/* 超长字符串处理 */
QString Cart::geteElidedText(QFont font, QString str, int MaxWidth)
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
void Cart::onListMailItemClicked(QListWidgetItem *item)
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
    ui->name->setText(productList[curItem]->name.c_str());
    char priceText[1000] = "";
    sprintf(priceText, "%.2lf", productList[curItem]->getPrice());
    ui->price->setText(priceText);
    string remainText;
    if (ui->refresh->text() == "刷新")
    {
        remainText = "剩余: " + to_string(productList[curItem]->remaining);
    }
    else
    {
        remainText = "Stock: " + to_string(productList[curItem]->remaining);
    }
    ui->remain->setText(remainText.c_str());
    ui->description->setText(productList[curItem]->description.c_str());
    curFirstPhoto = 0;
    mainPhoto = productList[curItem]->mainPhoto;
    curProduct = curItem;
    showPhoto();
}

/* 展示图片 */
void Cart::showPhoto()
{

    const productItem itemToShow = *productList[curProduct];
    QImage img;
    if (itemToShow.photo.size() > 0)
    {
        img.loadFromData(itemToShow.photo[mainPhoto]);
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
            QImage img;
            img.loadFromData(itemToShow.photo[i + curFirstPhoto]);
            photo = QPixmap::fromImage(img);
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
bool Cart::eventFilter(QObject *obj, QEvent *event)
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
void Cart::setMainPhoto(int mainPhotoNo)
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
void Cart::prePhoto()
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
void Cart::nextPhoto()
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
void Cart::showBigPhoto()
{
    if (productList[curProduct]->photo.size() > 0)
    {
        QDialog *a = new QDialog;
        a->setWindowIcon(QIcon(":/image/logo.png"));
        a->setWindowTitle("查看图片 Review image");
        a->setWindowFlags(Qt::CustomizeWindowHint | Qt::WindowCloseButtonHint);
        const QPixmap pixmap;
        QImage img;
        img.loadFromData(productList[curProduct]->photo[mainPhoto]);
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

/* 刷新 */
void Cart::refresh()
{

    for (int i = 0; i < (int)productList.size(); i++)
    {
        delete productList[i];
    }
    productList.clear();
    numberList.clear();
    checkedList.clear();
    db->queryCart(curUser->uid, productList, numberList, checkedList);


    showProduct();
}

void Cart::countPrice()
{
    double price = 0;
    for (int i = 0; i < (int)uiList.size(); i++)
    {
        if (uiList[i]->ui->buyCheck->isChecked())
        {
            price += productList[i]->getPrice() * uiList[i]->ui->number->text().toInt();
        }
    }
    char priceText[1000] = "";
    sprintf(priceText, "￥%.2lf", price);
    ui->cartPrice->setText(priceText);
}

/* 全选 */
void Cart::selectAll()
{
    for (int i = 0; i < (int)uiList.size(); i++)
    {
        uiList[i]->ui->buyCheck->setChecked(true);
    }
}

void Cart::generateOrder()
{
    int orderId = db->generateOrder(curUser->uid);
    if (orderId == -1)
    {
        promptBox *prompt = new promptBox(nullptr, "商品余量不足\nThere is a shortage of goods");
        prompt->show();
        return;
    }
    else if (orderId == -2)
    {
        promptBox *prompt = new promptBox(nullptr, "没有可以购买的商品\nThere are no goods to buy");
        prompt->show();
        return;
    }


    refresh();
    userClass *curUserToOrder;
    if (curUser->getUserType() == CONSUMERTYPE)
    {
        curUserToOrder = new consumerClass();
        curUserToOrder->uid = curUser->uid;
        curUserToOrder->name = curUser->name;
        curUserToOrder->type = curUser->type;
        curUserToOrder->balance = curUser->balance;
        curUserToOrder->setPass(curUser->getPass());
    }
    else
    {

        curUserToOrder = new sellerClass();
        curUserToOrder->uid = curUser->uid;
        curUserToOrder->name = curUser->name;
        curUserToOrder->type = curUser->type;
        curUserToOrder->balance = curUser->balance;
        curUserToOrder->setPass(curUser->getPass());
    }
    OrderDetail *od = new OrderDetail(curUserToOrder, orderId);
    od->show();
}
