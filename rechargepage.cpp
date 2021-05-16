#include "rechargepage.h"
#include "ui_rechargepage.h"

rechargePage::rechargePage(QWidget *parent, QObject *uc) : QWidget(parent),
                                                           ui(new Ui::rechargePage)
{
    parentuserCenter = (userCenter *)uc;
    ui->setupUi(this);
    init();
}

rechargePage::~rechargePage()
{
    delete ui;
}

/* 初始化 */
void rechargePage::init()
{
    connect(ui->buy6, &QPushButton::clicked, this, &rechargePage::buy6);
    connect(ui->buy30, &QPushButton::clicked, this, &rechargePage::buy30);
    connect(ui->buy98, &QPushButton::clicked, this, &rechargePage::buy98);
    connect(ui->buy198, &QPushButton::clicked, this, &rechargePage::buy198);
    connect(ui->buy328, &QPushButton::clicked, this, &rechargePage::buy328);
    connect(ui->buy648, &QPushButton::clicked, this, &rechargePage::buy648);
    connect(ui->otherMoney, &QLineEdit::textEdited, this, &rechargePage::buyOther);
    connect(ui->confirm, &QPushButton::clicked, this, &rechargePage::rechargeConfirm);
    const QRegExp regx1("^(([1-9]{1}\\d*)|([0]{1}))(\\.(\\d){0,2})?$");
    const QValidator *validator1 = new QRegExpValidator(regx1, ui->otherMoney);
    ui->otherMoney->setValidator(validator1);
}

void rechargePage::buy6()
{
    ui->buy6->setStyleSheet("background-color: rgb(255,255,255);border:2px dashed #1c87ff;border-radius:10px;padding: -1;color:gray");
    ui->buy30->setStyleSheet("background-color: rgb(255,255,255);border:2px dashed #D3D3D3;border-radius:10px;padding: -1;color:gray");
    ui->buy98->setStyleSheet("background-color: rgb(255,255,255);border:2px dashed #D3D3D3;border-radius:10px;padding: -1;color:gray");
    ui->buy198->setStyleSheet("background-color: rgb(255,255,255);border:2px dashed #D3D3D3;border-radius:10px;padding: -1;color:gray");
    ui->buy328->setStyleSheet("background-color: rgb(255,255,255);border:2px dashed #D3D3D3;border-radius:10px;padding: -1;color:gray");
    ui->buy648->setStyleSheet("background-color: rgb(255,255,255);border:2px dashed #D3D3D3;border-radius:10px;padding: -1;color:gray");
    ui->otherMoney->setStyleSheet("border:2px groove #D3D3D3;border-radius:20px;padding: -1;");
    ui->otherMoney->setText("");
    price = 6;
}

void rechargePage::buy30()
{
    ui->buy30->setStyleSheet("background-color: rgb(255,255,255);border:2px dashed #1c87ff;border-radius:10px;padding: -1;color:gray");
    ui->buy6->setStyleSheet("background-color: rgb(255,255,255);border:2px dashed #D3D3D3;border-radius:10px;padding: -1;color:gray");
    ui->buy98->setStyleSheet("background-color: rgb(255,255,255);border:2px dashed #D3D3D3;border-radius:10px;padding: -1;color:gray");
    ui->buy198->setStyleSheet("background-color: rgb(255,255,255);border:2px dashed #D3D3D3;border-radius:10px;padding: -1;color:gray");
    ui->buy328->setStyleSheet("background-color: rgb(255,255,255);border:2px dashed #D3D3D3;border-radius:10px;padding: -1;color:gray");
    ui->buy648->setStyleSheet("background-color: rgb(255,255,255);border:2px dashed #D3D3D3;border-radius:10px;padding: -1;color:gray");
    ui->otherMoney->setStyleSheet("border:2px groove #D3D3D3;border-radius:20px;padding: -1;");
    ui->otherMoney->setText("");
    price = 30;
}

void rechargePage::buy98()
{
    ui->buy98->setStyleSheet("background-color: rgb(255,255,255);border:2px dashed #1c87ff;border-radius:10px;padding: -1;color:gray");
    ui->buy30->setStyleSheet("background-color: rgb(255,255,255);border:2px dashed #D3D3D3;border-radius:10px;padding: -1;color:gray");
    ui->buy6->setStyleSheet("background-color: rgb(255,255,255);border:2px dashed #D3D3D3;border-radius:10px;padding: -1;color:gray");
    ui->buy198->setStyleSheet("background-color: rgb(255,255,255);border:2px dashed #D3D3D3;border-radius:10px;padding: -1;color:gray");
    ui->buy328->setStyleSheet("background-color: rgb(255,255,255);border:2px dashed #D3D3D3;border-radius:10px;padding: -1;color:gray");
    ui->buy648->setStyleSheet("background-color: rgb(255,255,255);border:2px dashed #D3D3D3;border-radius:10px;padding: -1;color:gray");
    ui->otherMoney->setStyleSheet("border:2px groove #D3D3D3;border-radius:20px;padding: -1;");
    ui->otherMoney->setText("");
    price = 98;
}

void rechargePage::buy198()
{
    ui->buy198->setStyleSheet("background-color: rgb(255,255,255);border:2px dashed #1c87ff;border-radius:10px;padding: -1;color:gray");
    ui->buy30->setStyleSheet("background-color: rgb(255,255,255);border:2px dashed #D3D3D3;border-radius:10px;padding: -1;color:gray");
    ui->buy98->setStyleSheet("background-color: rgb(255,255,255);border:2px dashed #D3D3D3;border-radius:10px;padding: -1;color:gray");
    ui->buy6->setStyleSheet("background-color: rgb(255,255,255);border:2px dashed #D3D3D3;border-radius:10px;padding: -1;color:gray");
    ui->buy328->setStyleSheet("background-color: rgb(255,255,255);border:2px dashed #D3D3D3;border-radius:10px;padding: -1;color:gray");
    ui->buy648->setStyleSheet("background-color: rgb(255,255,255);border:2px dashed #D3D3D3;border-radius:10px;padding: -1;color:gray");
    ui->otherMoney->setStyleSheet("border:2px groove #D3D3D3;border-radius:20px;padding: -1;");
    ui->otherMoney->setText("");
    price = 198;
}

void rechargePage::buy328()
{
    ui->buy328->setStyleSheet("background-color: rgb(255,255,255);border:2px dashed #1c87ff;border-radius:10px;padding: -1;color:gray");
    ui->buy30->setStyleSheet("background-color: rgb(255,255,255);border:2px dashed #D3D3D3;border-radius:10px;padding: -1;color:gray");
    ui->buy98->setStyleSheet("background-color: rgb(255,255,255);border:2px dashed #D3D3D3;border-radius:10px;padding: -1;color:gray");
    ui->buy198->setStyleSheet("background-color: rgb(255,255,255);border:2px dashed #D3D3D3;border-radius:10px;padding: -1;color:gray");
    ui->buy6->setStyleSheet("background-color: rgb(255,255,255);border:2px dashed #D3D3D3;border-radius:10px;padding: -1;color:gray");
    ui->buy648->setStyleSheet("background-color: rgb(255,255,255);border:2px dashed #D3D3D3;border-radius:10px;padding: -1;color:gray");
    ui->otherMoney->setStyleSheet("border:2px groove #D3D3D3;border-radius:20px;padding: -1;");
    ui->otherMoney->setText("");
    price = 328;
}

void rechargePage::buy648()
{
    ui->buy648->setStyleSheet("background-color: rgb(255,255,255);border:2px dashed #1c87ff;border-radius:10px;padding: -1;color:gray");
    ui->buy30->setStyleSheet("background-color: rgb(255,255,255);border:2px dashed #D3D3D3;border-radius:10px;padding: -1;color:gray");
    ui->buy98->setStyleSheet("background-color: rgb(255,255,255);border:2px dashed #D3D3D3;border-radius:10px;padding: -1;color:gray");
    ui->buy198->setStyleSheet("background-color: rgb(255,255,255);border:2px dashed #D3D3D3;border-radius:10px;padding: -1;color:gray");
    ui->buy328->setStyleSheet("background-color: rgb(255,255,255);border:2px dashed #D3D3D3;border-radius:10px;padding: -1;color:gray");
    ui->buy6->setStyleSheet("background-color: rgb(255,255,255);border:2px dashed #D3D3D3;border-radius:10px;padding: -1;color:gray");
    ui->otherMoney->setStyleSheet("border:2px groove #D3D3D3;border-radius:20px;padding: -1;");
    ui->otherMoney->setText("");
    price = 648;
}

void rechargePage::buyOther()
{
    ui->buy6->setStyleSheet("background-color: rgb(255,255,255);border:2px dashed #D3D3D3;border-radius:10px;padding: -1;color:gray");
    ui->buy30->setStyleSheet("background-color: rgb(255,255,255);border:2px dashed #D3D3D3;border-radius:10px;padding: -1;color:gray");
    ui->buy98->setStyleSheet("background-color: rgb(255,255,255);border:2px dashed #D3D3D3;border-radius:10px;padding: -1;color:gray");
    ui->buy198->setStyleSheet("background-color: rgb(255,255,255);border:2px dashed #D3D3D3;border-radius:10px;padding: -1;color:gray");
    ui->buy328->setStyleSheet("background-color: rgb(255,255,255);border:2px dashed #D3D3D3;border-radius:10px;padding: -1;color:gray");
    ui->buy648->setStyleSheet("background-color: rgb(255,255,255);border:2px dashed #D3D3D3;border-radius:10px;padding: -1;color:gray");
    ui->otherMoney->setStyleSheet("border:2px groove #1c87ff;border-radius:20px;padding: -1;");
    price = 0;
}

/* 确认充值 */
void rechargePage::rechargeConfirm()
{
    if (price == 0)
    {
        if (ui->otherMoney->text() != "")
        {
            price = ui->otherMoney->text().toDouble();
            ((userCenter *)parentuserCenter)->rechargeConfirm(price);
            close();
        }
        else
        {
            promptBox *prompt = new promptBox(nullptr, "没有选择充值金额！\nPlease select amount");
            prompt->show();
        }
    }
    else
    {
        ((userCenter *)parentuserCenter)->rechargeConfirm(price);
        close();
    }
}
