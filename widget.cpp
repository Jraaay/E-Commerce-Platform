#include "widget.h"
#include "ui_widget.h"
#include <QtDebug>
#include "product.h"

Widget::Widget(QTranslator *translator, QWidget *parent)
    : QWidget(parent), ui(new Ui::Widget)
{
    ui->setupUi(this);
    translatorPtr = translator;
    ui->passwordAgain->setHidden(true);
    init();
    connect(ui->login, &QPushButton::clicked, this, &Widget::loginRegFun);
    connect(ui->reg, &QPushButton::clicked, this, &Widget::loginRegSwitchFun);
    connect(ui->seller, &QPushButton::clicked, this, &Widget::setTypeSeller);
    connect(ui->consumer, &QPushButton::clicked, this, &Widget::setTypeConsumer);
    connect(ui->password, &QLineEdit::returnPressed, ui->login, &QPushButton::click);
    connect(ui->passwordAgain, &QLineEdit::returnPressed, ui->login, &QPushButton::click);
    connect(ui->guestLogin, &QPushButton::clicked, this, &Widget::guestLogin);
    connect(ui->language, &QPushButton::clicked, this, &Widget::changeLang);
    connect(ui->userName, &QLineEdit::inputRejected, this, &Widget::invalidUsername);
    connect(ui->userName, &QLineEdit::textEdited, this, &Widget::validUsername);
}

Widget::~Widget()
{
    delete ui;
}

void Widget::showProduct()
{
    product *pro;
    if (curType == SELLERTYPE)
    {
        pro = new product(translatorPtr, &curSeller);
        pro->curType = curType;
        pro->show();
        this->close();
    }
    else
    {
        pro = new product(translatorPtr, &curConsumer);
        pro->curType = curType;
        pro->show();
        this->close();
    }
}

void Widget::setTypeSeller()
{
    ui->consumer->setStyleSheet("padding: -1;background-color: rgb(255,255,255);border:none;border-radius:30px;color:gray");
    ui->seller->setStyleSheet("padding: -1;background-color: rgb(255,255,255);border:none;border-radius:30px;color:black");
    ui->userName->setFocus();
    curType = SELLERTYPE;
}

void Widget::setTypeConsumer()
{
    ui->consumer->setStyleSheet("padding: -1;background-color: rgb(255,255,255);border:none;border-radius:30px;color:black");
    ui->seller->setStyleSheet("padding: -1;background-color: rgb(255,255,255);border:none;border-radius:30px;color:gray");
    ui->userName->setFocus();
    curType = CONSUMERTYPE;
}

void Widget::init()
{
    ui->warning->hide();
    QRegExp regx1("^[a-zA-Z0-9_\\-]{0,16}$");
    QValidator *validator1 = new QRegExpValidator(regx1, ui->userName);
    ui->userName->setValidator(validator1);
    curType = CONSUMERTYPE;
    ifstream infile;
    string sellerJson = "";
    try
    {
        infile.open("sellerFile.json");
        infile >> sellerJson;
        infile.close();
    }
    catch (exception& e)
    {
        qDebug() << e.what() << endl;
    }
    if (sellerJson == "")
    {
        vector<string> tmp;
        json j;
        j["data"] = tmp;
        ofstream outFile;
        outFile.open("sellerFile.json");
        outFile << j.dump();
        outFile.close();
    }
    string consumerJson = "";
    try{
        infile.open("consumerFile.json");
        infile >> consumerJson;
        infile.close();
    }
    catch (exception& e)
    {
        qDebug() << e.what() << endl;
    }

    if (consumerJson == "")
    {
        vector<string> tmp;
        json j;
        j["data"] = tmp;
        ofstream outFile;
        outFile.open("consumerFile.json");
        outFile << j.dump();
        outFile.close();
    }
    string uidMaxJson = "";
    try
    {
        infile.open("uidMaxFile.json");
        infile >> uidMaxJson;
        infile.close();
    }
    catch (exception &e)
    {
        qDebug() << e.what() << endl;
    }

    if (uidMaxJson == "")
    {
        int tmp = 0;
        json j;
        j["uid"] = tmp;
        ofstream outFile;
        outFile.open("uidMaxFile.json");
        outFile << j.dump();
        outFile.close();
    }
    ui->userName->setFocus();
}

void Widget::changeLang()
{
    if (ui->language->text() == "切换为中文")
    {
        translatorPtr->load(":/zh_cn.qm");
        qApp->installTranslator(translatorPtr);
    }
    else
    {
        translatorPtr->load(":/en_us.qm");
        qApp->installTranslator(translatorPtr);
    }
    ui->retranslateUi(this);
}

void Widget::invalidUsername()
{
    ui->warning->show();
}

void Widget::validUsername()
{
    ui->warning->hide();
}
