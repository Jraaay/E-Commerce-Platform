#ifndef USERMANAGER_H
#define USERMANAGER_H

#include <fstream>
#include <string>
#include "nlohmann/json.hpp"
#include <QDebug>
#include "user.h"

using namespace std;
using json = nlohmann::json;

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
