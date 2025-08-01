#ifndef MOTORCONTROLDIALOG_H
#define MOTORCONTROLDIALOG_H

#include <QDialog>
#include "client_rpc.h"

class QPushButton;
class QSlider;
class QLabel;

class MotorControlDialog : public QDialog {
    Q_OBJECT
public:
    explicit MotorControlDialog(QVsoaClient* client, QWidget* parent = nullptr);
    ~MotorControlDialog();

signals:
    void startMotor();
    void stopMotor();
    void setSpeed(int speed);
    void setDirection(bool forward); // true: 正转, false: 反转

private slots:
    void onStartClicked();
    void onSetSpeedChanged(int value);
    void onCloseClicked();
    void onDirectionClicked();
    void onSpeedPresetClicked();

private:
    QVsoaClient* m_client; // 新增
    QPushButton* btnStart;
    QPushButton* btnClose;
    QPushButton* btnDirection;
    QPushButton* btnSpeed1;
    QPushButton* btnSpeed2;
    QPushButton* btnSpeed3;
    QPushButton* btnSpeed4;
    QPushButton* btnCustom;
    QSlider* sliderSpeed;
    QLabel* labelSpeedValue;
    QLabel* labelUnit;
    int speedPresets[4] = {500, 1000, 1500, 2000};
    int currentPresetIndex = -1;
    bool isForward = true;
    bool isRunning = false;

};

#endif // MOTORCONTROLDIALOG_H 
