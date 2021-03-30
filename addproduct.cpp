#include "addproduct.h"
#include "ui_addproduct.h"

addProduct::addProduct(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::addProduct)
{
    ui->setupUi(this);
}

addProduct::~addProduct()
{
    delete ui;
}
