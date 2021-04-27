#include "usercenter.h"
#include "ui_usercenter.h"

userCenter::userCenter(userClass *curUserFromWidget, product *father, QWidget *parent) : QWidget(parent),
                                                                        ui(new Ui::userCenter)
{
    fatherPtr = father;
    ui->setupUi(this);
    curUser = curUserFromWidget;
    init();
}

userCenter::~userCenter()
{
    delete ui;
}

void userCenter::init()
{
    sqlite *db = new sqlite();
    db->openDb();
    discount = db->getDiscount();
    db->closeDb();
    delete db;

    for (int i = 0; i < (int)discount.size(); i++)
    {
        if (discount[i][3] == curUser->uid)
        {
            discountPlace = i;
            break;
        }
    }
    ui->food->setTextMargins(5, 0, 0, 0);
    ui->clothes->setTextMargins(5, 0, 0, 0);
    ui->book->setTextMargins(5, 0, 0, 0);
    ui->food->setText(QString::number((1 - discount[discountPlace][0]) * 100));
    ui->clothes->setText(QString::number((1 - discount[discountPlace][1]) * 100));
    ui->book->setText(QString::number((1 - discount[discountPlace][2]) * 100));

    ui->tabWidget->setCurrentIndex(0);
    ui->tabWidget->tabBar()->hide();
    ui->title->setText(ui->title->text().replace("XXX", curUser->name.c_str()));
    ui->usernameEdit->hide();
    ui->password->hide();
    ui->passwordAgain->hide();
    ui->username->setText(curUser->name.c_str());
    ui->uid->setText(QString::number(curUser->uid));

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

    char priceText[1000] = "";
    sprintf(priceText, "%.2lf", curUser->balance);
    ui->balanceText->setText(priceText);
    if (curUser->getUserType() != SELLERTYPE)
    {
        ui->discount->hide();
    }
    connect(ui->changeUsername, &QPushButton::clicked, this, &userCenter::changeUserName);
    connect(ui->changePassword, &QPushButton::clicked, this, &userCenter::changePassword);
    connect(ui->account, &QPushButton::clicked, this, &userCenter::toAccount);
    connect(ui->balance, &QPushButton::clicked, this, &userCenter::toBalancePage);
    connect(ui->discount, &QPushButton::clicked, this, &userCenter::toDiscuss);
    connect(ui->recharge, &QPushButton::clicked, this, &userCenter::recharge);
    connect(ui->reset, &QPushButton::clicked, this, &userCenter::resetDiscount);
    connect(ui->save, &QPushButton::clicked, this, &userCenter::saveDiscount);
}

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
        if (curUser->getUserType() == SELLERTYPE)
        {
            ifstream infile;
            string sellerJson = "";
            vector<sellerClass> sellerList;
            try{
                infile.open("sellerFile.json");
                infile >> sellerJson;
                infile.close();
                json j = json::parse(sellerJson);
                vector<string> userListJson = j["data"];
                for (int i = 0; i < (int)userListJson.size(); i++)
                {
                    json jTmp = json::parse(userListJson[i]);
                    sellerClass tmp;
                    tmp.uid = jTmp["uid"];
                    tmp.name = jTmp["name"];
                    tmp.type = jTmp["type"];
                    tmp.balance = jTmp["balance"];
                    tmp.setPass(jTmp["password"]);
                    sellerList.push_back(tmp);
                }
            }
            catch (exception& e)
            {
                qDebug() << e.what() << endl;
                vector<string> tmp;
                json j;
                j["data"] = tmp;
                ofstream outFile;
                outFile.open("sellerFile.json");
                outFile << j.dump();
                outFile.close();
                close();
                return;
            }
            bool only = true;
            int numToChange;
            for (int i = 0; i < (int)sellerList.size(); i++)
            {
                if (sellerList[i].name == ui->usernameEdit->text().toStdString() && sellerList[i].name != curUser->name)
                {
                    only = false;
                    break;
                }
                if (sellerList[i].uid == curUser->uid)
                {
                    numToChange = i;
                }
            }
            if (only)
            {
                curUser->name = ui->usernameEdit->text().toStdString();
                sellerList[numToChange].name = ui->usernameEdit->text().toStdString();
                vector<string> sellerJsonList;
                for (int i = 0; i < (int)sellerList.size(); i++)
                {
                    sellerJsonList.push_back(sellerList[i].getJson());
                }
                json j;
                j["data"] = sellerJsonList;
                ofstream outFile;
                outFile.open("sellerFile.json");
                outFile << j.dump();
                outFile.close();
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
        else
        {
            ifstream infile;
            string consumerJson = "";
            vector<consumerClass> consumerList;
            try{
                infile.open("consumerFile.json");
                infile >> consumerJson;
                infile.close();
                json j = json::parse(consumerJson);
                vector<string> userListJson = j["data"];
                for (int i = 0; i < (int)userListJson.size(); i++)
                {
                    json jTmp = json::parse(userListJson[i]);
                    consumerClass tmp;
                    tmp.uid = jTmp["uid"];
                    tmp.name = jTmp["name"];
                    tmp.type = jTmp["type"];
                    tmp.balance = jTmp["balance"];
                    tmp.setPass(jTmp["password"]);
                    consumerList.push_back(tmp);
                }
            }
            catch (exception& e)
            {
                qDebug() << e.what() << endl;
                vector<string> tmp;
                json j;
                j["data"] = tmp;
                ofstream outFile;
                outFile.open("consumerFile.json");
                outFile << j.dump();
                outFile.close();
                close();
                return;
            }
            bool only = true;
            int numToChange;
            for (int i = 0; i < (int)consumerList.size(); i++)
            {
                if (consumerList[i].name == ui->usernameEdit->text().toStdString() && consumerList[i].name != curUser->name)
                {
                    only = false;
                    break;
                }
                if (consumerList[i].uid == curUser->uid)
                {
                    numToChange = i;
                }
            }
            if (only)
            {
                curUser->name = ui->usernameEdit->text().toStdString();
                consumerList[numToChange].name = ui->usernameEdit->text().toStdString();
                vector<string> consumerJsonList;
                for (int i = 0; i < (int)consumerList.size(); i++)
                {
                    consumerJsonList.push_back(consumerList[i].getJson());
                }
                json j;
                j["data"] = consumerJsonList;
                ofstream outFile;
                outFile.open("consumerFile.json");
                outFile << j.dump();
                outFile.close();
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
}

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
        if (curUser->getUserType() == SELLERTYPE)
        {
            ifstream infile;
            string sellerJson = "";
            vector<sellerClass> sellerList;
            try{
                infile.open("sellerFile.json");
                infile >> sellerJson;
                infile.close();
                json j = json::parse(sellerJson);
                vector<string> userListJson = j["data"];
                for (int i = 0; i < (int)userListJson.size(); i++)
                {
                    json jTmp = json::parse(userListJson[i]);
                    sellerClass tmp;
                    tmp.uid = jTmp["uid"];
                    tmp.name = jTmp["name"];
                    tmp.type = jTmp["type"];
                    tmp.balance = jTmp["balance"];
                    tmp.setPass(jTmp["password"]);
                    sellerList.push_back(tmp);
                }
            }
            catch (exception& e)
            {
                qDebug() << e.what() << endl;
                vector<string> tmp;
                json j;
                j["data"] = tmp;
                ofstream outFile;
                outFile.open("sellerFile.json");
                outFile << j.dump();
                outFile.close();
                close();
                return;
            }
            int numToChange = 0;
            for (int i = 0; i < (int)sellerList.size(); i++)
            {
                if (sellerList[i].uid == curUser->uid)
                {
                    numToChange = i;
                    break;
                }
            }
            if (ui->password->text() != ui->passwordAgain->text())
            {
                promptBox *prompt = new promptBox(nullptr, "两次密码不一致，修改失败\nTwo passwords are not the same");
                prompt->show();
            }
            else
            {
                sellerList[numToChange].setPass(QString(QCryptographicHash::hash(ui->password->text().toUtf8(), QCryptographicHash::Md5).toHex()).toStdString());
                vector<string> sellerJsonList;
                for (int i = 0; i < (int)sellerList.size(); i++)
                {
                    sellerJsonList.push_back(sellerList[i].getJson());
                }
                json jWrite;
                jWrite["data"] = sellerJsonList;
                ofstream outFile;
                outFile.open("sellerFile.json");
                outFile << jWrite.dump();
                outFile.close();
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
        else
        {
            ifstream infile;
            string consumerJson = "";
            vector<consumerClass> consumerList;
            try{
                infile.open("consumerFile.json");
                infile >> consumerJson;
                infile.close();
                json j = json::parse(consumerJson);
                vector<string> userListJson = j["data"];
                for (int i = 0; i < (int)userListJson.size(); i++)
                {
                    json jTmp = json::parse(userListJson[i]);
                    consumerClass tmp;
                    tmp.uid = jTmp["uid"];
                    tmp.name = jTmp["name"];
                    tmp.type = jTmp["type"];
                    tmp.balance = jTmp["balance"];
                    tmp.setPass(jTmp["password"]);
                    consumerList.push_back(tmp);
                }
            }
            catch (exception& e)
            {
                qDebug() << e.what() << endl;
                vector<string> tmp;
                json j;
                j["data"] = tmp;
                ofstream outFile;
                outFile.open("consumerFile.json");
                outFile << j.dump();
                outFile.close();
                close();
                return;
            }
            int numToChange = 0;
            for (int i = 0; i < (int)consumerList.size(); i++)
            {
                if (consumerList[i].uid == curUser->uid)
                {
                    numToChange = i;
                    break;
                }
            }
            if (ui->password->text() != ui->passwordAgain->text())
            {
                promptBox *prompt = new promptBox(nullptr, "两次密码不一致，修改失败\nTwo passwords are not the same");
                prompt->show();
            }
            else
            {
                consumerList[numToChange].setPass(QString(QCryptographicHash::hash(ui->password->text().toUtf8(), QCryptographicHash::Md5).toHex()).toStdString());
                vector<string> consumerJsonList;
                for (int i = 0; i < (int)consumerList.size(); i++)
                {
                    consumerJsonList.push_back(consumerList[i].getJson());
                }
                json j;
                j["data"] = consumerJsonList;
                ofstream outFile;
                outFile.open("consumerFile.json");
                outFile << j.dump();
                outFile.close();
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
}

void userCenter::toAccount()
{
    ui->balance->setStyleSheet("background-color: rgb(255,255,255);border:none;border-radius:30px;padding: -1;color:gray");
    ui->discount->setStyleSheet("background-color: rgb(255,255,255);border:none;border-radius:30px;padding: -1;color:gray");
    ui->account->setStyleSheet("background-color: rgb(255,255,255);border:none;border-radius:30px;padding: -1;color:#1c87ff");
    ui->tabWidget->setCurrentIndex(0);
}

void userCenter::toBalancePage()
{
    ui->account->setStyleSheet("background-color: rgb(255,255,255);border:none;border-radius:30px;padding: -1;color:gray");
    ui->discount->setStyleSheet("background-color: rgb(255,255,255);border:none;border-radius:30px;padding: -1;color:gray");
    ui->balance->setStyleSheet("background-color: rgb(255,255,255);border:none;border-radius:30px;padding: -1;color:#1c87ff");
    ui->tabWidget->setCurrentIndex(1);
}

void userCenter::toDiscuss()
{
    ui->balance->setStyleSheet("background-color: rgb(255,255,255);border:none;border-radius:30px;padding: -1;color:gray");
    ui->account->setStyleSheet("background-color: rgb(255,255,255);border:none;border-radius:30px;padding: -1;color:gray");
    ui->discount->setStyleSheet("background-color: rgb(255,255,255);border:none;border-radius:30px;padding: -1;color:#1c87ff");
    ui->tabWidget->setCurrentIndex(2);
}

void userCenter::recharge()
{
    rechargePage *tmp = new rechargePage(nullptr, this);
    tmp->show();
}

void userCenter::rechargeConfirm(double moneyToCharge)
{
    curUser->recharge(moneyToCharge);
    if (curUser->getUserType() == SELLERTYPE)
    {
        ifstream infile;
        string sellerJson = "";
        vector<sellerClass> sellerList;
        try{
            infile.open("sellerFile.json");
            infile >> sellerJson;
            infile.close();
            json j = json::parse(sellerJson);
            vector<string> userListJson = j["data"];
            for (int i = 0; i < (int)userListJson.size(); i++)
            {
                json jTmp = json::parse(userListJson[i]);
                sellerClass tmp;
                tmp.uid = jTmp["uid"];
                tmp.name = jTmp["name"];
                tmp.type = jTmp["type"];
                tmp.balance = jTmp["balance"];
                tmp.setPass(jTmp["password"]);
                sellerList.push_back(tmp);
            }
        }
        catch (exception& e)
        {
            qDebug() << e.what() << endl;
            vector<string> tmp;
            json j;
            j["data"] = tmp;
            ofstream outFile;
            outFile.open("sellerFile.json");
            outFile << j.dump();
            outFile.close();
            close();
            return;
        }
        int numToChange = 0;
        for (int i = 0; i < (int)sellerList.size(); i++)
        {
            if (sellerList[i].uid == curUser->uid)
            {
                numToChange = i;
                break;
            }
        }
        sellerList[numToChange].balance += moneyToCharge;
        vector<string> sellerJsonList;
        for (int i = 0; i < (int)sellerList.size(); i++)
        {
            sellerJsonList.push_back(sellerList[i].getJson());
        }
        json jWrite;
        jWrite["data"] = sellerJsonList;
        ofstream outFile;
        outFile.open("sellerFile.json");
        outFile << jWrite.dump();
        outFile.close();
        promptBox *prompt = new promptBox(nullptr, "充值成功\nRecharge successfully");
        prompt->show();
    }
    else
    {
        ifstream infile;
        string consumerJson = "";
        vector<consumerClass> consumerList;
        try{
            infile.open("consumerFile.json");
            infile >> consumerJson;
            infile.close();
            json j = json::parse(consumerJson);
            vector<string> userListJson = j["data"];
            for (int i = 0; i < (int)userListJson.size(); i++)
            {
                json jTmp = json::parse(userListJson[i]);
                consumerClass tmp;
                tmp.uid = jTmp["uid"];
                tmp.name = jTmp["name"];
                tmp.type = jTmp["type"];
                tmp.balance = jTmp["balance"];
                tmp.setPass(jTmp["password"]);
                consumerList.push_back(tmp);
            }
        }
        catch (exception& e)
        {
            qDebug() << e.what() << endl;
            vector<string> tmp;
            json j;
            j["data"] = tmp;
            ofstream outFile;
            outFile.open("consumerFile.json");
            outFile << j.dump();
            outFile.close();
            close();
            return;
        }
        int numToChange = 0;
        for (int i = 0; i < (int)consumerList.size(); i++)
        {
            if (consumerList[i].uid == curUser->uid)
            {
                numToChange = i;
                break;
            }
        }
        consumerList[numToChange].balance += moneyToCharge;
        vector<string> consumerJsonList;
        for (int i = 0; i < (int)consumerList.size(); i++)
        {
            consumerJsonList.push_back(consumerList[i].getJson());
        }
        json jWrite;
        jWrite["data"] = consumerJsonList;
        ofstream outFile;
        outFile.open("consumerFile.json");
        outFile << jWrite.dump();
        outFile.close();
        promptBox *prompt = new promptBox(nullptr, "充值成功\nRecharge successfully");
        prompt->show();
    }
    char priceText[1000] = "";
    sprintf(priceText, "%.2lf", curUser->balance);
    ui->balanceText->setText(priceText);
    moneyToCharge = 0;
}

void userCenter::resetDiscount()
{
    ui->food->setText(QString::number(1 - discount[discountPlace][0]));
    ui->clothes->setText(QString::number(1 - discount[discountPlace][1]));
    ui->book->setText(QString::number(1 - discount[discountPlace][2]));
}

void userCenter::saveDiscount()
{
    discount[discountPlace][0] = 1 - ui->food->text().toDouble() * 0.01;
    discount[discountPlace][1] = 1 - ui->clothes->text().toDouble() * 0.01;
    discount[discountPlace][2] = 1 - ui->book->text().toDouble() * 0.01;
    sqlite *db = new sqlite();
    db->openDb();
    db->setDiscount(discount);
    db->closeDb();
    delete db;
    promptBox *prompt = new promptBox(nullptr, "保存成功\nSave successfully");
    prompt->show();
}
