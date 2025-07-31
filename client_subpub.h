#include <QCoreApplication>
#include <QDebug>
#include <QVsoa>
#include <QJsonArray>
#include <QJsonDocument>

using namespace std::placeholders;
extern int latestPotDacBrightnessPercent;
extern QVector<QPair<double, double>> lidarPoints;

void onMessage(QVsoaClient *client, const QString url, const QVsoaPayload payload);
void onDatagram(QVsoaClient *client, const QString url, const QVsoaPayload payload);

void DHT11on(QVsoaClient *client);
void DHT11off(QVsoaClient *client);

void USSon(QVsoaClient *client);
void USSoff(QVsoaClient *client);

void ADCon(QVsoaClient *client);
void ADCoff(QVsoaClient *client);

void LidarControlOn(QVsoaClient *client);
void LidarControlOff(QVsoaClient *client);



