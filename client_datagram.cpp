#include "client_datagram.h"
#include "client_rpc.h"
#include <QJsonParseError>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonValue>
#include <QDebug>

QVector<QPair<double, double>> lidarPoints;

void onDatagram(QVsoaClient *client, const QString url, const QVsoaPayload payload)
{
    if (url == "/lidar/data") {
        QString param = payload.param();
        QJsonParseError parseError;
        QJsonDocument doc = QJsonDocument::fromJson(param.toUtf8(), &parseError);

        if (parseError.error != QJsonParseError::NoError) {
            qDebug() << "LidarControlSet JSON parse error:" << parseError.errorString() 
                     << "at offset:" << parseError.offset;
            qDebug() << "Raw param:" << param;
            return;
        }
        
        if (doc.isObject()) {
            QJsonObject obj = doc.object();
            // 解析st1.st2
            QVector<QPair<double, double>> points;
            if (obj.contains("st1") && obj.value("st1").isObject()) {
                QJsonObject st1Obj = obj.value("st1").toObject();
                if (st1Obj.contains("st2") && st1Obj.value("st2").isArray()) {
                    QJsonArray st2Array = st1Obj.value("st2").toArray();
                    for (const QJsonValue &val : st2Array) {
                        if (val.isObject()) {
                            QJsonObject pointObj = val.toObject();
                            double angle = pointObj.value("angle").toDouble();
                            double distance = pointObj.value("distance").toDouble();
                            points.append(qMakePair(angle, distance));
                        }
                    }
                }
            }
            lidarPoints = points;
        }
    }
}

void LidarControlOn(QVsoaClient *client)
{
    auto invoker = new QVsoaClientRPCInvoker(client, "/lidar/on", RPCMethod::SET);
    invoker->call(QVsoaPayload{});
    
    client->subscribe("/lidar/data");
    client->autoConsistent({"/lidar/data"}, 500);
}

void LidarControlOff(QVsoaClient *client)
{
    bool success = client->unsubscribe("/lidar/data");
    if (success) {
        qDebug() << "取消订阅 LidarControl";
    } else {
        qDebug() << "取消订阅失败，可能未订阅该主题或网络异常";
    }
    
    auto invoker = new QVsoaClientRPCInvoker(client, "/lidar/off", RPCMethod::SET);
    invoker->call(QVsoaPayload{});
    delete invoker;  // 修复内存泄漏
}
