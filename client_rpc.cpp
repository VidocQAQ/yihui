#include "client_rpc.h"
#include <QRandomGenerator>

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
        qDebug() << "Command /light error!";
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


//led_mono
//单色灯光开关
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