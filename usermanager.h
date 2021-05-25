#ifndef USERMANAGER_H
#define USERMANAGER_H

#define PASSWORDWRONG 1
#define USERNOTEXIST 2

#include <fstream>
#include <string>
#include <QDebug>
#include "user.h"
#include <QJsonDocument>
#include <QJsonParseError>
#include <QJsonObject>
#include <QJsonValue>
#include <QJsonArray>
#include "tcpclient.h"

using namespace std;

class userManager
{
public:
    userManager();
    static void logout();
    static void recharge(int userId, double moneyToCharge);
    static void changePassword(int userId, string password);
    static int loginCheck(int curType, string loginName, string loginPassword, QJsonObject &curUserJson);
    static int createUser(int curType, string loginName, string loginPassword);
    static int getMaxUid();
    static int changeUserName(int userId, string userName);
    static void getUser(int userId, userClass &user);
};

#endif // USERMANAGER_H
