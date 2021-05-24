#include "productitem.h"

productItem::productItem(double discount1, string name1, string description1, double price1, int remaining1, vector<QByteArray> photo1, int mainPhoto1, int type1, int id1, int seller1)
{
    discount = discount1;
    name = name1;
    description = description1;
    price = price1;
    remaining = remaining1;
    photo = photo1;
    mainPhoto = mainPhoto1;
    type = type1;
    id = id1;
    seller = seller1;
}

productItem::productItem()
{
}


productItem::productItem(QJsonObject data)
{
    name = data.value("name").toString().toStdString();
    description = data.value("description").toString().toStdString();
    price = data.value("price").toDouble();
    remaining = data.value("remaining").toInt();
    discount = data.value("discount").toDouble();
    QJsonArray photoArray = data.value("photo").toArray();
    for (int i = 0; i < photoArray.size(); i++)
    {
        photo.push_back(QByteArray::fromBase64(photoArray[i].toString().toUtf8()));
    }
    mainPhoto = data.value("mainPhoto").toInt();
    type = data.value("type").toInt();
    if (data.contains("id"))
    {
        id = data.value("id").toInt();
    }
    if (data.contains("seller"))
    {
        seller = data.value("seller").toInt();
    }
    if (data.contains("sellerName"))
    {
        sellerName = data.value("sellerName").toString().toStdString();
    }
}

QJsonObject productItem::getJson()
{
    QJsonObject object;
    object.insert("name", name.c_str());
    object.insert("id", id);
    object.insert("type", type);
    object.insert("price", price);
    object.insert("seller", seller);
    object.insert("sellerName", sellerName.c_str());
    object.insert("discount", discount);
    object.insert("mainPhoto", mainPhoto);
    object.insert("remaining", remaining);
    object.insert("description", description.c_str());
    object.insert("discount", 0);
    object.insert("seller", seller);
    QJsonArray array;
    for (int i = 0; i < (int)photo.size(); i++)
    {
        array.append(QString::fromStdString(photo[i].toStdString()));
    }
    object.insert("photo", array);
    return object;
}
