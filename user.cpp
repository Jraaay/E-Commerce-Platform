#include "user.h"

string userClass::getJson()
{
    QJsonObject object;
    object.insert("uid",this->uid);
    object.insert("name",this->name.c_str());
    object.insert("balance",this->balance);
    object.insert("type",this->type);
    object.insert("password",this->password.c_str());
    QJsonDocument document;
    document.setObject(object);
    string jsonStr = document.toJson(QJsonDocument::Compact).toStdString();
    return jsonStr;
}
