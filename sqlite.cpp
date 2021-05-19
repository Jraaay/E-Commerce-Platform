#include "sqlite.h"
#include <QDebug>

/* 构造函数设置数据库的连接参数 */
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

/* 打开数据库 */
bool sqlite::openDb()
{
    if (!db.open())
    {
        qDebug() << "Error: Failed to connect database." << db.lastError();
    }
    else
    {
        QString tableName = "productItem";
        if (!isTableExist(tableName))
        {
            createTable();
        }
    }
    return true;
}

/* 判断数据库中某个数据表是否存在 */
bool sqlite::isTableExist(QString &tableName) const
{
    if (db.tables().contains(tableName))
    {
        return true;
    }
    return false;
}

/* 创建数据表 */
void sqlite::createTable() const
{
    // 用于执行sql语句的对象
    QSqlQuery sqlQuery;
    // 构建创建数据库的sql语句字符串
    QString createSql = QString("CREATE TABLE `productItem` (`id` INTEGER  PRIMARY KEY,`name` TEXT NOT NULL,`description` TEXT,`price` DOUBLE(32,2) NOT NULL,`remaining` INTEGER  NOT NULL,`mainPhoto` INTEGER ,`type` INTEGER  NOT NULL, `deleted` BOOLEAN DEFAULT false, `seller` INTEGER  NOT NULL);");
    sqlQuery.prepare(createSql);
    // 执行sql语句
    if (!sqlQuery.exec())
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
    if (!sqlQuery.exec())
    {
        qDebug() << "Error: Fail to create table. " << sqlQuery.lastError();
    }
    else
    {
        qDebug() << "Table created!";
    }
    createSql = QString("CREATE TABLE `discount` (`fooddiscount` DOUBLE(32,2) NOT NULL,`clothesdiscount` DOUBLE(32,2) NOT NULL,`bookdiscount` DOUBLE(32,2) NOT NULL, `seller` INTEGER PRIMARY KEY NOT NULL);");
    sqlQuery.prepare(createSql);
    // 执行sql语句
    if (!sqlQuery.exec())
    {
        qDebug() << "Error: Fail to create table. " << sqlQuery.lastError();
    }
    else
    {
        qDebug() << "Table created!";
    }
}

/* 插入单条数据 */
void sqlite::singleInsertData(productItem item) const
{
    QSqlQuery sqlQuery;
    sqlQuery.prepare("INSERT INTO `productItem` (`name`,`description`,`price`,`remaining`,`mainPhoto`,`type`, `seller`) VALUES (:name, :description, :price, :remaining, :mainPhoto, :type, :seller)");
    sqlQuery.bindValue(":name", item.name.c_str());
    sqlQuery.bindValue(":description", item.description.c_str());
    sqlQuery.bindValue(":price", item.price);
    sqlQuery.bindValue(":remaining", item.remaining);
    sqlQuery.bindValue(":mainPhoto", item.mainPhoto);
    sqlQuery.bindValue(":type", item.type);
    sqlQuery.bindValue(":seller", item.seller);

    // 执行sql语句
    if (!sqlQuery.exec())
    {
        qDebug() << "Error: Fail to insert. " << sqlQuery.lastError();
    }
    else
    {
        qDebug() << "Insert successed!" << sqlQuery.lastInsertId().toInt();
    }
    int productId = sqlQuery.lastInsertId().toInt();
    string destFolder = "./source/" + to_string(productId); // 图片保存到软件目录的source中防止删除源文件
    QDir dir;
    dir.mkpath(destFolder.c_str());
    for (int i = 0; i < int(item.photo.size()); i++)
    {
        string dest = destFolder + "/" + to_string(i) + "." + QFileInfo(item.photo[i]).suffix().toStdString();
        QFile::copy(item.photo[i], dest.c_str());
        sqlQuery.prepare("INSERT INTO `productPhoto` (`productId`,`photo`) VALUES (:productId, :photo)");
        sqlQuery.bindValue(":productId", productId);
        sqlQuery.bindValue(":photo", dest.c_str());
        if (!sqlQuery.exec())
        {
            qDebug() << "Error: Fail to insert. " << sqlQuery.lastError();
        }
        else
        {
            qDebug() << "Insert successed!" << sqlQuery.lastInsertId().toInt();
        }
    }
}

/* 查询全部数据 */
vector<productItem *> sqlite::queryTable(string LIKE, string SORT) const
{
    vector<productItem *> productList;
    QSqlQuery sqlQuery;
    string sqlCommand = "SELECT * FROM `productItem` WHERE `deleted`==false ";
    if (LIKE != "")
    {
        sqlCommand += " AND (`name` LIKE '%" + LIKE + "%' OR `description` LIKE '%" + LIKE + "%')";
    }
    sqlCommand += SORT;
    qDebug() << sqlCommand.c_str();
    sqlQuery.exec(sqlCommand.c_str());
    if (!sqlQuery.exec())
    {
        qDebug() << "Error: Fail to query table. " << sqlQuery.lastError();
    }
    else
    {
        while (sqlQuery.next())
        {
            productItem *tmp;
            switch (sqlQuery.value(6).toInt())
            {
            case FOODTYPE:
                tmp = new foodItem;
                break;
            case CLOTHESTYPE:
                tmp = new clothesItem;
                break;
            case BOOKTYPE:
                tmp = new bookItem;
                break;
            default:
                tmp = new productItem;
            }

            tmp->id = sqlQuery.value(0).toInt();
            tmp->name = sqlQuery.value(1).toString().toStdString();
            tmp->description = sqlQuery.value(2).toString().toStdString();
            tmp->price = sqlQuery.value(3).toDouble();
            tmp->remaining = sqlQuery.value(4).toInt();
            tmp->mainPhoto = sqlQuery.value(5).toInt();
            tmp->type = sqlQuery.value(6).toInt();
            tmp->seller = sqlQuery.value(8).toInt();
            QSqlQuery sqlQueryPhoto;
            sqlQueryPhoto.exec("SELECT * FROM `productPhoto` WHERE `productId` LIKE " + sqlQuery.value(0).toString());
            while (sqlQueryPhoto.next())
            {
                tmp->photo.push_back(sqlQueryPhoto.value(2).toString());
            }
            productList.push_back(tmp);
        }
    }
    return productList;
}

/* 更新单条数据 */
void sqlite::modifyData(productItem item, int updateImage) const
{
    QSqlQuery sqlQuery;
    sqlQuery.prepare("UPDATE `productItem` SET "
                     "`name`=:name,"
                     "`description`=:description,"
                     "`price`=:price,"
                     "`remaining`=:remaining,"
                     "`mainPhoto`=:mainPhoto,"
                     "`type`=:type "
                     "WHERE `id`=:id;");
    sqlQuery.bindValue(":name", item.name.c_str());
    sqlQuery.bindValue(":description", item.description.c_str());
    sqlQuery.bindValue(":price", item.price);
    sqlQuery.bindValue(":remaining", item.remaining);
    sqlQuery.bindValue(":mainPhoto", item.mainPhoto);
    sqlQuery.bindValue(":type", item.type);
    sqlQuery.bindValue(":id", item.id);

    // 执行sql语句
    if (!sqlQuery.exec())
    {
        qDebug() << "Error: Fail to update. " << sqlQuery.lastError();
    }
    else
    {
        qDebug() << "Update successed!";
    }
    if (updateImage) //对图片更改进行处理
    {
        const int productId = item.id;
        sqlQuery.prepare("DELETE FROM `productPhoto` "
                         "WHERE `productId`=:productId;");
        sqlQuery.bindValue(":productId", productId);
        if (!sqlQuery.exec())
        {
            qDebug() << "Error: Fail to delete old photos. " << sqlQuery.lastError();
        }
        else
        {
            qDebug() << "Old photos deleting successed!";
        }
        string destFolder = "./source/" + to_string(productId);
        QDir dir;
        dir.setPath(destFolder.c_str());
        bool oldFolderExist = dir.exists();
        if (oldFolderExist)
        {
            destFolder = "./source/" + to_string(productId) + "_tmp";
        }
        dir.setPath(".");
        dir.mkpath((destFolder).c_str());
        for (int i = 0; i < int(item.photo.size()); i++)
        {
            string dest = destFolder + "/" + to_string(i) + "." + QFileInfo(item.photo[i]).suffix().toStdString();
            QFile::copy(item.photo[i], dest.c_str());
            sqlQuery.prepare("INSERT INTO `productPhoto` (`productId`,`photo`) VALUES (:productId, :photo)");
            sqlQuery.bindValue(":productId", productId);
            sqlQuery.bindValue(":photo", ("./source/" + to_string(productId) + "/" + to_string(i) + "." + QFileInfo(item.photo[i]).suffix().toStdString()).c_str());
            if (!sqlQuery.exec())
            {
                qDebug() << "Error: Fail to update photos. " << sqlQuery.lastError();
            }
            else
            {
                qDebug() << "Photos update successed!";
            }
        }
        if (oldFolderExist)
        {
            dir.setPath(("./source/" + to_string(productId)).c_str());
            dir.removeRecursively();
            dir.setPath(".");
            qDebug() << destFolder.c_str() << endl
                     << ("./source/" + to_string(productId)).c_str();
            qDebug() << dir.rename((destFolder).c_str(), ("./source/" + to_string(productId)).c_str());
        }
    }
}

/* 删除单条数据 */
void sqlite::deleteData(int id) const
{
    QSqlQuery sqlQuery;

    sqlQuery.prepare("UPDATE `productItem` SET "
                     "`deleted`=true "
                     "WHERE `id`=:id;");
    sqlQuery.bindValue(":id", id);
    if (!sqlQuery.exec())
    {
        qDebug() << "Error: Fail to delete. " << sqlQuery.lastError();
    }
    else
    {
        qDebug() << "Deleting successed!";
    }
}

/* 新账户初始化 */
void sqlite::newDiscount(int id) const
{
    QSqlQuery sqlQuery;
    QString createSql = QString("INSERT INTO `discount` (`fooddiscount`,`clothesdiscount`, `bookdiscount`, `seller`) VALUES (1, 1, 1, :seller)");
    sqlQuery.prepare(createSql);
    sqlQuery.bindValue(":seller", id);
    if (!sqlQuery.exec())
    {
        qDebug() << "Error: Fail to insert discount. " << sqlQuery.lastError();
    }
    else
    {
        qDebug() << "Insert success!";
    }
}

/* 获取所有折扣 */
vector<vector<double>> sqlite::getDiscount() const
{
    vector<vector<double>> discount;
    QSqlQuery sqlQuery;
    QString sqlCommand = "SELECT * FROM `discount`;";
    sqlQuery.prepare(sqlCommand);
    if (!sqlQuery.exec())
    {
        qDebug() << "Error: Fail to query table. " << sqlQuery.lastError();
    }
    else
    {
        while (sqlQuery.next())
        {
            vector<double> tmp;
            tmp.push_back(sqlQuery.value(0).toDouble());
            tmp.push_back(sqlQuery.value(1).toDouble());
            tmp.push_back(sqlQuery.value(2).toDouble());
            tmp.push_back(sqlQuery.value(3).toInt());
            discount.push_back(tmp);
        }
    }
    return discount;
}

/* 保存折扣 */
void sqlite::setDiscount(vector<vector<double>> discount) const
{
    QSqlQuery sqlQuery;
    for (int i = 0; i < (int)discount.size(); i++)
    {
        QString createSql = QString("UPDATE `discount` SET `fooddiscount`=:fooddiscount, `clothesdiscount`=:clothesdiscount, `bookdiscount`=:bookdiscount WHERE `seller`=:seller;");
        sqlQuery.prepare(createSql);
        sqlQuery.bindValue(":seller", discount[i][3]);
        sqlQuery.bindValue(":fooddiscount", discount[i][0]);
        sqlQuery.bindValue(":clothesdiscount", discount[i][1]);
        sqlQuery.bindValue(":bookdiscount", discount[i][2]);
        if (!sqlQuery.exec())
        {
            qDebug() << "Error: Fail to create table. " << sqlQuery.lastError();
        }
        else
        {
            qDebug() << "Table created!";
        }
    }
}

/* 关闭数据库 */
void sqlite::closeDb(void)
{
    db.close();
}
