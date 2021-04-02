#include <widget.h>
#include "ui_widget.h"
#include <QDebug>
#include "loginandregister.h"

void Widget::loginRegFun()
{
    string loginName = ui->userName->text().toStdString();
    string loginPassword = QString(QCryptographicHash::hash(ui->password->text().toUtf8(), QCryptographicHash::Md5).toHex()).toStdString();
    vector<sellerClass> sellerList;
    vector<consumerClass> consumerList;
    if (curType == SELLERTYPE)
    {
        ifstream infile;
        infile.open("sellerFile.json");
        string sellerJson;
        infile >> sellerJson;
        infile.close();
        json j = json::parse(sellerJson);
        vector<string> userListJson = j["data"];
        for (int i = 0; i < (int)userListJson.size(); i++)
        {
            json jTmp = json::parse(userListJson[i]);
            sellerClass tmp;
            tmp.name = jTmp["name"];
            tmp.type = jTmp["type"];
            tmp.balance = jTmp["balance"];
            tmp.setPass(jTmp["password"]);
            sellerList.push_back(tmp);
        }
    }
    if (curType == CONSUMERTYPE)
    {
        ifstream infile;
        infile.open("consumerFile.json");
        string consumerJson;
        infile >> consumerJson;
        infile.close();
        json j = json::parse(consumerJson);
        vector<string> userListJson = j["data"];
        for (int i = 0; i < (int)userListJson.size(); i++)
        {
            json jTmp = json::parse(userListJson[i]);
            consumerClass tmp;
            tmp.name = jTmp["name"];
            tmp.type = jTmp["type"];
            tmp.balance = jTmp["balance"];
            tmp.setPass(jTmp["password"]);
            consumerList.push_back(tmp);
        }
    }
    if (ui->login->text() == "登录")
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
                        promptBox *prompt = new promptBox(nullptr, "密码错误");
                        prompt->show();
                    }
                    break;
                }
            }
            if (flag)
            {
                promptBox *prompt = new promptBox(nullptr, "未找到用户");
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
                        promptBox *prompt = new promptBox(nullptr, "密码错误");
                        prompt->show();
                    }
                    break;
                }
            }
            if (flag)
            {
                promptBox *prompt = new promptBox(nullptr, "未找到用户");
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
            promptBox *prompt = new promptBox(nullptr, "两次输入的密码不一致");
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
                promptBox *prompt = new promptBox(nullptr, "注册成功");
                prompt->show();
            }
            else
            {
                promptBox *prompt = new promptBox(nullptr, "用户已经存在");
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
                promptBox *prompt = new promptBox(nullptr, "注册成功");
                prompt->show();
            }
            else
            {
                promptBox *prompt = new promptBox(nullptr, "用户已经存在");
                prompt->show();
            }
        }
    }
}

void Widget::loginRegSwitchFun()
{
    if (ui->reg->text() == "注册")
    {
        ui->passwordAgain->setHidden(false);
        ui->login->setText("注册");
        ui->guestLogin->setHidden(true);
        ui->reg->setText("返回登录");
    }
    else
    {
        ui->passwordAgain->setHidden(true);
        ui->login->setText("登录");
        ui->guestLogin->setHidden(false);
        ui->reg->setText("注册");
    }
}


void Widget::guestLogin()
{
    curType = GUESTTYPE;
    consumerClass *guest = new consumerClass;
    guest->type = GUESTTYPE;
    guest->name = "游客";
    guest->balance = 0;
    curConsumer = *guest;
//    msgBox = new QMessageBox("Login",
//                             "使用游客身份登录成功",
//                             QMessageBox::Information,
//                             QMessageBox::Ok | QMessageBox::Default,
//                             QMessageBox::Cancel | QMessageBox::Escape,
//                             0);
//    msgBox->setWindowIcon(QIcon(":/image/logo.png"));
//    msgBox->show();
    showProduct();
}
