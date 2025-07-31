#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QRegularExpression>
#include <QPushButton>
#include <QColor>
#include <QRandomGenerator>
#include <QVsoa>
#include <QDebug>
#include <QTimer>
#include <QDateTime>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonParseError>
#include <QJsonArray>
#include <QDialog>
#include "motorcontroldialog.h"
#include "servocontroldialog.h"
#include "lidarcontroldialog.h"


constexpr char SERVER_PASSWORD[] = "123456";
const QString DHT11_URL = "/sensor/dht11/data";
const QString USS_URL = "/sensor/uss/data";

int count=0;
int count1=0;
int m_lightshowLedIndex = 1; //当前闪烁的LED编号
MainWindow::MainWindow(QWidget *parent)
    : QWidget(parent)
       , ui(new Ui::MainWindow)
       , m_client(nullptr)
       , m_lightshowTimer(new QTimer(this))
   {
       ui->setupUi(this);



       // 设置初始文本
       ui->textDisplay->setText(QString("HELLO,BAOZI!\n"));
       // 初始化灯状态
       initLamps();

       // 初始化QVsoaClient连接
       initVsoaClient();
       // 灯光秀定时器初始化
       m_lightshowTimer->setInterval(100); // 100ms变色一次，更慢更赏心悦目
       connect(m_lightshowTimer, &QTimer::timeout, this, [this]() {
           if (m_client && m_client->isConnected()) {
               lightshowon(m_client);
           }
       });
       // 新增：初始化电位器同步定时器
       m_potSyncTimer = new QTimer(this);
       m_potSyncTimer->setInterval(100); // 1秒
       connect(m_potSyncTimer, &QTimer::timeout, this, &MainWindow::syncDialWithPotValue);
       m_potSyncTimer->start();
       
       // 新增：初始化LED状态同步定时器
       m_ledStatusTimer = new QTimer(this);
       m_ledStatusTimer->setInterval(100); // 每0.1秒同步一次LED状态
       connect(m_ledStatusTimer, &QTimer::timeout, this, &MainWindow::syncLedStatus);
       m_ledStatusTimer->start();
       
       // 新增：初始化呼吸灯UI特效定时器
       m_breathUITimer = new QTimer(this);
       m_breathUITimer->setInterval(100); // 100ms更新一次UI呼吸效果
       connect(m_breathUITimer, &QTimer::timeout, this, &MainWindow::updateBreathUIEffect);
       // 连接电机控制按钮
       connect(ui->btnMotorControl, &QPushButton::clicked, this, &MainWindow::on_btnMotorControl_clicked);
       connect(ui->btnLidar, &QPushButton::clicked, this, &MainWindow::on_btnLidar_clicked);
   }

MainWindow::~MainWindow()
{
    // 停止所有定时器
    if (m_lightshowTimer) {
        m_lightshowTimer->stop();
        delete m_lightshowTimer;
    }
    if (m_breathTimer) {
        m_breathTimer->stop();
        delete m_breathTimer;
    }
    if (m_potSyncTimer) {
        m_potSyncTimer->stop();
        delete m_potSyncTimer;
    }
    if (m_ledStatusTimer) {
        m_ledStatusTimer->stop();
        delete m_ledStatusTimer;
    }
    if (m_breathUITimer) {
        m_breathUITimer->stop();
        delete m_breathUITimer;
    }
    
    if (m_client) {
        m_client->disconnect();
        delete m_client;
    }
    delete ui;
}

void MainWindow::initLamps()
{
    // 设置灯的初始状态
    QList<QLabel*> lamps = {
        ui->lampRed,
        ui->lampYellow,
        ui->lampGreen,
        ui->lampBlue
    };

    QList<QColor> colors = {
        Qt::red,
        Qt::yellow,
        Qt::green,
        Qt::blue
    };

    for (int i = 0; i < lamps.size(); i++) {
        QLabel *lamp = lamps[i];
        lamp->setProperty("on", false);
        lamp->setProperty("originalColor", colors[i].name());
        lamp->setStyleSheet("background-color: #808080; border-radius: 15px;");
    }
}

void MainWindow::toggleLamp(QLabel *lamp)
{
    bool isOn = lamp->property("on").toBool();
    QString originalColor = lamp->property("originalColor").toString();

    if (isOn) {
        lamp->setStyleSheet("background-color: gray; border-radius: 15px;");
    } else {
        lamp->setStyleSheet(QString("background-color: %1; border-radius: 15px;").arg(originalColor));
    }

    lamp->setProperty("on", !isOn);
}

void MainWindow::cleartext(){
    count++;
    if(count==1){
      ui->textDisplay->clear();
      count=0;
    }
}
void MainWindow::cleartext1(){
    count1++;
    if(count1==2){
      ui->textDisplay_2->clear();
      count1=0;
    }
}

void MainWindow::on_btnRed_clicked()
{
    cleartext();
    // 如果呼吸灯开着，先关闭呼吸灯
    if (isBreathOn) {
        on_btnMotorOff_clicked();
    }
    if (!isRedOn) {
        redmonoon(m_client); // 传入 m_client
        isRedOn = true;
        ui->textDisplay->append("红灯已打开");
        toggleLamp(ui->lampRed);
    } else {
        redmonooff(m_client); // 传入 m_client
        isRedOn = false;
        ui->textDisplay->append("红灯已关闭");
        toggleLamp(ui->lampRed);
    }
    
    // 延迟同步状态，确保命令执行完成
    QTimer::singleShot(200, this, static_cast<void (MainWindow::*)()>(&MainWindow::updateLedStatusDisplay));
}

void MainWindow::on_btnYellow_clicked()
{
    cleartext();
    // 如果呼吸灯开着，先关闭呼吸灯
    if (isBreathOn) {
        on_btnMotorOff_clicked();
    }
    if (!isYellowOn) {
        yellowmonoon(m_client);
        isYellowOn = true;
        ui->textDisplay->append("黄灯已打开");
        toggleLamp(ui->lampYellow);
    } else {
        yellowmonooff(m_client);
        isYellowOn = false;
        ui->textDisplay->append("黄灯已关闭");
        toggleLamp(ui->lampYellow);
    }
    
    // 延迟同步状态，确保命令执行完成
    QTimer::singleShot(200, this, static_cast<void (MainWindow::*)()>(&MainWindow::updateLedStatusDisplay));
}

void MainWindow::on_btnGreen_clicked()
{
    cleartext();
    // 如果呼吸灯开着，先关闭呼吸灯
    if (isBreathOn) {
        on_btnMotorOff_clicked();
    }
    if (!isGreenOn) {
        greenmonoon(m_client);
        isGreenOn = true;
        ui->textDisplay->append("绿灯已打开");
        toggleLamp(ui->lampGreen);
    } else {
        greenmonooff(m_client);
        isGreenOn = false;
        ui->textDisplay->append("绿灯已关闭");
        toggleLamp(ui->lampGreen);
    }
    
    // 延迟同步状态，确保命令执行完成
    QTimer::singleShot(200, this, static_cast<void (MainWindow::*)()>(&MainWindow::updateLedStatusDisplay));
}

void MainWindow::on_btnBlue_clicked()
{
    cleartext();
    // 如果呼吸灯开着，先关闭呼吸灯
    if (isBreathOn) {
        on_btnMotorOff_clicked();
    }
    if (!isBlueOn) {
        bluemonoon(m_client);
        isBlueOn = true;
        ui->textDisplay->append("蓝灯已打开");
        toggleLamp(ui->lampBlue);
    } else {
        bluemonooff(m_client);
        isBlueOn = false;
        ui->textDisplay->append("蓝灯已关闭");
        toggleLamp(ui->lampBlue);
    }
    
    // 延迟同步状态，确保命令执行完成
    QTimer::singleShot(200, this, static_cast<void (MainWindow::*)()>(&MainWindow::updateLedStatusDisplay));
}

void MainWindow::on_btnBrightnessSensor_clicked()
{
    buttonToggleStates["btnBrightnessSensor"] = !buttonToggleStates.value("btnBrightnessSensor", false);
    toggleButtonColor("btnBrightnessSensor", buttonToggleStates["btnBrightnessSensor"]);
    if (m_client && m_client->isConnected()) {
        if (!isADCOn) {
            ADCon(m_client);
            ui->textDisplay->append("亮度传感器：开启");
            isADCOn = true;
        } else {
            ADCoff(m_client);
            isADCOn = false;
            removeSensorValue("亮度 ：");
            ui->textDisplay->append("亮度传感器：关闭");

        }
    } else {
        ui->textDisplay->append("错误：VSOA客户端未连接，无法发送命令");
    }
}

void MainWindow::on_btnUltrasonic_clicked()
{
    buttonToggleStates["btnUltrasonic"] = !buttonToggleStates.value("btnUltrasonic", false);
    toggleButtonColor("btnUltrasonic", buttonToggleStates["btnUltrasonic"]);
    if (m_client && m_client->isConnected()) {
        if (!isUSSOn) {
            USSon(m_client);
            ui->textDisplay->append("超声波测距：开启");
            isUSSOn = true;
        } else {
            USSoff(m_client);
            isUSSOn = false;
            removeSensorValue("距离 ：");
            ui->textDisplay->append("超声波测距：关闭");

        }
    } else {
        ui->textDisplay->append("错误：VSOA客户端未连接，无法发送命令");
    }
}

void MainWindow::on_btnDht11_clicked()
{
    buttonToggleStates["btnDht11"] = !buttonToggleStates.value("btnDht11", false);
    toggleButtonColor("btnDht11", buttonToggleStates["btnDht11"]);
    if (m_client && m_client->isConnected()) {
        if (!isDHT11On) {
            DHT11on(m_client);
            ui->textDisplay->append("温湿度传感器：开启");
             isDHT11On = true;
        } else {
            DHT11off(m_client);
            isDHT11On = false;
            removeSensorValue("温度 ：");
            removeSensorValue("湿度 ：");
            ui->textDisplay->append("温湿度传感器：关闭");
        }
    } else {
        ui->textDisplay->append("错误：VSOA客户端未连接，无法发送命令");
    }
    
}

void MainWindow::on_btnBuzzerOn_clicked()
{
    cleartext();
    buttonToggleStates["btnBuzzerOn"] = !buttonToggleStates.value("btnBuzzerOn", false);
    toggleButtonColor("btnBuzzerOn", buttonToggleStates["btnBuzzerOn"]);
    if (!isBuzzerOn) {
        buzzeron(m_client);
        isBuzzerOn = true;
        ui->textDisplay->append("蜂鸣器已开启");
    } else {
        buzzeroff(m_client);
        isBuzzerOn = false;
        ui->textDisplay->append("蜂鸣器已关闭");
    }
}

void MainWindow::on_btnBuzzerSongon_clicked()
{
    cleartext();
    buttonToggleStates["btnBuzzerSongon"] = !buttonToggleStates.value("btnBuzzerSongon", false);
    toggleButtonColor("btnBuzzerSongon", buttonToggleStates["btnBuzzerSongon"]);
    if (!isBuzzerSongOn) {
        buzzersongon(m_client);
        isBuzzerSongOn = true;
        ui->textDisplay->append("蜂鸣器唱歌已开始");
    } else {
        buzzersongoff(m_client);
        isBuzzerSongOn = false;
        ui->textDisplay->append("蜂鸣器唱歌已停止");
    }
}


void MainWindow::on_btnMotorOn_clicked()
{
    cleartext();
    if (!isBreathOn) {
        // 启动呼吸灯时，先改变UI上四个灯的颜色
        if (!isRedOn) {
            toggleLamp(ui->lampRed);
            isRedOn = true;
        }
        if (!isYellowOn) {
            toggleLamp(ui->lampYellow);
            isYellowOn = true;
        }
        if (!isGreenOn) {
            toggleLamp(ui->lampGreen);
            isGreenOn = true;
        }
        if (!isBlueOn) {
            toggleLamp(ui->lampBlue);
            isBlueOn = true;
        }
        // 启动呼吸灯
        if (!m_breathTimer) {
            m_breathTimer = new QTimer(this);
            connect(m_breathTimer, &QTimer::timeout, this, [this]() {
                if (!m_client || !m_client->isConnected()) return;
                // 调整亮度
                if (m_breathIncreasing) {
                    m_breathBrightness += 5;
                    if (m_breathBrightness >= 255) {
                        m_breathBrightness = 255;
                        m_breathIncreasing = false;
                    }
                } else {
                    m_breathBrightness -= 5;
                    if (m_breathBrightness <= 0) {
                        m_breathBrightness = 0;
                        m_breathIncreasing = true;
                    }
                }
                breathon(m_client, m_breathBrightness);
            });
        }
        m_breathBrightness = 0;
        m_breathIncreasing = true;
        m_breathTimer->start(30); // 30ms调整一次亮度
        isBreathOn = true;
        ui->textDisplay->append("呼吸灯已开启");
        
        // 启动UI呼吸特效
        startBreathUIEffect();
    }
}

void MainWindow::on_btnMotorOff_clicked()
{
    cleartext();
    // 关闭呼吸灯时，先改变UI上四个灯的颜色
    if (isRedOn) {
        toggleLamp(ui->lampRed);
        isRedOn = false;
        redmonooff(m_client);
    }
    if (isYellowOn) {
        toggleLamp(ui->lampYellow);
        isYellowOn = false;
        yellowmonooff(m_client);
    }
    if (isGreenOn) {
        toggleLamp(ui->lampGreen);
        isGreenOn = false;
        greenmonooff(m_client);
    }
    if (isBlueOn) {
        toggleLamp(ui->lampBlue);
        isBlueOn = false;
        bluemonooff(m_client);
    }
    // 关闭呼吸灯
    if (m_breathTimer) m_breathTimer->stop();
    breathoff(m_client);
    isBreathOn = false;
    ui->textDisplay->append("呼吸灯已关闭");
    
    // 停止UI呼吸特效
    stopBreathUIEffect();
}

void MainWindow::on_btnPwmRainbow_clicked()
{
    cleartext();
    // 互斥高亮：如果灯光秀按钮已高亮，先恢复
    if (buttonToggleStates.value("btnPwmLightshow", false)) {
        buttonToggleStates["btnPwmLightshow"] = false;
        toggleButtonColor("btnPwmLightshow", false);
    }
    buttonToggleStates["btnPwmRainbow"] = !buttonToggleStates.value("btnPwmRainbow", false);
    toggleButtonColor("btnPwmRainbow", buttonToggleStates["btnPwmRainbow"]);
    m_lightshowTimer->stop();
    lightshowoff(m_client);
    if (!m_pwmRainbowOn) {
        m_lightshowOn=false;
        ui->textDisplay->append("多色灯：rainbow模式启动");
        // 检查客户端是否已连接
        if (m_client && m_client->isConnected()) {
            rainbowon(m_client);
        } else {
            ui->textDisplay->append("错误：VSOA客户端未连接，无法发送命令");
        }
        m_pwmRainbowOn = true;
    } else {
        ui->textDisplay->append("多色灯：rainbow模式关闭");
        // 关闭rainbow模式的逻辑
        if (m_client && m_client->isConnected()) {
            rainbowoff(m_client);
        } else {
            ui->textDisplay->append("错误：VSOA客户端未连接，无法发送关灯命令");
        }
        m_pwmRainbowOn = false;
    }
}

void MainWindow::on_btnPwmLightshow_clicked()
{
    cleartext();
    // 互斥高亮：如果彩虹灯效按钮已高亮，先恢复
    if (buttonToggleStates.value("btnPwmRainbow", false)) {
        buttonToggleStates["btnPwmRainbow"] = false;
        toggleButtonColor("btnPwmRainbow", false);
    }
    buttonToggleStates["btnPwmLightshow"] = !buttonToggleStates.value("btnPwmLightshow", false);
    toggleButtonColor("btnPwmLightshow", buttonToggleStates["btnPwmLightshow"]);
    if(isled_pwmOn(m_client)==1){
        rainbowoff(m_client);
    }
    if (!m_lightshowOn) {
        m_pwmRainbowOn=false;
        ui->textDisplay->append("多色灯：lightshow模式启动");
        if (m_client && m_client->isConnected()) {
            m_lightshowLedIndex = 1; // 启动时从1号灯开始
            m_lightshowTimer->start();
            ui->textDisplay->append("已开始灯光秀");
        } else {
            ui->textDisplay->append("错误：VSOA客户端未连接，无法启动灯光秀");
        }
        m_lightshowOn = true;
    } else {
        ui->textDisplay->append("多色灯：lightshow模式关闭");
        // 彻底停止定时器
        m_lightshowTimer->stop();
        if (m_client && m_client->isConnected()) {
            // 连续两次发送关灯命令，确保可靠
            lightshowoff(m_client);
            QTimer::singleShot(200, this, [this]() {
                lightshowoff(m_client);
                // 关灯后同步一次LED状态到UI
                updateLedStatusDisplay();
            });
            ui->textDisplay->append("已发送关灯命令到服务器");
        }
        m_lightshowOn = false;
    }
}

void MainWindow::on_dialAdjust_valueChanged(int value)
{
    cleartext();
    ui->textDisplay->append(QString("调节旋钮：%1").arg(value));
}

void MainWindow::on_textDisplay_copyAvailable(bool b)
{

}


void MainWindow::on_dialAdjust_actionTriggered(int action)
{

}

void MainWindow::initVsoaClient()
{
    // 创建QVsoaClient实例
    m_client = new QVsoaClient(this);

    // 连接信号槽
    QObject::connect(m_client, &QVsoaClient::connected, std::bind(onConnected, std::placeholders::_1, std::placeholders::_2));
    QObject::connect(m_client, &QVsoaClient::disconnected, onDisconnected);
    QObject::connect(m_client, &QVsoaClient::connected, [this]() {
        // 连接成功后延迟同步LED状态
        QTimer::singleShot(500, this, static_cast<void (MainWindow::*)()>(&MainWindow::updateLedStatusDisplay));
    });
    QObject::connect(m_client, &QVsoaClient::datagram, std::bind(onDatagram, m_client, _1, _2));
    QObject::connect(m_client, &QVsoaClient::message, this,
        [=](QString url, QVsoaPayload payload){
            onMessage(m_client, url, payload);displaySensorStatus();
        });
    

    // 连接到服务器（需要根据实际情况修改服务器地址和端口）
    m_client->connect2server("vsoa://127.0.0.1:5600", SERVER_PASSWORD);

    // 设置自动重连
    m_client->autoConnect(1000, 500);

    ui->textDisplay->append("正在连接VSOA服务器...");

    m_client->subscribe("/sensor/uss/data");
    m_client->subscribe("/adc/data");
    m_client->subscribe("/sensor/dht11/data");
    m_client->autoConsistent({"/sensor/dht11/data", "/sensor/uss/data", "/adc/data"}, 1000);

}

void MainWindow::displaySensorStatus()
{
    QString temp = pDHT11temp+" C";
    QString humi = pDHT11humi+" %";
    QString dist = pUSS+" cm";
    QString brig = pADC+" lux";
    
    // 根据全局标志输出对应信息
    if (hasDHT11 && !hasUSS && !hasADC) {
        updateSensorValue("温度 ：", temp );
        updateSensorValue("湿度 ：", humi);
    } else if (hasUSS && !hasDHT11 && !hasADC) {
        updateSensorValue("距离 ：", dist);
    } else if (hasADC && !hasDHT11 && !hasUSS) {
        updateSensorValue("亮度 ：", brig);
    } else if (hasDHT11 && hasUSS && !hasADC) {
        updateSensorValue("温度 ：", temp );
        updateSensorValue("湿度 ：", humi);
        updateSensorValue("距离 ：", dist);
    } else if (hasDHT11 && hasADC && !hasUSS) {
        updateSensorValue("温度 ：", temp );
        updateSensorValue("湿度 ：", humi);
        updateSensorValue("亮度 ：", brig);
    } else if (hasUSS && hasADC && !hasDHT11) {
        updateSensorValue("距离 ：", dist);
        updateSensorValue("亮度 ：", brig);
    } else if (hasDHT11 && hasUSS && hasADC) {
        updateSensorValue("温度 ：", temp );
        updateSensorValue("湿度 ：", humi);
        updateSensorValue("距离 ：", dist);
        updateSensorValue("亮度 ：", brig);
    }
    else {
        ui->textDisplay_2->append("未订阅任何传感器数据");
    }
}


void MainWindow::removeSensorValue(const QString& key)
{
    QString text = ui->textDisplay_2->toPlainText();
    // 匹配以 key 开头的整行，兼容 Windows 和 Unix 换行，启用多行模式
    QRegularExpression re(QString("^%1.*\\r?\\n?").arg(QRegularExpression::escape(key)), QRegularExpression::MultilineOption);
    QRegularExpressionMatch match = re.match(text);
    if (match.hasMatch()) {
        text.remove(match.capturedStart(), match.capturedLength());
    }
    ui->textDisplay_2->setText(text.trimmed());
    
}

void MainWindow::updateSensorValue(const QString& key, const QString& value)
{
    QString text = ui->textDisplay_2->toPlainText();
    // 匹配 key: 任意内容
    QRegularExpression re(QString("%1[^\n]*").arg(key));
    QString valueStr = QString("%1%2").arg(key).arg(value);
    if (re.match(text).hasMatch()) {
        text.replace(re, valueStr);
    } else {
        if (!text.isEmpty() && !text.endsWith('\n'))
            text += '\n';
        text += valueStr;
    }
    ui->textDisplay_2->setText(text);
}


void MainWindow::syncDialWithPotValue()
{
    extern int latestPotValueC;
    if (qAbs(ui->dialAdjust->value() - latestPotValueC) > 1) {
        ui->dialAdjust->setValue(latestPotValueC);
    }
    // 新增：同步白灯亮度
    extern int latestPotDacBrightnessPercent; // 需在client_subpub.cpp中定义并赋值
    int percent = latestPotDacBrightnessPercent;
    percent = qBound(0, percent, 100); // 限制在0-100
    QString color;
    if (percent < 50) {
        // 低亮度时用灰色，亮度越低越深
        int gray = 180 + (75 * percent) / 50; // 180~255
        color = QString("background-color: rgb(%1,%1,%1); border-radius: 15px;").arg(gray);
    } else {
        // 高亮度时用白色，alpha越高越亮
        int alpha = 100 + (155 * (percent - 50)) / 50; // 100~255
        color = QString("background-color: rgba(255,255,255,%1); border-radius: 15px;").arg(alpha);
    }
    ui->lampWhite->setStyleSheet(color);
}
void MainWindow::syncLedStatus()
{
    // 如果呼吸灯开启，暂停LED状态同步，避免与呼吸特效冲突
    if (isBreathOn) {
        return;
    }
    
    if (m_client && m_client->isConnected()) {
        QJsonObject status = getMonoLedStatus(m_client);
        if (!status.isEmpty()) {
            updateLedStatusDisplay(status);
        }
    } else {
        qDebug() << "[syncLedStatus] Client not connected, skipping LED status sync";
        // 断开时将所有灯置灰
        ui->lampRed->setStyleSheet("background-color: #808080; border-radius: 15px;");
        ui->lampYellow->setStyleSheet("background-color: #808080; border-radius: 15px;");
        ui->lampGreen->setStyleSheet("background-color: #808080; border-radius: 15px;");
        ui->lampBlue->setStyleSheet("background-color: #808080; border-radius: 15px;");
    }
}

void MainWindow::updateLedStatusDisplay()
{
    // 如果呼吸灯开启，暂停LED状态更新，避免与呼吸特效冲突
    if (isBreathOn) {
        return;
    }
    
    if (m_client && m_client->isConnected()) {
        QJsonObject status = getMonoLedStatus(m_client);
        updateLedStatusDisplay(status);
    }
}

void MainWindow::updateLedStatusDisplay(const QJsonObject &status)
{
    // 如果呼吸灯开启，暂停LED状态更新，避免与呼吸特效冲突
    if (isBreathOn) {
        return;
    }
    
    if (!status.contains("leds") || !status["leds"].isArray()) return;
    QJsonArray leds = status["leds"].toArray();

    for (const QJsonValue &ledVal : leds) {
        QJsonObject led = ledVal.toObject();
        QString name = led["name"].toString();
        QString state = led["state"].toString();
        bool isOn = (state == "on");

        if (name == "red") {
            isRedOn = isOn;
            ui->lampRed->setStyleSheet(isOn
                ? "background-color: #e63946; border-radius: 15px;"
                : "background-color: #808080; border-radius: 15px;");
            ui->lampRed->setProperty("on", isOn);
        } else if (name == "yellow") {
            isYellowOn = isOn;
            ui->lampYellow->setStyleSheet(isOn
                ? "background-color: #ffb81c; border-radius: 15px;"
                : "background-color: #808080; border-radius: 15px;");
            ui->lampYellow->setProperty("on", isOn);
        } else if (name == "green") {
            isGreenOn = isOn;
            ui->lampGreen->setStyleSheet(isOn
                ? "background-color: #38b000; border-radius: 15px;"
                : "background-color: #808080; border-radius: 15px;");
            ui->lampGreen->setProperty("on", isOn);
        } else if (name == "blue") {
            isBlueOn = isOn;
            ui->lampBlue->setStyleSheet(isOn
                ? "background-color: #0096ff; border-radius: 15px;"
                : "background-color: #808080; border-radius: 15px;");
            ui->lampBlue->setProperty("on", isOn);
        }
    }
}

void MainWindow::startBreathUIEffect()
{
    // 确保所有灯都亮起，然后开始呼吸动画
    ui->lampRed->setStyleSheet("background-color: rgba(230, 57, 70, 255); border-radius: 15px;");
    ui->lampYellow->setStyleSheet("background-color: rgba(255, 184, 28, 255); border-radius: 15px;");
    ui->lampGreen->setStyleSheet("background-color: rgba(56, 176, 0, 255); border-radius: 15px;");
    ui->lampBlue->setStyleSheet("background-color: rgba(0, 150, 255, 255); border-radius: 15px;");
    
    m_breathUIAlpha = 255;
    m_breathUIIncreasing = true;
    m_breathUITimer->start();
    qDebug() << "UI呼吸特效已启动";
}

void MainWindow::stopBreathUIEffect()
{
    m_breathUITimer->stop();
    
    // 恢复LED状态同步，并重新同步一次硬件状态
    QTimer::singleShot(100, this, [this]() {
        updateLedStatusDisplay();
    });
    
    qDebug() << "UI呼吸特效已停止";
}

void MainWindow::updateBreathUIEffect()
{
    if (!isBreathOn) return;
    
    // 调整UI透明度
    if (m_breathUIIncreasing) {
        m_breathUIAlpha += 10;
        if (m_breathUIAlpha >= 255) {
            m_breathUIAlpha = 255;
            m_breathUIIncreasing = false;
        }
    } else {
        m_breathUIAlpha -= 10;
        if (m_breathUIAlpha <= 100) {
            m_breathUIAlpha = 100;
            m_breathUIIncreasing = true;
        }
    }
    
    // 应用透明度到所有灯
    QString alphaStr = QString::number(m_breathUIAlpha);
    ui->lampRed->setStyleSheet(QString("background-color: rgba(230, 57, 70, %1); border-radius: 15px;").arg(alphaStr));
    ui->lampYellow->setStyleSheet(QString("background-color: rgba(255, 184, 28, %1); border-radius: 15px;").arg(alphaStr));
    ui->lampGreen->setStyleSheet(QString("background-color: rgba(56, 176, 0, %1); border-radius: 15px;").arg(alphaStr));
    ui->lampBlue->setStyleSheet(QString("background-color: rgba(0, 150, 255, %1); border-radius: 15px;").arg(alphaStr));
}

void MainWindow::toggleButtonColor(const QString& btnName, bool isOn)
{
    QPushButton* btn = findChild<QPushButton*>(btnName);
    if (!btn) return;
    QString baseStyle;
    if (btnName == "btnBuzzerOn") {
        baseStyle = isOn ? "background-color: #e63946; color: white;" : "";
    } else if (btnName == "btnBuzzerSongon") {
        baseStyle = isOn ? "background-color: #ffb81c; color: black;" : "";
    } else if (btnName == "btnPwmRainbow") {
        baseStyle = isOn ? "background-color: #38b000; color: white;" : "";
    } else if (btnName == "btnPwmLightshow") {
        baseStyle = isOn ? "background-color: #0096ff; color: white;" : "";
    } else if (btnName == "btnBrightnessSensor" || btnName == "btnUltrasonic" || btnName == "btnDht11") {
        baseStyle = isOn ? "background-color: #888; color: white;" : "";
    }
    btn->setStyleSheet(baseStyle);
}

void MainWindow::on_btnMotorControl_clicked()
{
    if (!motorControlDialog) {
        motorControlDialog = new MotorControlDialog(m_client, this); // 传递 m_client
    }
    motorControlDialog->show();
    motorControlDialog->raise();
    motorControlDialog->activateWindow();
}

void MainWindow::on_btnServoControl_clicked()
{
    if (!servoControlDialog) {
        servoControlDialog = new ServoControlDialog(m_client, this); // 传递 m_client
    }
    servoControlDialog->show();
    servoControlDialog->raise();
    servoControlDialog->activateWindow();
}

void MainWindow::on_btnLidar_clicked() {
    if (!lidarDialog) {
        lidarDialog = new LidarControlDialog(m_client, this);
    }
    lidarDialog->show();
    lidarDialog->raise();
    lidarDialog->activateWindow();
}





