#include <widget.h>
#include "ui_widget.h"
#include <QDebug>
#include "loginandregister.h"
/* 登录与注册 */
void Widget::loginRegFun()
{
    const string loginName = ui->userName->text().toStdString();
    const string loginPassword = QString(QCryptographicHash::hash(ui->password->text().toUtf8(), QCryptographicHash::Md5).toHex()).toStdString();
    if (ui->login->text() == "登录" || ui->login->text() == "Sign in")
    {
        int loginStatus;
        qDebug() << "login now";
        QJsonObject curUserJson;
        loginStatus = userManager::loginCheck(curType, loginName, loginPassword, curUserJson);
        if (loginStatus == 0)
        {
            if (curType == SELLERTYPE)
            {
                curSeller = sellerClass(curUserJson);
            }
            else
            {
                curConsumer = consumerClass(curUserJson);
            }
            showProduct();
        }
        else if (loginStatus == PASSWORDWRONG)
        {
            promptBox *prompt = new promptBox(nullptr, "密码错误\nPassword wrong");
            prompt->show();
        }
        else
        {
            promptBox *prompt = new promptBox(nullptr, "未找到用户\nUser doesn't exist");
            prompt->show();
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
        int regStatus = userManager::createUser(curType, loginName, loginPassword);
        if (regStatus == 0)
        {
            promptBox *prompt = new promptBox(nullptr, "注册成功\nRegister successfully");
            prompt->show();
        }
        else
        {
            promptBox *prompt = new promptBox(nullptr, "用户已经存在\nUser exists");
            prompt->show();
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
