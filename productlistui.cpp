#include "productlistui.h"
#include "ui_productlistui.h"

productListUi::productListUi(QWidget *parent) : QWidget(parent),
                                                ui(new Ui::productListUi)
{
    ui->setupUi(this);
}

productListUi::~productListUi()
{
    delete ui;
}
