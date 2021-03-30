#ifndef SQLITE_H
#define SQLITE_H

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include "productitem.h"

class sqlite
{
public:
    sqlite();
    // 打开数据库
    bool openDb(void);
    // 创建数据表
    void createTable(void);
    // 判断数据表是否存在
    bool isTableExist(QString& tableName);
    // 查询全部数据
    vector<productItem> queryTable();
    // 插入单条数据
    void singleInsertData(productItem item);
    // 修改数据
    void modifyData(/*int id, QString name, int age*/);
    // 删除数据
    void deleteData(/*int id*/);
    // 关闭数据库
    void closeDb(void);

private:
    QSqlDatabase db;
};

#endif // SQLITE_H
