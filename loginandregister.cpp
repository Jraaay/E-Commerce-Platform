#include <widget.h>
#include "ui_widget.h"
#include <QDebug>
#include "loginandregister.h"
/* 登录与注册 */
void Widget::loginRegFun()
{
    const string loginName = ui->userName->text().toStdString();
    const string loginPassword = QString(QCryptographicHash::hash(ui->password->text().toUtf8(), QCryptographicHash::Md5).toHex()).toStdString();
    vector<sellerClass> sellerList;
    vector<consumerClass> consumerList;
    int uidMax = userManager::getMaxUid();


    if (curType == SELLERTYPE)
    {
        sellerList = userManager::getSellerList();
    }
    if (curType == CONSUMERTYPE)
    {
        consumerList = userManager::getConsumerList();
    }
    if (ui->login->text() == "登录" || ui->login->text() == "Sign in")
    {
        bool logined = false;
        qDebug() << "login now";
        bool flag = true;
        if (curType == SELLERTYPE)
        {
            for (int i = 0; i < (int)sellerList.size(); i++)
            {
                if (sellerList[i].name == loginName)
                {
                    flag = false;
                    if (sellerList[i].getPass() == loginPassword)
                    {
                        logined = true;
                        curSeller = sellerList[i];
                    }
                    else
                    {
                        promptBox *prompt = new promptBox(nullptr, "密码错误\nPassword wrong");
                        prompt->show();
                    }
                    break;
                }
            }
            if (flag)
            {
                promptBox *prompt = new promptBox(nullptr, "未找到用户\nUser doesn't exist");
                prompt->show();
            }
        }
        else
        {
            for (int i = 0; i < (int)consumerList.size(); i++)
            {
                if (consumerList[i].name == loginName)
                {
                    flag = false;
                    if (consumerList[i].getPass() == loginPassword)
                    {
                        curConsumer = consumerList[i];
                        logined = true;
                    }
                    else
                    {
                        promptBox *prompt = new promptBox(nullptr, "密码错误\nPassword wrong");
                        prompt->show();
                    }
                    break;
                }
            }
            if (flag)
            {
                promptBox *prompt = new promptBox(nullptr, "未找到用户\nUser doesn't exist");
                prompt->show();
            }
        }
        if (logined)
        {
            showProduct();
        }
    }
    else
    {
        qDebug() << "reg now";
        if (ui->password->text() != ui->passwordAgain->text())
        {
            promptBox *prompt = new promptBox(nullptr, "两次输入的密码不一致\nTwo passwords are not the same");
            prompt->show();
            return;
        }
        if (curType == SELLERTYPE)
        {
            bool only = true;
            for (int i = 0; i < (int)sellerList.size(); i++)
            {
                if (sellerList[i].name == loginName)
                {
                    only = false;
                    break;
                }
            }
            if (only)
            {
                sellerClass tmp;
                tmp.uid = uidMax + 1;
                uidMax++;
                tmp.name = loginName;
                tmp.setPass(loginPassword);
                tmp.type = SELLERTYPE;
                tmp.balance = 0;
                sellerList.push_back(tmp);
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
                promptBox *prompt = new promptBox(nullptr, "注册成功\nRegister successfully");
                prompt->show();
                json uidJson;
                uidJson["uid"] = uidMax;
                ofstream uidOutFile;
                uidOutFile.open("uidMaxFile.json");
                uidOutFile << uidJson.dump();
                uidOutFile.close();
                sqlite db;
                db.openDb();
                db.newDiscount(tmp.uid);
                db.closeDb();
            }
            else
            {
                promptBox *prompt = new promptBox(nullptr, "用户已经存在\nUser exists");
                prompt->show();
            }
        }
        else
        {
            bool only = true;
            for (int i = 0; i < (int)consumerList.size(); i++)
            {
                if (consumerList[i].name == loginName)
                {
                    only = false;
                    break;
                }
            }
            if (only)
            {
                consumerClass tmp;
                tmp.uid = uidMax + 1;
                uidMax++;
                tmp.name = loginName;
                tmp.setPass(loginPassword);
                tmp.type = CONSUMERTYPE;
                tmp.balance = 0;
                consumerList.push_back(tmp);
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
                promptBox *prompt = new promptBox(nullptr, "注册成功\nRegister successfully");
                prompt->show();
                json uidJson;
                uidJson["uid"] = uidMax;
                ofstream uidOutFile;
                uidOutFile.open("uidMaxFile.json");
                uidOutFile << uidJson.dump();
                uidOutFile.close();
            }
            else
            {
                promptBox *prompt = new promptBox(nullptr, "用户已经存在\nUser exists");
                prompt->show();
            }
        }
    }
}

/* 服用注册和登录按钮 */
void Widget::loginRegSwitchFun()
{
    if (ui->reg->text() == "注册")
    {
        ui->passwordAgain->setHidden(false);
        ui->login->setText("注册");
        ui->guestLogin->setHidden(true);
        ui->reg->setText("返回登录");
    }
    else if (ui->reg->text() == "Register")
    {
        ui->passwordAgain->setHidden(false);
        ui->login->setText("Register");
        ui->guestLogin->setHidden(true);
        ui->reg->setText("Back to login");
    }
    else if (ui->reg->text() == "返回登录")
    {
        ui->passwordAgain->setHidden(true);
        ui->login->setText("登录");
        ui->guestLogin->setHidden(false);
        ui->reg->setText("注册");
    }
    else
    {
        ui->passwordAgain->setHidden(true);
        ui->login->setText("Sign in");
        ui->guestLogin->setHidden(false);
        ui->reg->setText("Register");
    }
}

/* 游客登录 */
void Widget::guestLogin()
{
    curType = GUESTTYPE;
    consumerClass *guest = new consumerClass;
    guest->type = GUESTTYPE;
    guest->name = "Guest";
    guest->balance = 0;
    curConsumer = *guest;
    showProduct();
}
