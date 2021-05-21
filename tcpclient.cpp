#include "tcpclient.h"

TcpClient::TcpClient(QObject *parent) : QObject(parent)
{

}


void TcpClient::connectToServer()
{
    tcpSocket = new QTcpSocket();
    tcpSocket->connectToHost(HOST, PORT);
    if(!tcpSocket->waitForConnected(30000))
    {
        qDebug() << "Connection failed!" ;
        return;
    }
    qDebug() << "Connect successfully!";
}

void TcpClient::sendData(QByteArray data)
{
    if (tcpSocket==nullptr)
    {
        connectToServer();
    }
    tcpSocket->write(data);
}

void TcpClient::disconnectFromServer()
{
    if (tcpSocket != nullptr)
    {
        tcpSocket->disconnectFromHost();
        delete tcpSocket;
    }
}
