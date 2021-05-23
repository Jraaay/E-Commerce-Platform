#ifndef USERMANAGER_H
#define USERMANAGER_H

#include <fstream>
#include <string>
#include <QDebug>
#include "user.h"
#include <QJsonDocument>
#include <QJsonParseError>
#include <QJsonObject>
#include <QJsonValue>
#include <QJsonArray>

using namespace std;

class userManager
{
public:
    userManager();
    static int getMaxUid();
    static vector<sellerClass> getSellerList();
    static vector<consumerClass> getConsumerList();
    static void getUser(int userId, userClass &user);
};

#endif // USERMANAGER_H
