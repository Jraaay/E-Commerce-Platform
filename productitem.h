#ifndef PRODUCTITEM_H
#define PRODUCTITEM_H

#define FOODTYPE 1

#include <string>
#include <QImage>
#include <vector>

using namespace  std;

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
    productItem(string name, string description, double price, int remaining, vector<QString> photo, int mainPhoto, int type, int id = -1);
    productItem();
    virtual double getPrice(vector<double> discount)
    {
        return price * discount[type];
    }
};

class foodItem : public productItem
{

};

#endif // PRODUCTITEM_H
