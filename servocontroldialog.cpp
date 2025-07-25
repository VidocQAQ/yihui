#include "servocontroldialog.h"
#include <QPushButton>
#include <QSlider>
#include <QLabel>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include "client_rpc.h"
#include "servodial.h"

ServoControlDialog::ServoControlDialog(QVsoaClient* client, QWidget* parent)
    : QDialog(parent), m_client(client)
{
    setWindowTitle("舵机控制");
    setFixedSize(320, 180);

    btnStart = new QPushButton("启动", this);
    btnClose = new QPushButton("关闭", this);

    dialAngle = new ServoDial(this);
    dialAngle->setRange(0, 180);
    dialAngle->setAngle(90);
    dialAngle->setEnabled(false);

    labelAngleValue = new QLabel(QString::number(dialAngle->angle()), this);
    labelUnit = new QLabel("°", this);

    QHBoxLayout* angleLayout = new QHBoxLayout;
    angleLayout->addWidget(new QLabel("角度:", this));
    angleLayout->addWidget(dialAngle);
    angleLayout->addWidget(labelAngleValue);
    angleLayout->addWidget(labelUnit);

    QHBoxLayout* btnLayout = new QHBoxLayout;
    btnLayout->addWidget(btnStart);
    btnLayout->addWidget(btnClose);

    QVBoxLayout* mainLayout = new QVBoxLayout;
    mainLayout->addLayout(angleLayout);
    mainLayout->addSpacing(10);
    mainLayout->addLayout(btnLayout);
    setLayout(mainLayout);

    connect(btnStart, &QPushButton::clicked, this, &ServoControlDialog::onStartClicked);
    connect(btnClose, &QPushButton::clicked, this, &ServoControlDialog::onCloseClicked);
    connect(dialAngle, &ServoDial::angleChanged, this, &ServoControlDialog::onSetAngleChanged);
}

ServoControlDialog::~ServoControlDialog() {}

void ServoControlDialog::onStartClicked() {
    isRunning = !isRunning;
    if (isRunning) {
        btnStart->setText("停止");
        btnStart->setStyleSheet("background-color:#38b000;color:white;");
        dialAngle->setEnabled(true);
        servoseton(m_client, dialAngle->angle());
    } else {
        btnStart->setText("启动");
        btnStart->setStyleSheet("");
        dialAngle->setEnabled(false);
        servooff(m_client);
    }
}

void ServoControlDialog::onSetAngleChanged(int value) {
    labelAngleValue->setText(QString::number(value));
    if (isRunning) {
        servoseton(m_client, value);
    }
}

void ServoControlDialog::onCloseClicked() {
    this->close();
} 
