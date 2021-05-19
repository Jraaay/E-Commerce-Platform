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

class sqlite
{
public:
    sqlite();
    // 打开数据库
    bool openDb(void);
    // 创建数据表
    void createTable(void) const;
    // 判断数据表是否存在
    bool isTableExist(QString &tableName) const;
    // 查询全部数据
    vector<productItem *> queryTable(string LIKE = "", string SORT = "") const;
    // 插入单条数据
    void singleInsertData(productItem item);
    void modifyItemInCart(int productId, int userId, int number = -1, bool checked = true);
    void deleteItemFromCart(int productId, int userId);
    void queryCart(int userId, vector<productItem *> &productList, vector<int> &numberList, vector<bool> &checkedList);
    // 修改数据
    void modifyData(productItem item, int updateImage) const;
    // 删除数据
    void deleteData(int id);
    void newDiscount(int id);
    vector<vector<double>> getDiscount();
    void setDiscount(vector<vector<double>> discount);
    int generateOrder(int userId, vector<productItem> orderList, vector<int> count, vector<double> price, double priceSum);
    void getOrder(int orderId, bool &paied, long long &time, int &userId, vector<productItem *> &orderList, vector<int> &count, vector<double> &price, double &priceSum);
    void payOrder(int orderId);
    void getOrderList(int userId, vector<int> &orderId, vector<double> &priceSum, vector<long long> &time, vector<bool> &paid);
    // 关闭数据库
    void closeDb(void);

private:
    QSqlDatabase db;
};

#endif // SQLITE_H
