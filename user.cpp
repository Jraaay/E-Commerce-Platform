#include "user.h"

userClass::userClass(QJsonObject data)
{
    uid = data.value("uid").toInt();
    name = data.value("name").toString().toStdString();
    balance = data.value("balance").toDouble();
    type = data.value("type").toInt();
    password = data.value("password").toString().toStdString();
}

sellerClass::sellerClass(QJsonObject data)
{
    uid = data.value("uid").toInt();
    name = data.value("name").toString().toStdString();
    balance = data.value("balance").toDouble();
    type = data.value("type").toInt();
    setPass(data.value("password").toString().toStdString());
}

consumerClass::consumerClass(QJsonObject data)
{
    uid = data.value("uid").toInt();
    name = data.value("name").toString().toStdString();
    balance = data.value("balance").toDouble();
    type = data.value("type").toInt();
    setPass(data.value("password").toString().toStdString());
}

QJsonObject userClass::getJson()
{
    QJsonObject object;
    object.insert("uid",this->uid);
    object.insert("name",this->name.c_str());
    object.insert("balance",this->balance);
    object.insert("type",this->type);
    object.insert("password",this->password.c_str());
    return object;
}
