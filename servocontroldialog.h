#ifndef SERVOCONTROLDIALOG_H
#define SERVOCONTROLDIALOG_H

#include <QDialog>

class QPushButton;
class QSlider;
class QLabel;

class ServoControlDialog : public QDialog {
    Q_OBJECT
public:
    explicit ServoControlDialog(QWidget* parent = nullptr);
    ~ServoControlDialog();

signals:
    void startServo();
    void stopServo();
    void setAngle(int angle);

private slots:
    void onStartClicked();
    void onStopClicked();
    void onSetAngleChanged(int value);
    void onCloseClicked();

private:
    QPushButton* btnStart;
    QPushButton* btnStop;
    QPushButton* btnClose;
    QSlider* sliderAngle;
    QLabel* labelAngleValue;
    QLabel* labelUnit;
};

#endif // SERVOCONTROLDIALOG_H 