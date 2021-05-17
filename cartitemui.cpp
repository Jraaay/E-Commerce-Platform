#include "cartitemui.h"
#include "ui_cartitemui.h"

CartItemUi::CartItemUi(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CartItemUi)
{
    ui->setupUi(this);
}

CartItemUi::~CartItemUi()
{
    delete ui;
}
