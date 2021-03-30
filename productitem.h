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
    vector<QImage> photo;
    int mainPhoto;
    int type;
    productItem(string name, string description, double price, int remaining, vector<QImage> photo, int mainPhoto, int type);
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
