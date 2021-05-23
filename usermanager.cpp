#include "usermanager.h"

userManager::userManager()
{

}

int userManager::getMaxUid()
{
    ifstream infile;
    string uidMaxJson = "";
    int uidMax = 0;
    infile.open("uidMaxFile.json");
    infile >> uidMaxJson;
    infile.close();
    QJsonParseError jsonError;
    QJsonDocument document = QJsonDocument::fromJson(uidMaxJson.c_str(),&jsonError);
    if(!document.isNull() && (jsonError.error == QJsonParseError::NoError))
    {
        if(document.isObject())
        {
            QJsonObject object = document.object();
            if(object.contains("uid"))
            {
                QJsonValue value = object.value("uid");
                uidMax = value.toInt();
            }
        }
    }
    else
    {
        uidMaxJson = "";
        qDebug() << jsonError.error << endl;
        const int tmp = 0;
        QJsonObject object;
        object.insert("uid", tmp);
        QJsonDocument document;
        document.setObject(object);
        QByteArray array = document.toJson(QJsonDocument::Compact);
        ofstream outFile;
        outFile.open("uidMaxFile.json");
        outFile << array.toStdString();
        outFile.close();
    }
    return uidMax;
}

vector<sellerClass> userManager::getSellerList()
{
    vector<sellerClass> sellerList;
    ifstream infile;
    string sellerJson = "";
    infile.open("sellerFile.json");
    infile >> sellerJson;
    infile.close();
    QJsonParseError jsonError;
    QJsonArray userListJson;
    QJsonDocument document = QJsonDocument::fromJson(sellerJson.c_str(),&jsonError);
    if(!document.isNull() && (jsonError.error == QJsonParseError::NoError))
    {
        if(document.isObject())
        {
            QJsonObject object = document.object();
            if(object.contains("data"))
            {
                QJsonValue value = object.value("data");
                userListJson = value.toArray();
            }
        }
        for (int i = 0; i < (int)userListJson.size(); i++)
        {
            QJsonArray userListJson1;
            QJsonDocument document1 = QJsonDocument::fromJson(userListJson[i].toString().toUtf8(),&jsonError);
            if(document1.isObject())
            {
                QJsonObject object = document1.object();
                sellerClass tmp;
                tmp.uid = object.value("uid").toInt();
                tmp.name = object.value("name").toString().toStdString();
                tmp.type = object.value("type").toInt();
                tmp.balance = object.value("balance").toDouble();
                tmp.setPass(object.value("password").toString().toStdString());
                sellerList.push_back(tmp);
            }

        }
    }
    else
    {
        qDebug() << jsonError.error << endl;
        QString tmp = "";
        QJsonObject object;
        object.insert("data", tmp);
        QJsonDocument document;
        document.setObject(object);
        QByteArray array = document.toJson(QJsonDocument::Compact);
        ofstream outFile;
        outFile.open("sellerFile.json");
        outFile << array.toStdString();
        outFile.close();
    }
    return sellerList;
}

vector<consumerClass> userManager::getConsumerList()
{
    vector<consumerClass> consumerList;
    ifstream infile;
    string consumerJson = "";
    infile.open("consumerFile.json");
    infile >> consumerJson;
    infile.close();
    QJsonParseError jsonError;
    QJsonArray userListJson;
    QJsonDocument document = QJsonDocument::fromJson(consumerJson.c_str(),&jsonError);
    if(!document.isNull() && (jsonError.error == QJsonParseError::NoError))
    {
        if(document.isObject())
        {
            QJsonObject object = document.object();
            if(object.contains("data"))
            {
                QJsonValue value = object.value("data");
                userListJson = value.toArray();
            }
        }
        for (int i = 0; i < (int)userListJson.size(); i++)
        {
            QJsonArray userListJson1;
            QJsonDocument document1 = QJsonDocument::fromJson(userListJson[i].toString().toUtf8(),&jsonError);
            if(document1.isObject())
            {
                QJsonObject object = document1.object();
                consumerClass tmp;
                tmp.uid = object.value("uid").toInt();
                tmp.name = object.value("name").toString().toStdString();
                tmp.type = object.value("type").toInt();
                tmp.balance = object.value("balance").toDouble();
                tmp.setPass(object.value("password").toString().toStdString());
                consumerList.push_back(tmp);
            }

        }
    }
    else
    {
        qDebug() << jsonError.error << endl;
        QString tmp = "";
        QJsonObject object;
        object.insert("data", tmp);
        QJsonDocument document;
        document.setObject(object);
        QByteArray array = document.toJson(QJsonDocument::Compact);
        ofstream outFile;
        outFile.open("consumerFile.json");
        outFile << array.toStdString();
        outFile.close();
    }
    return consumerList;
}

void userManager::getUser(int userId, userClass &user)
{
    vector<sellerClass> sellerList = getSellerList();
    vector<consumerClass> consumerList = getConsumerList();
    for (int i = 0; i < (int) sellerList.size(); i++)
    {
        if (sellerList[i].uid == userId)
        {
            user.uid = sellerList[i].uid;
            user.name = sellerList[i].name;
            user.type = sellerList[i].type;
            user.balance = sellerList[i].balance;
            user.setPass(sellerList[i].getPass());
            return;
        }
    }
    for (int i = 0; i < (int) consumerList.size(); i++)
    {
        if (consumerList[i].uid == userId)
        {
            user.uid = consumerList[i].uid;
            user.name = consumerList[i].name;
            user.type = consumerList[i].type;
            user.balance = consumerList[i].balance;
            user.setPass(consumerList[i].getPass());
            return;
        }
    }
}
