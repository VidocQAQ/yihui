#ifndef CLIENT_RPC_H
#define CLIENT_RPC_H

#include <QCoreApplication>
#include <QDebug>
#include <QVsoa>
#include <QJsonDocument>

using namespace std::placeholders;

// 全局变量声明
extern QString latestDHT11;
extern QString latestUSS;
extern QString latestADC;
extern bool hasDHT11;
extern bool hasUSS;
extern bool hasADC;
extern int latestPotValueC;
extern QString pDHT11temp;
extern QString pDHT11humi;
extern QString pUSS;
extern QString pADC;

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
void lightshowon(QVsoaClient *client);
void lightshowoff(QVsoaClient *client);
//3.查询状态
int isled_pwmOn(QVsoaClient *client);


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
void breathon(QVsoaClient *client, int brightness);
void breathoff(QVsoaClient *client);

//3.查询单色LED状态
QJsonObject getMonoLedStatus(QVsoaClient *client);
QJsonObject getSingleMonoLedStatus(QVsoaClient *client, const QString &color);


//OLED相关功能
//1.显示文字
void displaytext(QVsoaClient *client, QString text);
//2.清屏
void clearoled(QVsoaClient *client);

// 蜂鸣器相关功能
void buzzeron(QVsoaClient *client);
void buzzeroff(QVsoaClient *client);
void buzzersongon(QVsoaClient *client);
void buzzersongoff(QVsoaClient *client);

#endif // CLIENT_RPC_H
