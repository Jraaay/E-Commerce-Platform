#include "productitem.h"

productItem::productItem(string name1, string description1, double price1, int remaining1, vector<QImage> photo1, int mainPhoto1, int type1)
{
    name = name1;
    description = description1;
    price = price1;
    remaining = remaining1;
    photo = photo1;
    mainPhoto = mainPhoto1;
    type = type1;
}

productItem::productItem()
{

}
