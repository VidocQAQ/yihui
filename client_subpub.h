#include <QCoreApplication>
#include <QDebug>
#include <QVsoa>
#include <QJsonDocument>

using namespace std::placeholders;

void onMessage(QVsoaClient *client, const QString url, const QVsoaPayload payload);

void DHT11on(QVsoaClient *client);
void DHT11off(QVsoaClient *client);

void USSon(QVsoaClient *client);
void USSoff(QVsoaClient *client);

void ADCon(QVsoaClient *client);
void ADCoff(QVsoaClient *client);
