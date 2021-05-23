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
                QVector<QString> sellerJsonList;
                for (int i = 0; i < (int)sellerList.size(); i++)
                {
                    sellerJsonList.push_back(sellerList[i].getJson().c_str());
                }
                QJsonArray array = QJsonArray::fromStringList(QStringList::fromVector(sellerJsonList));
                QJsonObject object;
                object.insert("data", array);
                QJsonDocument document;
                document.setObject(object);
                QByteArray byteArray = document.toJson(QJsonDocument::Compact);
                ofstream outFile;
                outFile.open("sellerFile.json");
                outFile << byteArray.toStdString();
                outFile.close();
                promptBox *prompt = new promptBox(nullptr, "注册成功\nRegister successfully");
                prompt->show();
                QJsonObject object2;
                object2.insert("uid", uidMax);
                QJsonDocument document2;
                document2.setObject(object2);
                QByteArray array2 = document.toJson(QJsonDocument::Compact);
                ofstream outFile2;
                outFile.open("uidMaxFile.json");
                outFile << array2.toStdString();
                outFile.close();
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
                QVector<QString> consumerJsonList;
                for (int i = 0; i < (int)consumerList.size(); i++)
                {
                    consumerJsonList.push_back(consumerList[i].getJson().c_str());
                }
                QJsonArray array = QJsonArray::fromStringList(QStringList::fromVector(consumerJsonList));
                QJsonObject object;
                object.insert("data", array);
                QJsonDocument document;
                document.setObject(object);
                QByteArray byteArray = document.toJson(QJsonDocument::Compact);
                ofstream outFile;
                outFile.open("consumerFile.json");
                outFile << byteArray.toStdString();
                outFile.close();
                promptBox *prompt = new promptBox(nullptr, "注册成功\nRegister successfully");
                prompt->show();

                QJsonObject object2;
                object2.insert("uid", uidMax);
                QJsonDocument document2;
                document2.setObject(object2);
                QByteArray array2 = document.toJson(QJsonDocument::Compact);
                ofstream outFile2;
                outFile.open("uidMaxFile.json");
                outFile << array2.toStdString();
                outFile.close();
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
void Widget::loginRegSwitchFun() const
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
