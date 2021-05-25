#include "usermanager.h"

userManager::userManager()
{

}

void userManager::logout()
{
    ofstream outfile;
    outfile.open("key");
    outfile.close();
}

void userManager::changePassword(int userId, string password)
{
    TcpClient client;
    client.connectToServer();
    QJsonObject object;
    object.insert("type", USER_changePassword);
    QJsonObject data;
    data.insert("userId", userId);
    data.insert("password", password.c_str());
    object.insert("data", data);
    QJsonDocument document;
    ifstream infile;
    infile.open("key");
    string key;
    infile >> key;
    infile.close();
    object.insert("key", key.c_str());
    document.setObject(object);
    client.getData(document.toJson(QJsonDocument::Compact), 0, false);
    client.disconnectFromServer();
}

void userManager::recharge(int userId, double moneyToCharge)
{
    TcpClient client;
    client.connectToServer();
    QJsonObject object;
    object.insert("type", USER_recharge);
    QJsonObject data;
    data.insert("userId", userId);
    data.insert("moneyToCharge", moneyToCharge);
    object.insert("data", data);
    ifstream infile;
    infile.open("key");
    string key;
    infile >> key;
    infile.close();
    object.insert("key", key.c_str());
    QJsonDocument document;
    document.setObject(object);
    client.getData(document.toJson(QJsonDocument::Compact), 0, false);
    client.disconnectFromServer();
}

int userManager::createUser(int curType, string loginName, string loginPassword)
{
    int regStatus = -1;
    TcpClient client;
    client.connectToServer();
    QJsonObject object;
    object.insert("type", USER_createUser);
    QJsonObject data;
    data.insert("curType", curType);
    data.insert("loginName", loginName.c_str());
    data.insert("loginPassword", loginPassword.c_str());
    object.insert("data", data);
    ifstream infile;
    QJsonDocument document;
    document.setObject(object);
    QByteArray json = client.getData(document.toJson(QJsonDocument::Compact), 1000, false);

    QJsonParseError jsonError;
    QJsonArray productJsonList;
    QJsonArray numberJsonList;
    QJsonArray checkedJsonList;
    document = QJsonDocument::fromJson(json,&jsonError);
    if(!document.isNull() && (jsonError.error == QJsonParseError::NoError))
    {
        if(document.isObject())
        {
            QJsonObject object = document.object();
            if(object.contains("regStatus"))
            {
                regStatus = object.value("regStatus").toInt();
            }
        }
    }
    client.disconnectFromServer();
    return regStatus;
}

int userManager::loginCheck(int curType, string loginName, string loginPassword, QJsonObject &curUserJson)
{
    int loginStatus = -1;
    TcpClient client;
    client.connectToServer();
    QJsonObject object;
    object.insert("type", USER_loginCheck);
    QJsonObject data;
    data.insert("curType", curType);
    data.insert("loginName", loginName.c_str());
    data.insert("loginPassword", loginPassword.c_str());
    object.insert("data", data);
    QJsonDocument document;
    document.setObject(object);
    QByteArray json = client.getData(document.toJson(QJsonDocument::Compact), 5000, false);

    QJsonParseError jsonError;
    QJsonArray productJsonList;
    QJsonArray numberJsonList;
    QJsonArray checkedJsonList;
    document = QJsonDocument::fromJson(json,&jsonError);
    if(!document.isNull() && (jsonError.error == QJsonParseError::NoError))
    {
        if(document.isObject())
        {
            QJsonObject object = document.object();
            if(object.contains("loginStatus"))
            {
                loginStatus = object.value("loginStatus").toInt();
            }
            if (object.contains("curUser"))
            {
                curUserJson = object.value("curUser").toObject();
            }
            if (object.contains("key"))
            {
                string key;
                key = object.value("key").toString().toStdString();
                ofstream outfile;
                outfile.open("key");
                outfile << key;
                outfile.close();
            }
        }
    }
    client.disconnectFromServer();
    return loginStatus;
}

int userManager::changeUserName(int userId, string userName)
{
    int status = -1;
    TcpClient client;
    client.connectToServer();
    QJsonObject object;
    object.insert("type", USER_changeUserName);
    QJsonObject data;
    data.insert("userId", userId);
    data.insert("userName", userName.c_str());
    object.insert("data", data);
    ifstream infile;
    infile.open("key");
    string key;
    infile >> key;
    infile.close();
    object.insert("key", key.c_str());
    QJsonDocument document;
    document.setObject(object);
    QByteArray json = client.getData(document.toJson(QJsonDocument::Compact), 1000, false);

    QJsonParseError jsonError;
    QJsonArray productJsonList;
    QJsonArray numberJsonList;
    QJsonArray checkedJsonList;
    document = QJsonDocument::fromJson(json,&jsonError);
    if(!document.isNull() && (jsonError.error == QJsonParseError::NoError))
    {
        if(document.isObject())
        {
            QJsonObject object = document.object();
            if(object.contains("status"))
            {
                status = object.value("status").toInt();
            }
        }
    }
    client.disconnectFromServer();
    return status;
}

void userManager::getUser(int userId, userClass &user)
{
    TcpClient client;
    client.connectToServer();
    QJsonObject object;
    object.insert("type", USER_getUser);
    QJsonObject data;
    data.insert("userId", userId);
    object.insert("data", data);
    ifstream infile;
    infile.open("key");
    string key;
    infile >> key;
    infile.close();
    object.insert("key", key.c_str());
    QJsonDocument document;
    document.setObject(object);
    QByteArray json = client.getData(document.toJson(QJsonDocument::Compact), 1000, false);

    QJsonParseError jsonError;
    QJsonArray productJsonList;
    QJsonArray numberJsonList;
    QJsonArray checkedJsonList;
    document = QJsonDocument::fromJson(json,&jsonError);
    if(!document.isNull() && (jsonError.error == QJsonParseError::NoError))
    {
        if(document.isObject())
        {
            QJsonObject object = document.object();
            if(object.contains("user"))
            {
                user = sellerClass(object.value("user").toObject());
            }
        }
    }
    client.disconnectFromServer();
}
