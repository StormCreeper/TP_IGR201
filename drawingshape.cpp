#include "drawingshape.h"

void Stroke::paint(QPainter &painter) {
    QPen pen {};
    pen.setColor(getColor());
    pen.setWidth(getSize());
    painter.setPen(pen);

    if(points.size() <= 1) return;
    for(int i=0; i<points.size()-1; i++) {
        QPoint &p1 = points[i];
        QPoint &p2 = points[i+1];

        painter.drawLine(p1.x(), p1.y(), p2.x(), p2.y());
    }
}

void Rectangle::paint(QPainter &painter) {
    QPen pen {};
    pen.setColor(getColor());
    pen.setWidth(getSize());
    painter.setPen(pen);

    int x = std::min(startPoint.x(), endPoint.x());
    int y = std::min(startPoint.y(), endPoint.y());

    int w = std::abs(startPoint.x() - endPoint.x());
    int h = std::abs(startPoint.y() - endPoint.y());

    painter.drawRect(x, y, w, h);
}

void Ellipse::paint(QPainter &painter) {
    QPen pen {};
    pen.setColor(getColor());
    pen.setWidth(getSize());
    painter.setPen(pen);

    int x = std::min(startPoint.x(), endPoint.x());
    int y = std::min(startPoint.y(), endPoint.y());

    int w = std::abs(startPoint.x() - endPoint.x());
    int h = std::abs(startPoint.y() - endPoint.y());

    painter.drawEllipse(x, y, w, h);
}

void Line::paint(QPainter &painter) {
    QPen pen {};
    pen.setColor(getColor());
    pen.setWidth(getSize());
    painter.setPen(pen);

    painter.drawLine(startPoint, endPoint);
}
