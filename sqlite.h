#ifndef SQLITE_H
#define SQLITE_H

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include "productitem.h"
#include <QFile>
#include <QFileInfo>
#include <QDir>
#include <time.h>
#include <QVariant>
#include "tcpclient.h"
#include <QJsonDocument>

class sqlite
{
public:
    sqlite();
    // 打开数据库

    // 创建数据表
    void createTable(void) const;
    // 判断数据表是否存在
    bool isTableExist(QString &tableName) const;
    // 查询全部数据
    vector<productItem *> queryTable(string LIKE = "", string SORT = "") const;
    // 插入单条数据
    void singleInsertData(productItem item) const;
    void modifyItemInCart(int productId, int userId, int number = -1, bool checked = true);
    void deleteItemFromCart(int productId, int userId);
    void queryCart(int userId, vector<productItem *> &productList, vector<int> &numberList, vector<bool> &checkedList);
    // 修改数据
    void modifyData(productItem item, int updateImage) const;
    // 删除数据
    void deleteData(int id) const;
    void newDiscount(int id) const;
    void getDiscount(int userId, double &fooddiscount, double &clothesdiscount, double &bookdiscount) const;
    void setDiscount(int userId, double fooddiscount, double clothesdiscount, double bookdiscount) const;
    int generateOrder(int userId, vector<productItem> orderList, vector<int> count, vector<double> price, double priceSum);
    void getOrder(int orderId, bool &paied, long long &time, int &userId, vector<productItem *> &orderList, vector<int> &count, vector<double> &price, double &priceSum);
    void payOrder(int orderId);
    void getOrderList(int userId, vector<int> &orderId, vector<double> &priceSum, vector<long long> &time, vector<bool> &paid);

private:
    QSqlDatabase db;
};

#endif // SQLITE_H
