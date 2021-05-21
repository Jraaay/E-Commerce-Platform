#ifndef TCPCLIENT_H
#define TCPCLIENT_H

#define HOST "localhost"
#define PORT 8848

#include <QObject>
#include <QTcpSocket>
class TcpClient : public QObject
{
    Q_OBJECT
public:
    explicit TcpClient(QObject *parent = nullptr);
    QTcpSocket *tcpSocket = nullptr;
    void connectToServer();
    void disconnectFromServer();
    void sendData(QByteArray data);

signals:

};

#endif // TCPCLIENT_H
