#include "drawingshape.h"

bool pointInEllipse(QPoint p, QPoint center, int rx, int ry) {
    float dx = p.x() - center.x();
    float dy = p.y() - center.y();

    return (dx*dx)/(rx*rx) + (dy*dy)/(ry*ry) <= 1.0f;
}

bool pointOnLine(QPoint p, QPoint p1, QPoint p2, float tolerance) {
    float dx = p2.x() - p1.x();
    float dy = p2.y() - p1.y();

    float d = std::sqrt(dx*dx + dy*dy);

    float u = ((p.x() - p1.x()) * dx + (p.y() - p1.y()) * dy) / (d*d);

    if(u < 0.0f || u > 1.0f) return false;

    float x = p1.x() + u * dx;
    float y = p1.y() + u * dy;

    float dist = std::sqrt((x-p.x())*(x-p.x()) + (y-p.y())*(y-p.y()));

    return dist <= tolerance;
}

void Stroke::paint(QPainter &painter) {
    if(points.size() <= 1) return;
    for(int i=0; i<points.size()-1; i++) {
        QPoint &p1 = points[i];
        QPoint &p2 = points[i+1];

        painter.drawLine(p1.x(), p1.y(), p2.x(), p2.y());
    }
}

void Rectangle::paint(QPainter &painter) {
    painter.drawRect(getBoundingBox());
}

void Ellipse::paint(QPainter &painter) {
    int x = std::min(startPoint.x(), endPoint.x());
    int y = std::min(startPoint.y(), endPoint.y());

    int w = std::abs(startPoint.x() - endPoint.x());
    int h = std::abs(startPoint.y() - endPoint.y());

    painter.drawEllipse(x, y, w, h);
}

void Line::paint(QPainter &painter) {
    painter.drawLine(startPoint, endPoint);
}
