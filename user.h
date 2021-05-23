#ifndef USER_H
#define USER_H
#define CONSUMERTYPE 1
#define SELLERTYPE 2
#define GUESTTYPE 3

#include <string>
#include <QJsonObject>
#include <QJsonDocument>

using namespace std;

class userClass
{
public:
    string name;
    double balance;
    int type;
    int uid;
    virtual int getUserType() = 0;
    string getPass()
    {
        return password;
    };
    void setPass(string pass)
    {
        password = pass;
    };
    void recharge(double money)
    {
        balance += money;
    }
    string getJson();
    virtual ~userClass()
    {
    }

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
    virtual ~consumerClass()
    {
    }
};

class sellerClass : public userClass
{
public:
    int getUserType() override
    {
        return type;
    };
    virtual ~sellerClass()
    {
    }
};

#endif // USER_H
