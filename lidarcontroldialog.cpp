#include "lidarcontroldialog.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include "lidarpolarwidget.h"
#include <QRandomGenerator>
#include "client_subpub.h"

LidarControlDialog::LidarControlDialog(QVsoaClient* client, QWidget* parent)
    : QDialog(parent), m_client(client)
{
    setWindowTitle("激光雷达控制");
    setFixedSize(360, 340);

    btnStart = new QPushButton("启动", this);
    btnClose = new QPushButton("关闭", this);
    polarWidget = new LidarPolarWidget(this);
    testTimer = new QTimer(this);
    connect(testTimer, &QTimer::timeout, this, &LidarControlDialog::setLidarPoints);

    QHBoxLayout* btnLayout = new QHBoxLayout;
    btnLayout->addWidget(btnStart);
    btnLayout->addWidget(btnClose);

    QVBoxLayout* mainLayout = new QVBoxLayout;
    mainLayout->addWidget(polarWidget);
    mainLayout->addLayout(btnLayout);
    setLayout(mainLayout);

    connect(btnStart, &QPushButton::clicked, this, &LidarControlDialog::onStartClicked);
    connect(btnClose, &QPushButton::clicked, this, &LidarControlDialog::onCloseClicked);
}

LidarControlDialog::~LidarControlDialog() {}

//开启
void LidarControlDialog::onStartClicked() {
    isRunning = !isRunning;
    if (isRunning) {
        btnStart->setText("停止");
        btnStart->setStyleSheet("background-color:#38b000;color:white;");
        testTimer->start(500); // 启动定时器，500ms刷新一次
        LidarControlOn(m_client);
    } else {
        btnStart->setText("启动");
        btnStart->setStyleSheet("");
        testTimer->stop();
        LidarControlOff(m_client);
        lidarPoints.clear();         // 清空点
        setLidarPoints();            // 刷新界面
    }
}

void LidarControlDialog::onCloseClicked() {
    this->close();
}

void LidarControlDialog::setLidarPoints() {
    QVector<QPair<double, double>> points = lidarPoints;
    if (polarWidget) polarWidget->setPoints(points);
}

//
