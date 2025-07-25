#include "lidarcontroldialog.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include "lidarpolarwidget.h"
#include <QRandomGenerator>

LidarControlDialog::LidarControlDialog(QWidget* parent)
    : QDialog(parent)
{
    setWindowTitle("激光雷达控制");
    setFixedSize(360, 340);

    btnStart = new QPushButton("启动", this);
    btnClose = new QPushButton("关闭", this);
    polarWidget = new LidarPolarWidget(this);
    testTimer = new QTimer(this);
    connect(testTimer, &QTimer::timeout, this, &LidarControlDialog::generateTestData);

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

void LidarControlDialog::onStartClicked() {
    isRunning = !isRunning;
    if (isRunning) {
        btnStart->setText("停止");
        btnStart->setStyleSheet("background-color:#38b000;color:white;");
        testTimer->start(500); // 启动定时器，500ms刷新一次
    } else {
        btnStart->setText("启动");
        btnStart->setStyleSheet("");
        testTimer->stop();
    }
}

void LidarControlDialog::onCloseClicked() {
    this->close();
}

void LidarControlDialog::setLidarPoints(const QVector<QPair<double, double>>& points) {
    if (polarWidget) polarWidget->setPoints(points);
}

void LidarControlDialog::generateTestData() {
    QVector<QPair<double, double>> points;
    int num = 720;
    for (int i = 0; i < num; ++i) {
        double angle = i * 0.5; // 0~359.5度
        double distance = 1.0 + QRandomGenerator::global()->bounded(2.0); // 1.0~3.0米
        points.append(qMakePair(angle, distance));
    }
    setLidarPoints(points);
} 