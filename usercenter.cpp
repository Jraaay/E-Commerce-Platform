#include "usercenter.h"
#include "ui_usercenter.h"

userCenter::userCenter(userClass *curUserFromWidget, product *father, QWidget *parent) : QWidget(parent),
                                                                                         ui(new Ui::userCenter)
{
    fatherPtr = father; // 设置父亲指针方便调用
    ui->setupUi(this);
    curUser = curUserFromWidget;
    init();
}

userCenter::~userCenter()
{
    delete ui;
}

/* 初始化 */
void userCenter::init()
{
    ui->orderListWidget->verticalScrollBar()->setStyleSheet("QScrollBar:vertical { width:8px; background:rgba(0,0,0,0%); margin:0px,0px,0px,0px; padding-top:9px; padding-bottom:9px; } QScrollBar::handle:vertical { width:8px; background:rgba(0,0,0,25%);  border-radius:4px; min-height:20; } QScrollBar::handle:vertical:hover { width:8px; background:rgba(0,0,0,50%);  border-radius:4px; min-height:20; } QScrollBar::add-line:vertical { height:9px;width:8px; border-image:url(:/images/a/3.png); subcontrol-position:bottom; } QScrollBar::sub-line:vertical { height:9px;width:8px; border-image:url(:/images/a/1.png); subcontrol-position:top; } QScrollBar::add-line:vertical:hover { height:9px;width:8px; border-image:url(:/images/a/4.png); subcontrol-position:bottom; } QScrollBar::sub-line:vertical:hover { height:9px;width:8px; border-image:url(:/images/a/2.png); subcontrol-position:top; } QScrollBar::add-page:vertical,QScrollBar::sub-page:vertical { background:rgba(0,0,0,10%); border-radius:4px; }");
    ui->orderListWidget->verticalScrollBar()->setSingleStep(10);
    ui->orderListWidget->setVerticalScrollMode(QListWidget::ScrollPerPixel);
    ui->orderListWidget->verticalScrollBar()->setContextMenuPolicy(Qt::NoContextMenu);
    const QRegExp regx1("^[a-zA-Z0-9_\\-]{0,16}$");
    const QValidator *validator1 = new QRegExpValidator(regx1, ui->usernameEdit);
    ui->usernameEdit->setValidator(validator1); // 正则匹配
    sqlite *db = new sqlite();
    if (curUser->getUserType() == SELLERTYPE)
    {
        db->getDiscount(curUser->uid, fooddiscount, clothesdiscount, bookdiscount);

        // 设置样式和折扣
        ui->food->setTextMargins(5, 0, 0, 0);
        ui->clothes->setTextMargins(5, 0, 0, 0);
        ui->book->setTextMargins(5, 0, 0, 0);
        ui->food->setText(QString::number((1 - fooddiscount) * 100));
        ui->clothes->setText(QString::number((1 - clothesdiscount) * 100));
        ui->book->setText(QString::number((1 - bookdiscount) * 100));

        //设置正则匹配
        QRegExp *regx1 = new QRegExp("^(100|(([1-9]){1}[0-9]?|0{1})((\\.)([0-9]){1,2})?)$");
        QValidator *validator1 = new QRegExpValidator(*regx1, ui->food);
        ui->food->setValidator(validator1);
        validator1 = new QRegExpValidator(*regx1, ui->clothes);
        QValidator *validator2 = new QRegExpValidator(*regx1, ui->food);
        ui->clothes->setValidator(validator2);
        validator1 = new QRegExpValidator(*regx1, ui->book);
        QValidator *validator3 = new QRegExpValidator(*regx1, ui->food);
        ui->book->setValidator(validator3);
        delete regx1;
    }

    // 设置样式
    ui->tabWidget->setCurrentIndex(0);
    ui->tabWidget->tabBar()->hide();
    ui->title->setText(ui->title->text().replace("XXX", curUser->name.c_str()));
    ui->usernameEdit->hide();
    ui->password->hide();
    ui->passwordAgain->hide();
    ui->username->setText(curUser->name.c_str());
    ui->uid->setText(QString::number(curUser->uid));
    ui->orderListWidget->setFrameShape(QListWidget::NoFrame);
    void (userCenter::*slotFun)(QListWidgetItem *) = &userCenter::onListMailItemClicked;
    void (QListWidget::*signal)(QListWidgetItem *) = &QListWidget::itemClicked;
    connect(ui->orderListWidget, signal, this, slotFun);

    // 显示余额
    char priceText[1000] = "";
    sprintf(priceText, "%.2lf", curUser->balance);
    ui->balanceText->setText(priceText);
    if (curUser->getUserType() != SELLERTYPE)
    {
        ui->discount->hide();
    }
    if (curUser->getUserType() == GUESTTYPE)
    {
        ui->balance->hide();
    }
    // 链接信号与槽
    connect(ui->changeUsername, &QPushButton::clicked, this, &userCenter::changeUserName);
    connect(ui->changePassword, &QPushButton::clicked, this, &userCenter::changePassword);
    connect(ui->account, &QPushButton::clicked, this, &userCenter::toAccount);
    connect(ui->balance, &QPushButton::clicked, this, &userCenter::toBalancePage);
    connect(ui->discount, &QPushButton::clicked, this, &userCenter::toDiscuss);
    connect(ui->order, &QPushButton::clicked, this, &userCenter::toOrders);
    connect(ui->recharge, &QPushButton::clicked, this, &userCenter::recharge);
    connect(ui->reset, &QPushButton::clicked, this, &userCenter::resetDiscount);
    connect(ui->save, &QPushButton::clicked, this, &userCenter::saveDiscount);
}

/* 修改用户名 */
void userCenter::changeUserName()
{
    if (ui->changeUsername->text() == "修改")
    {
        ui->username->hide();
        ui->usernameEdit->setText(ui->username->text());
        ui->usernameEdit->show();
        ui->changeUsername->setText("保存");
    }
    else if (ui->changeUsername->text() == "Change")
    {
        ui->username->hide();
        ui->usernameEdit->setText(ui->username->text());
        ui->usernameEdit->show();
        ui->changeUsername->setText("Save");
    }
    else
    {
        int status = userManager::changeUserName(curUser->uid, ui->usernameEdit->text().toStdString());
        if (status == 0)
        {
            curUser->name = ui->usernameEdit->text().toStdString();
            promptBox *prompt = new promptBox(nullptr, "修改成功\nChange successfully");
            fatherPtr->refreshUser();
            prompt->show();
            ui->usernameEdit->hide();
            ui->username->setText(ui->usernameEdit->text());
            ui->username->show();
            if (ui->changeUsername->text() == "保存")
            {
                ui->changeUsername->setText("修改");
            }
            else
            {
                ui->changeUsername->setText("Change");
            }
        }
        else
        {
            promptBox *prompt = new promptBox(nullptr, "用户名已经存在\nUsername already exist");
            prompt->show();
            ui->usernameEdit->setText(ui->username->text());
        }
    }
}

/* 修改密码 */
void userCenter::changePassword()
{
    if (ui->changePassword->text() == "修改密码")
    {
        ui->password->setText("");
        ui->passwordAgain->setText("");
        ui->password->show();
        ui->passwordAgain->show();
        ui->changePassword->setText("保存");
    }
    else if (ui->changePassword->text() == "Change Password")
    {
        ui->password->setText("");
        ui->passwordAgain->setText("");
        ui->password->show();
        ui->passwordAgain->show();
        ui->changePassword->setText("Save");
    }
    else
    {
        if (ui->password->text() != ui->passwordAgain->text())
        {
            promptBox *prompt = new promptBox(nullptr, "两次密码不一致，修改失败\nTwo passwords are not the same");
            prompt->show();
        }
        else
        {
            userManager::changePassword(curUser->uid, QString(QCryptographicHash::hash(ui->password->text().toUtf8(), QCryptographicHash::Md5).toHex()).toStdString());
            promptBox *prompt = new promptBox(nullptr, "修改成功\nChange successfully");
            prompt->show();
        }
        ui->password->hide();
        ui->passwordAgain->hide();
        if (ui->changePassword->text() == "保存")
        {
            ui->changePassword->setText("修改密码");
        }
        else
        {
            ui->changePassword->setText("Change Password");
        }
    }
}

void userCenter::showOrders()
{
    sqlite db;

    vector<double> priceSum;
    vector<long long> time;
    vector<bool> paid;
    vector<bool> canceled;
    orderId.clear();
    db.getOrderList(curUser->uid, orderId, priceSum, time, paid, canceled);

    for (int i = 0; i < (int)itemList.size(); i++)
    {
        delete itemList[i];
        delete uiList[i];
    }
    itemList.clear();
    uiList.clear();
    ui->orderListWidget->clear();
    ui->orderListWidget->verticalScrollBar()->setSingleStep(16);

    for (int i = 0; i < (int)orderId.size(); i++) // 循环添加所有订单
    {
        QListWidgetItem *tmp = new QListWidgetItem();
        itemList.push_back(tmp);
        ui->orderListWidget->addItem(tmp);
        tmp->setSizeHint(QSize(626, 160));
        OrderItem *w = new OrderItem(ui->orderListWidget);
        uiList.push_back(w);
        w->ui->orderId->setText(QString::number(orderId[i]));
        char priceText[1000] = "";
        sprintf(priceText, "%.2lf", priceSum[i]);
        w->ui->price->setText(priceText);
        time_t t = time[i];
        tm* local = localtime(&t);
        char buf[128] = { 0 };
        strftime(buf, 64, "%Y-%m-%d %H:%M:%S", local);
        w->ui->time->setText(buf);
        if (!paid[i])
        {
            w->ui->paid->setIcon(QIcon(":/image/unpaid.png"));
        }
        if (canceled[i])
        {
            w->ui->paid->setIcon(QIcon(":/image/canceled.png"));
        }
        ui->orderListWidget->setItemWidget(tmp, w);
    }
}

/* 打开账户管理界面 */
void userCenter::toAccount() const
{
    ui->balance->setStyleSheet("background-color: rgb(255,255,255);border:none;border-radius:30px;padding: -1;color:gray");
    ui->discount->setStyleSheet("background-color: rgb(255,255,255);border:none;border-radius:30px;padding: -1;color:gray");
    ui->account->setStyleSheet("background-color: rgb(255,255,255);border:none;border-radius:30px;padding: -1;color:#1c87ff");
    ui->order->setStyleSheet("background-color: rgb(255,255,255);border:none;border-radius:30px;padding: -1;color:gray");
    ui->tabWidget->setCurrentIndex(0);
    userManager::getUser(curUser->uid, *curUser);
    ui->username->setText(curUser->name.c_str());
}

/* 打开余额界面 */
void userCenter::toBalancePage() const
{
    ui->account->setStyleSheet("background-color: rgb(255,255,255);border:none;border-radius:30px;padding: -1;color:gray");
    ui->discount->setStyleSheet("background-color: rgb(255,255,255);border:none;border-radius:30px;padding: -1;color:gray");
    ui->balance->setStyleSheet("background-color: rgb(255,255,255);border:none;border-radius:30px;padding: -1;color:#1c87ff");
    ui->order->setStyleSheet("background-color: rgb(255,255,255);border:none;border-radius:30px;padding: -1;color:gray");
    ui->tabWidget->setCurrentIndex(1);
    userManager::getUser(curUser->uid, *curUser);
    // 显示余额
    char priceText[1000] = "";
    sprintf(priceText, "%.2lf", curUser->balance);
    ui->balanceText->setText(priceText);
}

/* 打开折扣管理界面 */
void userCenter::toDiscuss() const
{
    ui->balance->setStyleSheet("background-color: rgb(255,255,255);border:none;border-radius:30px;padding: -1;color:gray");
    ui->account->setStyleSheet("background-color: rgb(255,255,255);border:none;border-radius:30px;padding: -1;color:gray");
    ui->discount->setStyleSheet("background-color: rgb(255,255,255);border:none;border-radius:30px;padding: -1;color:#1c87ff");
    ui->order->setStyleSheet("background-color: rgb(255,255,255);border:none;border-radius:30px;padding: -1;color:gray");
    ui->tabWidget->setCurrentIndex(2);
}

/* 打开我的订单界面 */
void userCenter::toOrders()
{
    ui->balance->setStyleSheet("background-color: rgb(255,255,255);border:none;border-radius:30px;padding: -1;color:gray");
    ui->account->setStyleSheet("background-color: rgb(255,255,255);border:none;border-radius:30px;padding: -1;color:gray");
    ui->discount->setStyleSheet("background-color: rgb(255,255,255);border:none;border-radius:30px;padding: -1;color:gray");
    ui->order->setStyleSheet("background-color: rgb(255,255,255);border:none;border-radius:30px;padding: -1;color:#1c87ff");
    ui->tabWidget->setCurrentIndex(3);
    showOrders();
}

/* 打开充值界面 */
void userCenter::recharge()
{
    rechargePage *tmp = new rechargePage(nullptr, this);
    tmp->show();
}

/* 确认充值 */
void userCenter::rechargeConfirm(double moneyToCharge)
{
    curUser->recharge(moneyToCharge);
    userManager::recharge(curUser->uid, moneyToCharge);
    promptBox *prompt = new promptBox(nullptr, "充值成功\nRecharge successfully");
    prompt->show();
    char priceText[1000] = "";
    sprintf(priceText, "%.2lf", curUser->balance);
    ui->balanceText->setText(priceText);
    moneyToCharge = 0;
}

/* 重置折扣 */
void userCenter::resetDiscount() const
{
    ui->food->setText(QString::number((1 - fooddiscount) * 100));
    ui->clothes->setText(QString::number((1 - clothesdiscount) * 100));
    ui->book->setText(QString::number((1 - bookdiscount) * 100));
}

/* 保存折扣 */
void userCenter::saveDiscount()
{
    sqlite *db = new sqlite();

    db->setDiscount(curUser->uid, 1 - ui->food->text().toDouble() * 0.01, 1 - ui->clothes->text().toDouble() * 0.01, 1 - ui->book->text().toDouble() * 0.01);

    delete db;
    promptBox *prompt = new promptBox(nullptr, "保存成功\nSave successfully");
    prompt->show();
    fatherPtr->refresh();
}

/* 点击订单 */
void userCenter::onListMailItemClicked(QListWidgetItem *item)
{
    int curItem = 0;
    while (curItem < ui->orderListWidget->count())
    {
        if (ui->orderListWidget->item(curItem) == item)
        {
            break;
        }
        else
        {
            curItem++;
        }
    }
    userClass *curUserToOrder;
    if (curUser->getUserType() == CONSUMERTYPE)
    {
        curUserToOrder = new consumerClass();
        curUserToOrder->uid = curUser->uid;
        curUserToOrder->name = curUser->name;
        curUserToOrder->type = curUser->type;
        curUserToOrder->balance = curUser->balance;
        curUserToOrder->setPass(curUser->getPass());
    }
    else
    {

        curUserToOrder = new sellerClass();
        curUserToOrder->uid = curUser->uid;
        curUserToOrder->name = curUser->name;
        curUserToOrder->type = curUser->type;
        curUserToOrder->balance = curUser->balance;
        curUserToOrder->setPass(curUser->getPass());
    }
    OrderDetail *od = new OrderDetail(curUserToOrder, orderId[curItem], this);
    od->show();

}
