#include "cartitemui.h"
#include "ui_cartitemui.h"
#include <QDebug>
#include "cart.h"

CartItemUi::CartItemUi(void *father, int userId, int productId, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CartItemUi)
{
    ui->setupUi(this);
    connect(ui->plus, &QPushButton::clicked, this, &CartItemUi::plus);
    connect(ui->sub, &QPushButton::clicked, this, &CartItemUi::sub);
    connect(ui->deleteCart, &QPushButton::clicked, this, &CartItemUi::deleteCart);
    connect(ui->number, &QLineEdit::textChanged, this, &CartItemUi::numberChange);
    connect(ui->buyCheck, &QCheckBox::stateChanged, this, &CartItemUi::checkedChange);
    _userId = userId;
    _productId = productId;
    _father = father;
}

CartItemUi::~CartItemUi()
{
    delete ui;
}

void CartItemUi::plus()
{
    ui->number->setText(QString::number(ui->number->text().toInt() + 1));
}

void CartItemUi::sub()
{
    ui->number->setText(QString::number(ui->number->text().toInt() -1));
}

void CartItemUi::numberChange()
{
    int remain = ui->remain->text().replace("剩余：", "").replace("Stork:", "").toInt();
    if (ui->number->text().toInt() > remain)
    {
        ui->number->setText(QString::number(remain));
    }
    else if (ui->number->text().toInt() < 0)
    {
        ui->number->setText(QString::number(0));
    }
    sqlite db;

    db.modifyItemInCart(_productId, _userId, ui->number->text().toInt(), ui->buyCheck->checkState());

    if (ui->number->text().toInt() >= remain)
    {
        ui->plus->setEnabled(false);
    }
    else
    {
        ui->plus->setEnabled(true);
    }
    if (ui->number->text().toInt() > 0)
    {
        ui->sub->setEnabled(true);
    }
    else
    {
        ui->sub->setEnabled(false);
    }
    ((Cart *)_father)->countPrice();
}

void CartItemUi::setValidator()
{
    const QRegExp regx1("^[1-9]\\d*|0$");
    const QValidator *validator1 = new QRegExpValidator(regx1, ui->price);
    ui->number->setValidator(validator1);
}

void CartItemUi::deleteCart()
{
    sqlite db;

    db.deleteItemFromCart(_productId, _userId);

    ((Cart *)_father)->refresh();
}

void CartItemUi::checkedChange()
{
    sqlite db;

    db.modifyItemInCart(_productId, _userId, ui->number->text().toInt(), ui->buyCheck->checkState());

    ((Cart *)_father)->countPrice();
}
