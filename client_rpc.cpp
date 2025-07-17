#include "client_rpc.h"
#include <QRandomGenerator>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QJsonParseError>


// 全局变量定义
QString latestDHT11;
QString latestUSS;
bool hasDHT11 = true;
bool hasUSS = true;

void onConnected(bool ok, QString info)//处理连接成功或者失败的回调
{
    if (!ok) {
        qDebug() << "Connected with server failed!";
        return;
    }
    qDebug() << "Connected with server:" << info;
}

void onDisconnected()//处理连接断开的回调
{
    qDebug() << "Connection break";
}

//异步方法
void onReplay(QVsoaClientRPCInvoker *invoker, const QVsoaHeader header, const QVsoaPayload payload)
{//处理返回信息
    if (header.status() == StatusCode::SUCCESS) {//status==success表示响应成功
        qDebug() << "reply:" << payload.param();
        //输出信息，也可以添加其他操作

    } else {
        qDebug() << "Command error!";
    }
    invoker->deleteLater();
}


//led_pwm相关功能

// rainbow模式
void rainbowon(QVsoaClient *client)//连接成功后，响应，发起调用
{
    // 生成随机RGB颜色
    int r = QRandomGenerator::global()->bounded(256);
    int g = QRandomGenerator::global()->bounded(256);
    int b = QRandomGenerator::global()->bounded(256);
    QString color = QString("%1%2%3")
        .arg(r, 2, 16, QChar('0'))
        .arg(g, 2, 16, QChar('0'))
        .arg(b, 2, 16, QChar('0'));
    color = color.toLower();

    auto invoker = new QVsoaClientRPCInvoker(client, "/ledpwm/set", RPCMethod::SET);//创建请求信息
    QVariantMap param = {
                {"color", color },
                {"brightness",80}
            };

    QVsoaPayload payload(QString::fromUtf8(QJsonDocument::fromVariant(param).toJson()), {});
    QObject::connect(invoker, &QVsoaClientRPCInvoker::serverReply, std::bind(onReplay, invoker, _1, _2));

    invoker->call(payload);//用于发送信息到服务端，其中QVsoaPayload{}可以用于装json信息

}

void rainbowoff(QVsoaClient *client)
{
    auto invoker = new QVsoaClientRPCInvoker(client, "/ledpwm/off", RPCMethod::SET);

    QObject::connect(invoker, &QVsoaClientRPCInvoker::serverReply, std::bind(onReplay, invoker, _1, _2));
    invoker->call(QVsoaPayload{});
} 

//lightshow模式
void lightshowon(QVsoaClient *client, int ledIndex)
{
    // 生成随机RGB颜色
    int r = QRandomGenerator::global()->bounded(256);
    int g = QRandomGenerator::global()->bounded(256);
    int b = QRandomGenerator::global()->bounded(256);
    QString color = QString("%1%2%3")
        .arg(r, 2, 16, QChar('0'))
        .arg(g, 2, 16, QChar('0'))
        .arg(b, 2, 16, QChar('0'));
    color = color.toLower();

    QString path = QString("/ledpwm/%1/set").arg(ledIndex);
    auto invoker = new QVsoaClientRPCInvoker(client, path, RPCMethod::SET);
    QVariantMap param = {
        {"color", color},
        {"brightness", 80}
    };
    QVsoaPayload payload(QString::fromUtf8(QJsonDocument::fromVariant(param).toJson()), {});
    QObject::connect(invoker, &QVsoaClientRPCInvoker::serverReply, std::bind(onReplay, invoker, _1, _2));
    invoker->call(payload);
} 
void lightshowoff(QVsoaClient *client){
    auto invoker = new QVsoaClientRPCInvoker(client, "/ledpwm/off", RPCMethod::SET);

    QObject::connect(invoker, &QVsoaClientRPCInvoker::serverReply, std::bind(onReplay, invoker, _1, _2));
    invoker->call(QVsoaPayload{});
}
//查询灯状态
int isled_pwmOn(QVsoaClient *client){
    QVsoaClientSynchronizer sync(client);
    auto [success, header] = sync.call("/ledpwm/status", RPCMethod::GET, QVsoaPayload{});
    if (success) {
        QString payloadStr = header.payload().param();
        QJsonParseError err;
        QJsonDocument doc = QJsonDocument::fromJson(payloadStr.toUtf8(), &err);
        if (err.error != QJsonParseError::NoError) {
            qDebug() << "JSON parse error:" << err.errorString();
            return -1;
        }
        QJsonObject obj = doc.object();
        QStringList keys = {"red", "yellow", "green", "blue"};
        int onCount = 0, offCount = 0;
        for (const QString& key : keys) {
            if (!obj.contains(key)) return -1;
            QJsonObject led = obj[key].toObject();
            QString state = led["state"].toString();
            if (state == "on") onCount++;
            else if (state == "off") offCount++;
        }
        if (onCount == 4) return 1;
        if (offCount == 4) return 0;
        return -1;
    } else {
        qDebug() << "Synchronous RPC call error!";
        return -1;
    }
}





//
//
//
//
//
//
//led_mono
//1.单色灯光开关
void redmonoon(QVsoaClient *client){
    auto invoker = new QVsoaClientRPCInvoker(client, "/ledmono/red/on", RPCMethod::SET);
    QObject::connect(invoker, &QVsoaClientRPCInvoker::serverReply, std::bind(onReplay, invoker, _1, _2));
    invoker->call(QVsoaPayload{});
}
void redmonooff(QVsoaClient *client){
    auto invoker = new QVsoaClientRPCInvoker(client, "/ledmono/red/off", RPCMethod::SET);
    QObject::connect(invoker, &QVsoaClientRPCInvoker::serverReply, std::bind(onReplay, invoker, _1, _2));
    invoker->call(QVsoaPayload{});
}
void yellowmonoon(QVsoaClient *client){
    auto invoker = new QVsoaClientRPCInvoker(client, "/ledmono/yellow/on", RPCMethod::SET);
    QObject::connect(invoker, &QVsoaClientRPCInvoker::serverReply, std::bind(onReplay, invoker, _1, _2));
    invoker->call(QVsoaPayload{});
}
void yellowmonooff(QVsoaClient *client){
    auto invoker = new QVsoaClientRPCInvoker(client, "/ledmono/yellow/off", RPCMethod::SET);
    QObject::connect(invoker, &QVsoaClientRPCInvoker::serverReply, std::bind(onReplay, invoker, _1, _2));
    invoker->call(QVsoaPayload{});
}
void greenmonoon(QVsoaClient *client){
    auto invoker = new QVsoaClientRPCInvoker(client, "/ledmono/green/on", RPCMethod::SET);
    QObject::connect(invoker, &QVsoaClientRPCInvoker::serverReply, std::bind(onReplay, invoker, _1, _2));
    invoker->call(QVsoaPayload{});
}
void greenmonooff(QVsoaClient *client){
    auto invoker = new QVsoaClientRPCInvoker(client, "/ledmono/green/off", RPCMethod::SET);
    QObject::connect(invoker, &QVsoaClientRPCInvoker::serverReply, std::bind(onReplay, invoker, _1, _2));
    invoker->call(QVsoaPayload{});
}
void bluemonoon(QVsoaClient *client){
    auto invoker = new QVsoaClientRPCInvoker(client, "/ledmono/blue/on", RPCMethod::SET);
    QObject::connect(invoker, &QVsoaClientRPCInvoker::serverReply, std::bind(onReplay, invoker, _1, _2));
    invoker->call(QVsoaPayload{});
}
void bluemonooff(QVsoaClient *client){
    auto invoker = new QVsoaClientRPCInvoker(client, "/ledmono/blue/off", RPCMethod::SET);
    QObject::connect(invoker, &QVsoaClientRPCInvoker::serverReply, std::bind(onReplay, invoker, _1, _2));
    invoker->call(QVsoaPayload{});
}
//2.呼吸灯，红色为例
void breathon(QVsoaClient *client, int brightness){
    auto invoker = new QVsoaClientRPCInvoker(client, "/ledmono/red/brightness", RPCMethod::SET);
    QObject::connect(invoker, &QVsoaClientRPCInvoker::serverReply, std::bind(onReplay, invoker, _1, _2));

    QVariantMap param = {
        {"brightness", brightness}
    };
    QVsoaPayload payload(QString::fromUtf8(QJsonDocument::fromVariant(param).toJson()), {});
    invoker->call(payload);
}
void breathoff(QVsoaClient *client){
    auto invoker = new QVsoaClientRPCInvoker(client, "/ledmono/off", RPCMethod::SET);
    QObject::connect(invoker, &QVsoaClientRPCInvoker::serverReply, std::bind(onReplay, invoker, _1, _2));
    invoker->call(QVsoaPayload{});
}

//OLED相关功能
//1.显示文字
void displaytext(QVsoaClient *client, QString text){
    auto invoker = new QVsoaClientRPCInvoker(client, "/oled/display", RPCMethod::SET);
    QVariantMap param = {
        {"text", text}
    };
    QVsoaPayload payload(QString::fromUtf8(QJsonDocument::fromVariant(param).toJson()), {});
    QObject::connect(invoker, &QVsoaClientRPCInvoker::serverReply, std::bind(onReplay, invoker, _1, _2));
    invoker->call(payload);
}
//2.清屏
void clearoled(QVsoaClient *client){
    auto invoker = new QVsoaClientRPCInvoker(client, "/oled/clear", RPCMethod::SET);
    QObject::connect(invoker, &QVsoaClientRPCInvoker::serverReply, std::bind(onReplay, invoker, _1, _2));
    invoker->call(QVsoaPayload{});
}


// 蜂鸣器相关功能
void buzzeron(QVsoaClient *client) {
    auto invoker = new QVsoaClientRPCInvoker(client, "/buzzer/on", RPCMethod::SET);
    QObject::connect(invoker, &QVsoaClientRPCInvoker::serverReply, std::bind(onReplay, invoker, _1, _2));
    invoker->call(QVsoaPayload{});
}

void buzzeroff(QVsoaClient *client) {
    auto invoker = new QVsoaClientRPCInvoker(client, "/buzzer/off", RPCMethod::SET);
    QObject::connect(invoker, &QVsoaClientRPCInvoker::serverReply, std::bind(onReplay, invoker, _1, _2));
    invoker->call(QVsoaPayload{});
}

void buzzersongon(QVsoaClient *client) {
    auto invoker = new QVsoaClientRPCInvoker(client, "/buzzer/songon", RPCMethod::SET);
    QObject::connect(invoker, &QVsoaClientRPCInvoker::serverReply, std::bind(onReplay, invoker, _1, _2));
    invoker->call(QVsoaPayload{});
}

void buzzersongoff(QVsoaClient *client) {
    auto invoker = new QVsoaClientRPCInvoker(client, "/buzzer/songoff", RPCMethod::SET);
    QObject::connect(invoker, &QVsoaClientRPCInvoker::serverReply, std::bind(onReplay, invoker, _1, _2));
    invoker->call(QVsoaPayload{});
}

