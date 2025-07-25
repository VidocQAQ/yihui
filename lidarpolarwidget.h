#pragma once
#include <QWidget>
#include <QVector>
#include <QPair>

class LidarPolarWidget : public QWidget {
    Q_OBJECT
public:
    explicit LidarPolarWidget(QWidget* parent = nullptr);
    void setPoints(const QVector<QPair<double, double>>& points); // angle, distance

protected:
    void paintEvent(QPaintEvent* event) override;

private:
    QVector<QPair<double, double>> m_points;
    double m_maxDistance = 5.0; // 最大显示距离，单位米
}; 