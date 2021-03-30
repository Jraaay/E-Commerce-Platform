#include "product.h"
#include "ui_product.h"
#include <QtDebug>
#include <QDebug>
#include <QCheckBox>
#include "ui_productlistui.h"

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
    qDebug() << curUser->name.c_str();
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
    for (int i = 0; i < 10; i++)
    {
        vector<QImage> tmpImage;
        productItem tmp(to_string(i), to_string(i), i, i, tmpImage, -1, i);
        productList.push_back(tmp);
    }
    db->openDb();
    QString tableName = "productItem";
    if (!db->isTableExist(tableName))
    {
        db->createTable();
    }
    if (!db->isTableExist(tableName))
    {
        db->createTable();
    }
    vector<QImage> tmpImage;
    productItem tmp(to_string(1), to_string(1), 1, 1, tmpImage, -1, 1);
    db->singleInsertData(tmp);
    db->closeDb();
    showProduct();
}

void product::showProduct()
{
    ui->listWidget->verticalScrollBar()->setSingleStep(16);
    for (int i = 0; i < (int)productList.size(); i++)
    {
        QListWidgetItem *tmp = new QListWidgetItem();
        ui->listWidget->addItem(tmp);
        tmp->setSizeHint(QSize(626,160));
        productListUi *w = new productListUi(ui->listWidget);
//        w->ui->label_2->setText(to_string(i).c_str());
        ui->listWidget->setItemWidget(tmp,w);
    }
}

product::~product()
{
    delete ui;
}
