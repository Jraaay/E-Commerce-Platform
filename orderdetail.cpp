#include "orderdetail.h"
#include "ui_orderdetail.h"

OrderDetail::OrderDetail(userClass *curUserFromFather, int orderId, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::OrderDetail)
{
    curUser = curUserFromFather;
    _orderId = orderId;
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
    db->openDb();
    productList.clear();
    numberList.clear();
    checkedList.clear();
    count.clear();
    price.clear();
    db->getOrder(_orderId, paied, time, curUser->uid, productList, count, price, priceSum);
    db->closeDb();
    char priceText[1000] = "";
    sprintf(priceText, "￥%.2lf", priceSum);
    ui->priceSum->setText(priceText);
    ui->orderId->setText(ui->orderId->text() + QString::number(_orderId));
    showProduct();
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
        db->openDb();
        for (int i = 0; i < (int)productList.size(); i++)
        {
            delete productList[i];
        }

        productList.clear();
        numberList.clear();
        checkedList.clear();
        count.clear();
        price.clear();
        db->getOrder(_orderId, paied, time, curUser->uid, productList, count, price, priceSum);
        db->closeDb();
    }
    ui->listWidget->clear();
    ui->listWidget->verticalScrollBar()->setSingleStep(16);
    const string typeList[4] = {"", "食物", "衣服", "书籍"};
    const string typeListEn[4] = {"", "Food", "Clothes", "Book"};

    ifstream infile;
    string sellerJson = "";
    vector<sellerClass> sellerList;

    try
    {
        infile.open("sellerFile.json");
        infile >> sellerJson;
        infile.close();
        const json j = json::parse(sellerJson);
        vector<string> userListJson = j["data"];
        for (int i = 0; i < (int)userListJson.size(); i++)
        {
            const json jTmp = json::parse(userListJson[i]);
            sellerClass tmp;
            tmp.uid = jTmp["uid"];
            tmp.name = jTmp["name"];
            tmp.type = jTmp["type"];
            tmp.balance = jTmp["balance"];
            tmp.setPass(jTmp["password"]);
            sellerList.push_back(tmp);
        }
    }
    catch (exception &e)
    {
        close();
        return;
    }

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
