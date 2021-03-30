#include "sqlite.h"
#include <QDebug>

//构造函数设置数据库的连接参数
sqlite::sqlite()
{
    if (QSqlDatabase::contains("qt_sql_default_connection"))
    {
        db = QSqlDatabase::database("qt_sql_default_connection");
    }
    else
    {
        // 建立和SQlite数据库的连接
        db = QSqlDatabase::addDatabase("QSQLITE");
        // 设置数据库文件的名字
        db.setDatabaseName("productList.db");
    }
}

//打开数据库
bool sqlite::openDb()
{
    if (!db.open())
    {
        qDebug() << "Error: Failed to connect database." << db.lastError();
    }
    else
    {
        // do something
    }
    return true;
}

// 判断数据库中某个数据表是否存在
bool sqlite::isTableExist(QString& tableName)
{
    if(db.tables().contains(tableName))
    {
        return true;
    }
    return false;
}

// 创建数据表
void sqlite::createTable()
{
    // 用于执行sql语句的对象
    QSqlQuery sqlQuery;
    // 构建创建数据库的sql语句字符串
    QString createSql = QString("CREATE TABLE `productItem` (`id` INTEGER  PRIMARY KEY,`name` TEXT NOT NULL,`description` TEXT,`price` DOUBLE(32,2) NOT NULL,`remaining` INTEGER  NOT NULL,`mainPhoto` INTEGER ,`type` INTEGER  NOT NULL);");
    sqlQuery.prepare(createSql);
    // 执行sql语句
    if(!sqlQuery.exec())
    {
        qDebug() << "Error: Fail to create table. " << sqlQuery.lastError();
    }
    else
    {
        qDebug() << "Table created!";
    }
    // 构建创建数据库的sql语句字符串
    createSql = QString("CREATE TABLE `productPhoto` (`id` INTEGER PRIMARY KEY, `productId` INTEGER NOT NULL, `photo` TEXT NOT NULL);");
    sqlQuery.prepare(createSql);
    // 执行sql语句
    if(!sqlQuery.exec())
    {
        qDebug() << "Error: Fail to create table. " << sqlQuery.lastError();
    }
    else
    {
        qDebug() << "Table created!";
    }
}

void sqlite::singleInsertData(productItem item) // 插入单条数据
{
    QSqlQuery sqlQuery;
    QString insertItem;
    QTextStream(&insertItem) << "INSERT INTO `productItem` (`name`,`description`,`price`,`remaining`,`mainPhoto`,`type`) VALUES (";
    QTextStream(&insertItem) << "'" << item.name.c_str() << "',";
    QTextStream(&insertItem) << "'" << item.description.c_str() << "',";
    QTextStream(&insertItem) << "'" << item.price << "',";
    QTextStream(&insertItem) << "'" << item.remaining << "',";
    QTextStream(&insertItem) << "'" << item.mainPhoto << "',";
    QTextStream(&insertItem) << "'" << item.type << "');";
    qDebug() << insertItem;
    sqlQuery.prepare(insertItem);
    // 执行sql语句
    if(!sqlQuery.exec())
    {
        qDebug() << "Error: Fail to create table. " << sqlQuery.lastError();
    }
    else
    {
        qDebug() << "Table created!";
    }
}

// 查询全部数据
vector<productItem> sqlite::queryTable()
{
    vector<productItem> productList;
    QSqlQuery sqlQuery;
    sqlQuery.exec("SELECT * FROM `productItem`");
    if(!sqlQuery.exec())
    {
        qDebug() << "Error: Fail to query table. " << sqlQuery.lastError();
    }
    else
    {
        while(sqlQuery.next())
        {
            productItem tmp;
            tmp.name = sqlQuery.value(1).toString().toStdString();
            tmp.description = sqlQuery.value(2).toString().toStdString();
            tmp.price = sqlQuery.value(3).toDouble();
            tmp.remaining = sqlQuery.value(4).toInt();
            tmp.mainPhoto = sqlQuery.value(5).toInt();
            tmp.type = sqlQuery.value(6).toInt();
            productList.push_back(tmp);
        }
    }
}

//关闭数据库
void sqlite::closeDb(void)
{
    db.close();
}
