#ifndef USER_H
#define USER_H
#define CONSUMERTYPE 1
#define SELLERTYPE 2
#define GUESTTYPE 3

#include <string>
#include "nlohmann/json.hpp"

using namespace std;
using json = nlohmann::json;

class userClass
{
public:
    string name;
    double balance;
    int type;
    int uid;
    virtual int getUserType()=0;
    string getPass()
    {
        return password;
    };
    void setPass(string pass)
    {
        password = pass;
    };
    string getJson();
private:
    string password;
};

class consumerClass : public userClass
{
public:
    int getUserType() override
    {
        return type;
    };
};

class sellerClass : public userClass
{
public:
    int getUserType() override
    {
        return type;
    };
};


#endif // USER_H
