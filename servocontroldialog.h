#ifndef SERVOCONTROLDIALOG_H
#define SERVOCONTROLDIALOG_H

#include <QDialog>
#include <QVsoa>
#include "servodial.h"

class QPushButton;
class QSlider;
class QLabel;

class ServoControlDialog : public QDialog {
    Q_OBJECT
public:
    explicit ServoControlDialog(QVsoaClient* client, QWidget* parent = nullptr);
    ~ServoControlDialog();

signals:
    void startServo();
    void stopServo();
    void setAngle(int angle);

private slots:
    void onStartClicked();
    void onSetAngleChanged(int value);
    void onCloseClicked();

private:
    QPushButton* btnStart;
    QPushButton* btnClose;
    ServoDial* dialAngle;
    QLabel* labelAngleValue;
    QLabel* labelUnit;
    QVsoaClient* m_client;
    bool isRunning = false;
};

#endif // SERVOCONTROLDIALOG_H 