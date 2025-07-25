#include "lidarpolarwidget.h"
#include <QPainter>
#include <QtMath>

LidarPolarWidget::LidarPolarWidget(QWidget* parent) : QWidget(parent) {
    setMinimumSize(240, 240);
}

void LidarPolarWidget::setPoints(const QVector<QPair<double, double>>& points) {
    m_points = points;
    update();
}

void LidarPolarWidget::paintEvent(QPaintEvent*) {
    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing);
    int w = width();
    int h = height();
    QPoint center(w/2, h/2);
    int radius = qMin(w, h)/2 - 20;
    // 画极坐标圆环
    p.setPen(QPen(Qt::gray, 1));
    for (int i = 1; i <= 4; ++i) {
        p.drawEllipse(center, radius*i/4, radius*i/4);
    }
    // 画主轴线
    for (int a = 0; a < 360; a += 30) {
        double rad = qDegreesToRadians((double)a);
        QPointF p1 = center;
        QPointF p2 = center + QPointF(qCos(rad)*radius, -qSin(rad)*radius);
        p.drawLine(p1, p2);
    }
    // 画点
    p.setPen(Qt::NoPen);
    p.setBrush(Qt::red);
    for (const auto& pt : m_points) {
        double angle = pt.first;
        double dist = pt.second;
        if (dist <= 0.01) continue;
        double r = qMin(dist / m_maxDistance, 1.0) * radius;
        double rad = qDegreesToRadians(angle);
        QPointF pos = center + QPointF(qCos(rad)*r, -qSin(rad)*r);
        p.drawEllipse(pos, 2, 2);
    }
} 