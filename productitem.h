#ifndef PRODUCTITEM_H
#define PRODUCTITEM_H

#define FOODTYPE 1
#define CLOTHESTYPE 2
#define BOOKTYPE 3

#include <string>
#include <QImage>
#include <vector>
#include <QDebug>
#include <math.h>
#include <QJsonObject>
#include <QJsonArray>

using namespace std;

class productItem
{
public:
    string name;
    string sellerName;
    string description;
    double price;
    int remaining;
    vector<QByteArray> photo;
    int mainPhoto;
    int type;
    int id;
    int seller;
    double discount;
    productItem(double discount1, string name, string description, double price, int remaining, vector<QByteArray> photo, int mainPhoto, int type, int id = -1, int seller1 = -1);
    productItem();
    productItem(QJsonObject data);
    virtual double getPrice()
    {
        return discount;
    };
    virtual ~productItem()
    {
    }
    QJsonObject getJson();
};

class foodItem : public productItem
{
public:
    virtual double getPrice() override
    {
        return discount;
    }
    virtual ~foodItem()
    {
    }
};

class clothesItem : public productItem
{
public:
    virtual double getPrice() override
    {
        return discount;
    }
    virtual ~clothesItem()
    {
    }
};

class bookItem : public productItem
{
public:
    virtual double getPrice() override
    {
        return discount;
    }
    virtual ~bookItem()
    {
    }
};

#endif // PRODUCTITEM_H
