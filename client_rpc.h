#ifndef CLIENT_RPC_H
#define CLIENT_RPC_H

#include <QCoreApplication>
#include <QDebug>
#include <QVsoa>
#include <QJsonDocument>

using namespace std::placeholders;

// 函数声明
void onConnected(bool ok, QString info);
void onDisconnected();
void onReplay(QVsoaClientRPCInvoker *invoker, const QVsoaHeader header, const QVsoaPayload payload);

//led_pwm相关功能
//
//1.rainbow模式
void rainbowon(QVsoaClient *client);
void rainbowoff(QVsoaClient *client);
//2.lightshow模式
void lightshowon(QVsoaClient *client, int ledIndex);
void lightshowoff(QVsoaClient *client);


//led_mono相关功能
//1.单色灯
void redmonoon(QVsoaClient *client);
void redmonooff(QVsoaClient *client);
void yellowmonoon(QVsoaClient *client);
void yellowmonooff(QVsoaClient *client);
void greenmonoon(QVsoaClient *client);
void greenmonooff(QVsoaClient *client);
void bluemonoon(QVsoaClient *client);
void bluemonooff(QVsoaClient *client);
//2.呼吸灯
void breathon(QVsoaClient *client);
void breathoff(QVsoaClient *client);
#endif // CLIENT_RPC_H
