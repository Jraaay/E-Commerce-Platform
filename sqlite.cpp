#include "sqlite.h"
#include <QDebug>

/* 构造函数设置数据库的连接参数 */
sqlite::sqlite()
{
}

/* 插入单条数据 */
void sqlite::singleInsertData(productItem item) const
{
    TcpClient client;
    client.connectToServer();
    QJsonObject object;
    object.insert("type", SQLITE_singleInsertData);
    object.insert("data", item.getJson());
    ifstream infile;
    infile.open("key");
    string key;
    infile >> key;
    infile.close();
    object.insert("key", key.c_str());
    QJsonDocument document;
    document.setObject(object);
    client.getData(document.toJson(QJsonDocument::Compact), 0, false);
    client.disconnectFromServer();
}

/* 加购物车 */
void sqlite::modifyItemInCart(int productId, int userId, int number, bool checked)
{
    TcpClient client;
    client.connectToServer();
    QJsonObject object;
    object.insert("type", SQLITE_modifyItemInCart);
    QJsonObject data;
    data.insert("productId", productId);
    data.insert("userId", userId);
    data.insert("number", number);
    data.insert("checked", checked);
    object.insert("data", data);
    ifstream infile;
    infile.open("key");
    string key;
    infile >> key;
    infile.close();
    object.insert("key", key.c_str());
    QJsonDocument document;
    document.setObject(object);
    client.getData(document.toJson(QJsonDocument::Compact), 0, false);
    client.disconnectFromServer();
}

/* 删除购物车物品 */
void sqlite::deleteItemFromCart(int productId, int userId)
{
    TcpClient client;
    client.connectToServer();
    QJsonObject object;
    object.insert("type", SQLITE_deleteItemFromCart);
    QJsonObject data;
    data.insert("productId", productId);
    data.insert("userId", userId);
    object.insert("data", data);
    ifstream infile;
    infile.open("key");
    string key;
    infile >> key;
    infile.close();
    object.insert("key", key.c_str());
    QJsonDocument document;
    document.setObject(object);
    client.getData(document.toJson(QJsonDocument::Compact), 0, false);
    client.disconnectFromServer();
}

/* 列出购物车 */
void sqlite::queryCart(int userId, vector<productItem *> &productList, vector<int> &numberList, vector<bool> &checkedList)
{
    TcpClient client;
    client.connectToServer();
    QJsonObject object;
    object.insert("type", SQLITE_queryCart);
    QJsonObject data;
    data.insert("userId", userId);
    object.insert("data", data);
    ifstream infile;
    infile.open("key");
    string key;
    infile >> key;
    infile.close();
    object.insert("key", key.c_str());
    QJsonDocument document;
    document.setObject(object);
    QByteArray json = client.getData(document.toJson(QJsonDocument::Compact), 5000);

    QJsonParseError jsonError;
    QJsonArray productJsonList;
    QJsonArray numberJsonList;
    QJsonArray checkedJsonList;
    document = QJsonDocument::fromJson(json,&jsonError);
    if(!document.isNull() && (jsonError.error == QJsonParseError::NoError))
    {
        if(document.isObject())
        {
            QJsonObject object = document.object();
            if(object.contains("productList"))
            {
                productJsonList = object.value("productList").toArray();
            }
            if (object.contains("numberList"))
            {
                numberJsonList = object.value("numberList").toArray();
            }
            if (object.contains("checkedList"))
            {
                checkedJsonList = object.value("checkedList").toArray();
            }
        }
        for (int i = 0; i < productJsonList.size(); i++)
        {
            productList.push_back(new productItem(productJsonList[i].toObject()));
            numberList.push_back(numberJsonList[i].toInt());
            checkedList.push_back(checkedJsonList[i].toBool());
        }
    }
    client.disconnectFromServer();
}

/* 查询全部数据 */
vector<productItem *> sqlite::queryTable(string LIKE, string SORT) const
{
    vector<productItem *> productList;
    TcpClient client;
    client.connectToServer();
    QJsonObject object;
    object.insert("type", SQLITE_queryTable);
    QJsonObject data;
    data.insert("LIKE", LIKE.c_str());
    data.insert("SORT", SORT.c_str());
    object.insert("data", data);
    ifstream infile;
    infile.open("key");
    string key;
    infile >> key;
    infile.close();
    object.insert("key", key.c_str());
    QJsonDocument document;
    document.setObject(object);

    QByteArray json = client.getData(document.toJson(QJsonDocument::Compact), 5000);
    client.disconnectFromServer();

    QJsonParseError jsonError;
    QJsonArray productJsonList;
    document = QJsonDocument::fromJson(json,&jsonError);
    if(!document.isNull() && (jsonError.error == QJsonParseError::NoError))
    {
        if(document.isObject())
        {
            QJsonObject object = document.object();
            if(object.contains("productList"))
            {
                productJsonList = object.value("productList").toArray();
            }
        }
        for (int i = 0; i < productJsonList.size(); i++)
        {
            productList.push_back(new productItem(productJsonList[i].toObject()));
        }
    }
    return productList;
}

/* 更新单条数据 */
void sqlite::modifyData(productItem item, int updateImage) const
{
    TcpClient client;
    client.connectToServer();
    QJsonObject object;
    object.insert("type", SQLITE_modifyData);
    QJsonObject data;
    data.insert("item", item.getJson());
    data.insert("updateImage", updateImage);
    object.insert("data", data);
    ifstream infile;
    infile.open("key");
    string key;
    infile >> key;
    infile.close();
    object.insert("key", key.c_str());
    QJsonDocument document;
    document.setObject(object);

    client.getData(document.toJson(QJsonDocument::Compact));
    client.disconnectFromServer();
}

/* 删除单条数据 */
void sqlite::deleteData(int id) const
{
    TcpClient client;
    client.connectToServer();
    QJsonObject object;
    object.insert("type", SQLITE_deleteData);
    QJsonObject data;
    data.insert("id", id);
    object.insert("data", data);
    ifstream infile;
    infile.open("key");
    string key;
    infile >> key;
    infile.close();
    object.insert("key", key.c_str());
    QJsonDocument document;
    document.setObject(object);

    client.getData(document.toJson(QJsonDocument::Compact), 0, false);
    client.disconnectFromServer();
}

/* 新账户初始化 */
void sqlite::newDiscount(int id) const
{
    TcpClient client;
    client.connectToServer();
    QJsonObject object;
    object.insert("type", SQLITE_newDiscount);
    QJsonObject data;
    data.insert("id", id);
    object.insert("data", data);
    ifstream infile;
    infile.open("key");
    string key;
    infile >> key;
    infile.close();
    object.insert("key", key.c_str());
    QJsonDocument document;
    document.setObject(object);

    client.getData(document.toJson(QJsonDocument::Compact), 0, false);
    client.disconnectFromServer();
}

void sqlite::getDiscount(int userId, double &fooddiscount, double &clothesdiscount, double &bookdiscount) const
{
    TcpClient client;
    client.connectToServer();
    QJsonObject object;
    object.insert("type", SQLITE_getDiscount);
    QJsonObject data;
    data.insert("userId", userId);
    object.insert("data", data);
    ifstream infile;
    infile.open("key");
    string key;
    infile >> key;
    infile.close();
    object.insert("key", key.c_str());
    QJsonDocument document;
    document.setObject(object);

    QByteArray json = client.getData(document.toJson(QJsonDocument::Compact), 5000, false);

    QJsonParseError jsonError;
    QJsonArray productJsonList;
    QJsonArray numberJsonList;
    QJsonArray checkedJsonList;
    document = QJsonDocument::fromJson(json,&jsonError);
    if(!document.isNull() && (jsonError.error == QJsonParseError::NoError))
    {
        if(document.isObject())
        {
            QJsonObject object = document.object();
            if(object.contains("fooddiscount"))
            {
                fooddiscount = object.value("fooddiscount").toDouble();
            }
            if(object.contains("clothesdiscount"))
            {
                clothesdiscount = object.value("clothesdiscount").toDouble();
            }
            if(object.contains("bookdiscount"))
            {
                bookdiscount = object.value("bookdiscount").toDouble();
            }
        }
    }
    client.disconnectFromServer();
}

/* 保存折扣 */
void sqlite::setDiscount(int userId, double fooddiscount, double clothesdiscount, double bookdiscount) const
{
    TcpClient client;
    client.connectToServer();
    QJsonObject object;
    object.insert("type", SQLITE_setDiscount);
    QJsonObject data;
    data.insert("userId", userId);
    data.insert("fooddiscount", fooddiscount);
    data.insert("clothesdiscount", clothesdiscount);
    data.insert("bookdiscount", bookdiscount);
    object.insert("data", data);
    ifstream infile;
    infile.open("key");
    string key;
    infile >> key;
    infile.close();
    object.insert("key", key.c_str());
    QJsonDocument document;
    document.setObject(object);

    client.getData(document.toJson(QJsonDocument::Compact), 0, false);
    client.disconnectFromServer();
}

int sqlite::generateOrder(int userId)
{
    int orderId = 0;
    TcpClient client;
    client.connectToServer();
    QJsonObject object;
    object.insert("type", SQLITE_generateOrder);
    QJsonObject data;
    data.insert("userId", userId);
    object.insert("data", data);
    ifstream infile;
    infile.open("key");
    string key;
    infile >> key;
    infile.close();
    object.insert("key", key.c_str());
    QJsonDocument document;
    document.setObject(object);

    QByteArray json = client.getData(document.toJson(QJsonDocument::Compact), 5000, false);

    QJsonParseError jsonError;
    QJsonArray productJsonList;
    QJsonArray numberJsonList;
    QJsonArray checkedJsonList;
    document = QJsonDocument::fromJson(json,&jsonError);
    if(!document.isNull() && (jsonError.error == QJsonParseError::NoError))
    {
        if(document.isObject())
        {
            QJsonObject object = document.object();
            if(object.contains("orderId"))
            {
                orderId = object.value("orderId").toInt();
            }
        }
    }
    client.disconnectFromServer();
    return orderId;

}

int sqlite::buyOne(int userId, int productId)
{
    int payStatus = -1;
    TcpClient client;
    client.connectToServer();
    QJsonObject object;
    object.insert("type", SQLITE_buyOneThing);
    QJsonObject data;
    data.insert("userId", userId);
    data.insert("productId", productId);
    object.insert("data", data);
    ifstream infile;
    infile.open("key");
    string key;
    infile >> key;
    infile.close();
    object.insert("key", key.c_str());
    QJsonDocument document;
    document.setObject(object);
    QByteArray json = client.getData(document.toJson(QJsonDocument::Compact), 5000, false);

    QJsonParseError jsonError;
    QJsonArray productJsonList;
    QJsonArray numberJsonList;
    QJsonArray checkedJsonList;
    document = QJsonDocument::fromJson(json,&jsonError);
    if(!document.isNull() && (jsonError.error == QJsonParseError::NoError))
    {
        if(document.isObject())
        {
            QJsonObject object = document.object();
            if(object.contains("payStatus"))
            {
                payStatus = object.value("payStatus").toInt();
            }
        }
    }
    client.disconnectFromServer();
    return payStatus;
}

void sqlite::getOrder(int orderId, bool &canceled, bool &paied, long long &time, int &userId, vector<productItem *> &orderList, vector<int> &count, vector<double> &price, double &priceSum)
{
    TcpClient client;
    client.connectToServer();
    QJsonObject object;
    object.insert("type", SQLITE_getOrder);
    QJsonObject data;
    data.insert("orderId", orderId);
    object.insert("data", data);
    ifstream infile;
    infile.open("key");
    string key;
    infile >> key;
    infile.close();
    object.insert("key", key.c_str());
    QJsonDocument document;
    document.setObject(object);

    QByteArray json = client.getData(document.toJson(QJsonDocument::Compact), 5000);

    QJsonParseError jsonError;
    QJsonArray orderJsonList;
    QJsonArray countJsonList;
    QJsonArray priceJsonList;
    document = QJsonDocument::fromJson(json,&jsonError);
    if(!document.isNull() && (jsonError.error == QJsonParseError::NoError))
    {
        if(document.isObject())
        {
            QJsonObject object = document.object();
            if(object.contains("paied"))
            {
                paied = object.value("paied").toBool();
            }
            if(object.contains("canceled"))
            {
                canceled = object.value("canceled").toBool();
            }
            if (object.contains("time"))
            {
                time = object.value("time").toVariant().toLongLong();
            }
            if (object.contains("userId"))
            {
                userId = object.value("userId").toInt();
            }
            if (object.contains("priceSum"))
            {
                priceSum = object.value("priceSum").toDouble();
            }
            if (object.contains("orderList"))
            {
                orderJsonList = object.value("orderList").toArray();
            }
            if (object.contains("price"))
            {
                priceJsonList = object.value("price").toArray();
            }
            if (object.contains("count"))
            {
                countJsonList = object.value("count").toArray();
            }
        }
        for (int i = 0; i < orderJsonList.size(); i++)
        {
            orderList.push_back(new productItem(orderJsonList[i].toObject()));
            count.push_back(countJsonList[i].toInt());
            price.push_back(priceJsonList[i].toDouble());
        }
    }
    client.disconnectFromServer();
}

int sqlite::payOrder(int orderId)
{
    int payStatus = -1;
    TcpClient client;
    client.connectToServer();
    QJsonObject object;
    object.insert("type", pay);
    QJsonObject data;
    data.insert("orderId", orderId);
    object.insert("data", data);
    ifstream infile;
    infile.open("key");
    string key;
    infile >> key;
    infile.close();
    object.insert("key", key.c_str());
    QJsonDocument document;
    document.setObject(object);
    QByteArray json = client.getData(document.toJson(QJsonDocument::Compact), 5000, false);

    QJsonParseError jsonError;
    QJsonArray orderIdJsonList;
    QJsonArray priceSumJsonList;
    QJsonArray timeJsonList;
    QJsonArray paidJsonList;
    document = QJsonDocument::fromJson(json,&jsonError);
    if(!document.isNull() && (jsonError.error == QJsonParseError::NoError))
    {
        if(document.isObject())
        {
            QJsonObject object = document.object();
            if (object.contains("payStatus"))
            {
                payStatus = object.value("payStatus").toInt();
            }
        }
    }
    client.disconnectFromServer();
    return payStatus;
}

int sqlite::cancelOrder(int orderId)
{
    int cancelStatus = -1;
    TcpClient client;
    client.connectToServer();
    QJsonObject object;
    object.insert("type", SQLITE_cancelOrder);
    QJsonObject data;
    data.insert("orderId", orderId);
    object.insert("data", data);
    ifstream infile;
    infile.open("key");
    string key;
    infile >> key;
    infile.close();
    object.insert("key", key.c_str());
    QJsonDocument document;
    document.setObject(object);
    QByteArray json = client.getData(document.toJson(QJsonDocument::Compact), 5000, false);

    QJsonParseError jsonError;
    QJsonArray orderIdJsonList;
    QJsonArray priceSumJsonList;
    QJsonArray timeJsonList;
    QJsonArray paidJsonList;
    document = QJsonDocument::fromJson(json,&jsonError);
    if(!document.isNull() && (jsonError.error == QJsonParseError::NoError))
    {
        if(document.isObject())
        {
            QJsonObject object = document.object();
            if (object.contains("cancelStatus"))
            {
                cancelStatus = object.value("cancelStatus").toInt();
            }
        }
    }
    client.disconnectFromServer();
    return cancelStatus;
}

void sqlite::getOrderList(int userId, vector<int> &orderId, vector<double> &priceSum, vector<long long> &time, vector<bool> &paid, vector<bool> &canceled)
{
    TcpClient client;
    client.connectToServer();
    QJsonObject object;
    object.insert("type", SQLITE_getOrderList);
    QJsonObject data;
    data.insert("userId", userId);
    object.insert("data", data);
    ifstream infile;
    infile.open("key");
    string key;
    infile >> key;
    infile.close();
    object.insert("key", key.c_str());
    QJsonDocument document;
    document.setObject(object);

    QByteArray json = client.getData(document.toJson(QJsonDocument::Compact), 5000);

    QJsonParseError jsonError;
    QJsonArray orderIdJsonList;
    QJsonArray priceSumJsonList;
    QJsonArray timeJsonList;
    QJsonArray paidJsonList;
    QJsonArray canceledJsonList;
    document = QJsonDocument::fromJson(json,&jsonError);
    if(!document.isNull() && (jsonError.error == QJsonParseError::NoError))
    {
        if(document.isObject())
        {
            QJsonObject object = document.object();
            if (object.contains("orderId"))
            {
                orderIdJsonList = object.value("orderId").toArray();
            }
            if (object.contains("priceSum"))
            {
                priceSumJsonList = object.value("priceSum").toArray();
            }
            if (object.contains("time"))
            {
                timeJsonList = object.value("time").toArray();
            }
            if (object.contains("paid"))
            {
                paidJsonList = object.value("paid").toArray();
            }
            if (object.contains("canceled"))
            {
                canceledJsonList = object.value("canceled").toArray();
            }
        }
        for (int i = 0; i < orderIdJsonList.size(); i++)
        {
            orderId.push_back(orderIdJsonList[i].toInt());
            priceSum.push_back(priceSumJsonList[i].toDouble());
            time.push_back(timeJsonList[i].toVariant().toLongLong());
            paid.push_back(paidJsonList[i].toBool());
            canceled.push_back(canceledJsonList[i].toBool());
        }
    }
    client.disconnectFromServer();
}
