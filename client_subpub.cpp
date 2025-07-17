#include "client_subpub.h"
#include <QDebug>
#include "client_rpc.h"
#include <QRandomGenerator>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QJsonParseError>

void onMessage(QVsoaClient *clientconst, QString url, const QVsoaPayload payload){
     if(url == "/sensor/dht11/data"){
            QString param = payload.param();
            QJsonParseError parseError;
            QJsonDocument doc = QJsonDocument::fromJson(param.toUtf8(), &parseError);
            if (parseError.error == QJsonParseError::NoError && doc.isObject()) {
                QJsonObject obj = doc.object();
                double temperature = obj.value("temperature").toDouble();
                int humidity = obj.value("humidity").toInt();
                latestDHT11 = QString("temperature:%1℃\nhumidity:%2%%").arg(temperature).arg(humidity);
                if(hasDHT11&&!hasUSS){
                displaytext(clientconst, latestDHT11);
                qDebug() << "DHT11 :" << payload.param();
                }
            }
        }
        else if(url == "/sensor/uss/data"){
            QString param = payload.param();
            QJsonParseError parseError;
            QJsonDocument doc = QJsonDocument::fromJson(param.toUtf8(), &parseError);
            if (parseError.error == QJsonParseError::NoError && doc.isObject()) {
                QJsonObject obj = doc.object();
                double distance = obj.value("distance").toDouble();
                latestUSS = QString("distance:%1cm").arg(distance);
                if(hasUSS&&!hasDHT11){
                displaytext(clientconst, latestUSS);
                qDebug() << "USS :" << distance;
                }
            }
        }
        else{
            qDebug() << "ERROR" ;
        }
        if(hasDHT11&&hasUSS){
            QString combined = QString("%1\n%2").arg(latestDHT11).arg(latestUSS);
            displaytext(clientconst, combined);
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
