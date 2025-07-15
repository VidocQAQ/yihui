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
void lightCall(QVsoaClient *client);
void lightOffCall(QVsoaClient *client);

#endif // CLIENT_RPC_H
