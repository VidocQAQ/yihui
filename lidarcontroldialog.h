#pragma once
#include <QDialog>
#include <QPushButton>
#include "lidarpolarwidget.h"
#include <QTimer>

class LidarControlDialog : public QDialog {
    Q_OBJECT
public:
    explicit LidarControlDialog(QWidget* parent = nullptr);
    ~LidarControlDialog();
    void setLidarPoints(const QVector<QPair<double, double>>& points);

private slots:
    void onStartClicked();
    void onCloseClicked();

private:
    QPushButton* btnStart;
    QPushButton* btnClose;
    LidarPolarWidget* polarWidget;
    QTimer* testTimer = nullptr;
    bool isRunning = false;
    void generateTestData();
}; 