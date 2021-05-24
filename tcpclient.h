#ifndef TCPCLIENT_H
#define TCPCLIENT_H

#define SQLITE_singleInsertData 0
#define SQLITE_modifyItemInCart 1
#define SQLITE_deleteItemFromCart 2
#define SQLITE_queryCart 3
#define SQLITE_queryTable 4
#define SQLITE_modifyData 5
#define SQLITE_deleteData 6
#define SQLITE_newDiscount 7
#define SQLITE_setDiscount 8
#define SQLITE_generateOrder 9
#define SQLITE_getOrder 10
#define SQLITE_getOrderList 11
#define USER_createUser 12
#define USER_changeUserName 13
#define USER_loginCheck 14
#define USER_getUser 15
#define pay 16
#define SQLITE_getDiscount 17
#define SQLITE_buyOneThing 18
#define USER_recharge 19
#define USER_changePassword 20

#define HOST "localhost"
#define PORT 8848

using namespace std;

#include <QObject>
#include <QTcpSocket>
class TcpClient : public QObject
{
    Q_OBJECT
public:
    explicit TcpClient(QObject *parent = nullptr);
    QTcpSocket *tcpSocket = nullptr;
    int connectToServer();
    void disconnectFromServer();
    void sendData(QByteArray data);
    QByteArray getData(QByteArray data, int msec = 50, bool whileOrIf = true);

signals:

};

#endif // TCPCLIENT_H
