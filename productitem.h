#ifndef PRODUCTITEM_H
#define PRODUCTITEM_H

#define FOODTYPE 1
#define CLOTHESTYPE 2
#define BOOKTYPE 3

#include <string>
#include <QImage>
#include <vector>
#include <QDebug>

using namespace std;

class productItem
{
public:
    string name;
    string description;
    double price;
    int remaining;
    vector<QString> photo;
    int mainPhoto;
    int type;
    int id;
    int seller;
    productItem(string name, string description, double price, int remaining, vector<QString> photo, int mainPhoto, int type, int id = -1, int seller1 = -1);
    productItem();
    virtual double getPrice(vector<vector<double>> discount)
    {
        qDebug() << discount;
        return price;
    };
    virtual ~productItem()
    {
    }
};

class foodItem : public productItem
{
public:
    virtual double getPrice(vector<vector<double>> discount) override
    {
        double discountThis = 1;
        for (int i = 0; i < (int)discount.size(); i++)
        {
            if ((int)discount[i][3] == seller)
            {
                discountThis = discount[i][FOODTYPE - 1];
                break;
            }
        }
        return (double)(int)(price * discountThis * 100) / 100;
    }
    virtual ~foodItem()
    {
    }
};

class clothesItem : public productItem
{
public:
    virtual double getPrice(vector<vector<double>> discount) override
    {
        double discountThis = 1;
        for (int i = 0; i < (int)discount.size(); i++)
        {
            if ((int)discount[i][3] == seller)
            {
                discountThis = discount[i][CLOTHESTYPE - 1];
                break;
            }
        }
        return (double)(int)(price * discountThis * 100) / 100;
    }
    virtual ~clothesItem()
    {
    }
};

class bookItem : public productItem
{
public:
    virtual double getPrice(vector<vector<double>> discount) override
    {
        double discountThis = 1;
        for (int i = 0; i < (int)discount.size(); i++)
        {
            if ((int)discount[i][3] == seller)
            {
                discountThis = discount[i][BOOKTYPE - 1];
                break;
            }
        }
        return (double)(int)(price * discountThis * 100) / 100;
    }
    virtual ~bookItem()
    {
    }
};

#endif // PRODUCTITEM_H
