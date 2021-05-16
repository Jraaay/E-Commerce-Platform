#include "widget.h"
#include "ui_widget.h"
#include <QtDebug>
#include "product.h"

Widget::Widget(QTranslator *translator, QWidget *parent)
    : QWidget(parent), ui(new Ui::Widget)
{
    ui->setupUi(this);                  // 初始化界面
    translatorPtr = translator;         // 加载翻译
    ui->passwordAgain->setHidden(true); // 隐藏注册时的第二次密码输入
    init();                             // 初始化
}

Widget::~Widget()
{
    delete ui; // 析构函数删除界面
}

/* 展示商品界面 */
void Widget::showProduct()
{
    product *pro;              // 新建商品展示页面
    if (curType == SELLERTYPE) // 根据类型进行传参初始化
    {
        pro = new product(translatorPtr, &curSeller);
        pro->show();
        this->close();
    }
    else
    {
        pro = new product(translatorPtr, &curConsumer);
        pro->show();
        this->close();
    }
}

/* 商家登录 */
void Widget::setTypeSeller()
{
    ui->consumer->setStyleSheet("padding: -1;background-color: rgb(255,255,255);border:none;border-radius:30px;color:gray");
    ui->seller->setStyleSheet("padding: -1;background-color: rgb(255,255,255);border:none;border-radius:30px;color:black");
    ui->userName->setFocus();
    curType = SELLERTYPE;
}

/* 消费者登录 */
void Widget::setTypeConsumer()
{
    ui->consumer->setStyleSheet("padding: -1;background-color: rgb(255,255,255);border:none;border-radius:30px;color:black");
    ui->seller->setStyleSheet("padding: -1;background-color: rgb(255,255,255);border:none;border-radius:30px;color:gray");
    ui->userName->setFocus();
    curType = CONSUMERTYPE;
}

/* 初始化 */
void Widget::init() // 初始化
{
    ui->warning->hide();                            // 隐藏用户名不合法提示
    const QRegExp regx1("^[a-zA-Z0-9_\\-]{0,16}$"); // 设置正则表达式
    const QValidator *validator1 = new QRegExpValidator(regx1, ui->userName);
    ui->userName->setValidator(validator1); // 设置正则匹配
    curType = CONSUMERTYPE;                 // 初始化类型为消费者
    ifstream infile;
    string sellerJson = "";
    try // 读取消费者、商家和最大uid文件
    {
        infile.open("sellerFile.json");
        infile >> sellerJson;
        infile.close();
    }
    catch (exception &e) // 错误读取处理
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
    try
    {
        infile.open("consumerFile.json");
        infile >> consumerJson;
        infile.close();
    }
    catch (exception &e) // 错误读取处理
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
    catch (exception &e) // 错误读取处理
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
    ui->userName->setFocus(); // 焦点设置到用户名方便用户输入
    // 进行信号与槽绑定
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

/* 更换界面语言 */
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

/* 不合法用户名提示 */
void Widget::invalidUsername()
{
    ui->warning->show();
}

/* 合法用户名关闭提示 */
void Widget::validUsername()
{
    ui->warning->hide();
}
