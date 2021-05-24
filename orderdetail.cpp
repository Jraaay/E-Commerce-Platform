#include "orderdetail.h"
#include "ui_orderdetail.h"
#include "usercenter.h"

OrderDetail::OrderDetail(userClass *curUserFromFather, int orderId, void* father, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::OrderDetail)
{
    curUser = curUserFromFather;
    _orderId = orderId;
    _father = father;
    init();
}

OrderDetail::~OrderDetail()
{
    for (int i = 0; i < (int)productList.size(); i++)
    {
        delete productList[i];
    }
    delete db;
    delete ui;
    delete curUser;
}


void OrderDetail::init()
{
    ui->setupUi(this);
    db = new sqlite;
    ui->listWidget->verticalScrollBar()->setStyleSheet("QScrollBar:vertical { width:8px; background:rgba(0,0,0,0%); margin:0px,0px,0px,0px; padding-top:9px; padding-bottom:9px; } QScrollBar::handle:vertical { width:8px; background:rgba(0,0,0,25%);  border-radius:4px; min-height:20; } QScrollBar::handle:vertical:hover { width:8px; background:rgba(0,0,0,50%);  border-radius:4px; min-height:20; } QScrollBar::add-line:vertical { height:9px;width:8px; border-image:url(:/images/a/3.png); subcontrol-position:bottom; } QScrollBar::sub-line:vertical { height:9px;width:8px; border-image:url(:/images/a/1.png); subcontrol-position:top; } QScrollBar::add-line:vertical:hover { height:9px;width:8px; border-image:url(:/images/a/4.png); subcontrol-position:bottom; } QScrollBar::sub-line:vertical:hover { height:9px;width:8px; border-image:url(:/images/a/2.png); subcontrol-position:top; } QScrollBar::add-page:vertical,QScrollBar::sub-page:vertical { background:rgba(0,0,0,10%); border-radius:4px; }");
    ui->listWidget->verticalScrollBar()->setContextMenuPolicy(Qt::NoContextMenu);
    ui->listWidget->verticalScrollBar()->setSingleStep(16);
    productList.clear();
    numberList.clear();
    checkedList.clear();
    count.clear();
    price.clear();
    db->getOrder(_orderId, paid, time, curUser->uid, productList, count, price, priceSum);

    char priceText[1000] = "";
    sprintf(priceText, "￥%.2lf", priceSum);
    if (paid)
    {
        ui->buy->setText("已支付");
        ui->buy->setEnabled(false);
    }
    ui->priceSum->setText(priceText);
    ui->orderId->setText(ui->orderId->text() + QString::number(_orderId));
    showProduct();
    connect(ui->buy, &QPushButton::clicked, this, &OrderDetail::payForOrder);
}

void OrderDetail::showProduct(bool getFromDB)
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
        count.clear();
        price.clear();
        db->getOrder(_orderId, paid, time, curUser->uid, productList, count, price, priceSum);

    }
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
        productListUi *w = new productListUi(ui->listWidget);
        uiList.push_back(w);
        w->ui->name->setText(geteElidedText(w->ui->name->font(), productList[i]->name.c_str(), w->ui->name->width()));
        char priceText[1000] = "";
        sprintf(priceText, "%.2lf", price[i]);
        w->ui->price->setText(priceText);
        w->ui->label_3->setText("");
        if (ui->label->text() == "订单")
        {
            w->ui->remain->setText("数量：" + QString::number(count[i]));
        }
        else
        {
            w->ui->remain->setText("Count:" + QString::number(count[i]));
        }
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
        w->ui->priceRaw->setText("");

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
}

/* 超长字符串处理 */
QString OrderDetail::geteElidedText(QFont font, QString str, int MaxWidth)
{
    const QFontMetrics fontWidth(font);
    int width = fontWidth.horizontalAdvance(str); //计算字符串宽度
    if (width >= MaxWidth)                        //当字符串宽度大于最大宽度时进行转换
    {
        str = fontWidth.elidedText(str, Qt::ElideRight, MaxWidth); //右部显示省略号
    }
    return str; //返回处理后的字符串
}

/* 支付 */
void OrderDetail::payForOrder()
{
    sqlite db;
    int payStatus = db.payOrder(_orderId);
    if (payStatus == 0)
    {
        promptBox *prompt = new promptBox(nullptr, "购买成功\nBuy successfully");
        prompt->show();
        if (_father != nullptr)
        {
            ((userCenter *)_father)->showOrders();
        }
        close();
    }
    else
    {
        promptBox *prompt = new promptBox(nullptr, "余额不足\nLack of balance");
        prompt->show();
    }
}
