#include <QCoreApplication>
#include <QDebug>
#include <QVsoa>
#include <QJsonArray>
#include <QJsonDocument>
using namespace std::placeholders;


extern QVector<QPair<double, double>> lidarPoints;

// datagram处理函数
void onDatagram(QVsoaClient *client, const QString url, const QVsoaPayload payload);

// 激光雷达相关功能
void LidarControlOn(QVsoaClient *client);
void LidarControlOff(QVsoaClient *client);

