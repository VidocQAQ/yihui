#include "client_rpc.h"

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

void lightCall(QVsoaClient *client)//连接成功后，响应，发起调用
{
    auto invoker = new QVsoaClientRPCInvoker(client, "/ledpwm/set", RPCMethod::SET);//创建请求信息
    QVariantMap param = {
                {"color","07e000" },
                {"brightness",80}
            };

    QVsoaPayload payload(QString::fromUtf8(QJsonDocument::fromVariant(param).toJson()), {});
    QObject::connect(invoker, &QVsoaClientRPCInvoker::serverReply, std::bind(onReplay, invoker, _1, _2));

    invoker->call(payload);//用于发送信息到服务端，其中QVsoaPayload{}可以用于装json信息

} 