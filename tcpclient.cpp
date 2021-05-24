#include "tcpclient.h"

TcpClient::TcpClient(QObject *parent) : QObject(parent)
{

}


int TcpClient::connectToServer()
{
    tcpSocket = new QTcpSocket();
    tcpSocket->connectToHost(HOST, PORT);
    if(!tcpSocket->waitForConnected(1000))
    {
        qDebug() << "Connection failed!" ;
        return 1;
    }
    qDebug() << "Connect successfully!";
    return 0;
}

void TcpClient::sendData(QByteArray data)
{
    if (tcpSocket==nullptr)
    {
        connectToServer();
    }
    tcpSocket->write(data);
}

QByteArray TcpClient::getData(QByteArray data, int msec, bool whileOrIf) // while true, if false
{
    if (tcpSocket==nullptr)
    {
        connectToServer();
    }
    tcpSocket->write(data);
    QByteArray buffer;
    if (tcpSocket->waitForReadyRead(msec))
    {
        //读取缓冲区数据
        buffer.append(tcpSocket->readAll());
    }
    if (whileOrIf)
    {
        while(tcpSocket->waitForReadyRead(10))
        {
            //读取缓冲区数据
            buffer.append(tcpSocket->readAll());
        }
    }
    return buffer;
}

void TcpClient::disconnectFromServer()
{
    if (tcpSocket != nullptr)
    {
        tcpSocket->disconnectFromHost();
        delete tcpSocket;
    }
}
