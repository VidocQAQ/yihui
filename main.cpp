#include "mainwindow.h"
#include <QApplication>
#include  "client_rpc.h"
#include <QCoreApplication>
#include <QDebug>
#include <QVsoa>
constexpr char SERVER_PASSWORD[] = "123456";
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();


    // Initilize client
        QVsoaClient client;
        if (client.isInvalid()) {
            qDebug() << "Can not create VSOA client!";
            return -1;
        }
        //把连接、断开连接等与相关响应处理函数关联
        QObject::connect(&client, &QVsoaClient::connected, onConnected);
        QObject::connect(&client, &QVsoaClient::disconnected, onDisconnected);
        QObject::connect(&client, &QVsoaClient::connected, std::bind(lightCall, &client));
        //添加额外功能


        // 把客户端与服务端连接起来
        client.connect2server("vsoa://127.0.0.1:5600", SERVER_PASSWORD);
        // 设置的自动连接
        client.autoConnect(1000, 500);



    return a.exec();
}
