#include "client_subpub.h"
#include <QDebug>
#include "client_rpc.h"
#include <QRandomGenerator>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QJsonParseError>
#include <QJsonArray>
#include <QVector>
#include <QPair>

QVector<QPair<double, double>> lidarPoints;

int latestPotDacBrightnessPercent = 100;

void onMessage(QVsoaClient *clientconst, QString url, const QVsoaPayload payload){
    if(url == "/sensor/dht11/data"){
        QString param = payload.param();
        QJsonParseError parseError;
        QJsonDocument doc = QJsonDocument::fromJson(param.toUtf8(), &parseError);
        if (parseError.error == QJsonParseError::NoError && doc.isObject()) {

            QJsonObject obj = doc.object();
            if (obj.value("status").toString() == "ERROR") {
                // 只用上一次的 latestDHT11

            }else{
            double temperature = obj.value("temperature").toDouble();
            int humidity = obj.value("humidity").toInt();
            if(temperature == 0 || humidity == 0){

            }else{
            latestDHT11 = QString("temp : %1C\nhumi : %2%").arg(temperature).arg(humidity);
            pDHT11temp = QString::number(temperature);
            pDHT11humi = QString::number(humidity);
            if(hasDHT11&&!hasUSS&&!hasADC){
            displaytext(clientconst, latestDHT11);
            //qDebug() << "DHT11 :" << payload.param();
            }
           }
        }
      }
    }
    else if(url == "/sensor/uss/data"){
        QString param = payload.param();
        QJsonParseError parseError;
        QJsonDocument doc = QJsonDocument::fromJson(param.toUtf8(), &parseError);
        if (parseError.error == QJsonParseError::NoError && doc.isObject()) {
            QJsonObject obj = doc.object();
            if (obj.value("status").toString() == "ERROR") {
                // 只用上一次的 latestUSS
            }else{
            double distance = obj.value("distance").toDouble();
            latestUSS = QString("dist : %1cm").arg(distance);
            pUSS = QString::number(distance);
            if(hasUSS&&!hasDHT11&&!hasADC){
            displaytext(clientconst, latestUSS);
            qDebug() << "USS :" << distance;
            }
        }
        }
    }
        else if(url == "/adc/data"){
            QString param=payload.param();
            QJsonParseError parseError;
            QJsonDocument doc = QJsonDocument::fromJson(param.toUtf8(), &parseError);
            if (parseError.error == QJsonParseError::NoError && doc.isObject()) {
                QJsonObject obj = doc.object();
                if (obj.value("status").toString() == "ERROR") {
                    // 只用上一次的 latestADC
                }else{  
                QJsonObject photosensitive = obj.value("photosensitive").toObject();
                int valuec0 = photosensitive.value("valuec0").toInt();
                latestADC = QString("brig : %1").arg(valuec0);
                pADC = QString::number(valuec0);
                QJsonObject potentiometer = obj.value("potentiometer").toObject();
                int valuec = potentiometer.value("valuec").toInt();
                latestPotValueC = valuec;
                
                // 修正：从obj获取dac_brightness_percent
                int dac_brightness_percent = obj.value("dac_brightness_percent").toInt();
                latestPotDacBrightnessPercent = dac_brightness_percent;
                if(hasADC && !hasDHT11 && !hasUSS){
                    displaytext(clientconst, latestADC);
                    qDebug() << "ADC :" << valuec0;
                }
            }
            }
        }
        else{
            qDebug() << "ERROR" ;
        }
        if(hasDHT11 && hasUSS && !hasADC){
            QString combined = QString("%1\n%2").arg(latestDHT11).arg(latestUSS);
            displaytext(clientconst, combined);
        }
        else if(hasDHT11 && hasADC && !hasUSS){
            QString combined = QString("%1\n%2").arg(latestDHT11).arg(latestADC);
            displaytext(clientconst, combined);
        }
        else if(hasUSS && hasADC && !hasDHT11){
            QString combined = QString("%1\n%2").arg(latestUSS).arg(latestADC);
            displaytext(clientconst, combined);
        }
        else if(hasDHT11 && hasUSS && hasADC){
            QString combined = QString("%1\n%2\n%3").arg(latestDHT11).arg(latestUSS).arg(latestADC);
            displaytext(clientconst, combined);
        }
    
}

void onDatagram(QVsoaClient *client, const QString url, const QVsoaPayload payload){
    if(url == "/lidar/data"){
            QString param = payload.param();
            QJsonParseError parseError;
            QJsonDocument doc = QJsonDocument::fromJson(param.toUtf8(), &parseError);

            if (parseError.error != QJsonParseError::NoError) {
                qDebug() << "LidarControlSet JSON parse error:" << parseError.errorString() << "at offset:" << parseError.offset;
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

void DHT11on(QVsoaClient *client){
    client->subscribe("/sensor/dht11/data");
    client->autoConsistent({"/sensor/dht11/data"}, 1000);
    hasDHT11 = true;
}

void DHT11off(QVsoaClient *client){
    bool success = client->unsubscribe("/sensor/dht11/data");
    hasDHT11 = false;
    clearoled(client);
    if (success) {
          qDebug() << "取消订阅 DHT11";
    } else {
          qDebug() << "取消订阅失败，可能未订阅该主题或网络异常";
    }
}



void USSon(QVsoaClient *client){
    client->subscribe("/sensor/uss/data");
    client->autoConsistent({"/sensor/uss/data"}, 1000);
    hasUSS = true;
}

void USSoff(QVsoaClient *client){
    bool success = client->unsubscribe("/sensor/uss/data");
    hasUSS = false;
    clearoled(client);
    if (success) {
          qDebug() << "取消订阅 USS";
    } else {
          qDebug() << "取消订阅失败，可能未订阅该主题或网络异常";
    }
}

void ADCon(QVsoaClient *client){
    client->subscribe("/adc/data");
    client->autoConsistent({"/adc/data"}, 1000);
    hasADC = true;
}
void ADCoff(QVsoaClient *client){
    bool success = client->unsubscribe("/adc/data");
    hasADC = false;
    clearoled(client);
    if (success) {
          qDebug() << "取消订阅 ADC";
    } else {
          qDebug() << "取消订阅失败，可能未订阅该主题或网络异常";
    }
}


void LidarControlOn(QVsoaClient *client){
    auto invoker = new QVsoaClientRPCInvoker(client, "/lidar/on", RPCMethod::SET);
    invoker->call(QVsoaPayload{});
    client->subscribe("/lidar/data");
    client->autoConsistent({"/lidar/data"}, 500);
}
void LidarControlOff(QVsoaClient *client){
    bool success = client->unsubscribe("/lidar/data");
    if (success) {
          qDebug() << "取消订阅 LidarControl";
    } else {
          qDebug() << "取消订阅失败，可能未订阅该主题或网络异常";
    }
    auto invoker = new QVsoaClientRPCInvoker(client, "/lidar/off", RPCMethod::SET);
    invoker->call(QVsoaPayload{});
}
//
