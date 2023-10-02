#include "drawingarea.h"
#include "QPainter"

DrawingArea::DrawingArea(QWidget *parent) : QWidget{parent} {
    setMinimumSize(QSize(500, 500));
}

void DrawingArea::paintEvent(QPaintEvent *e) {
    QWidget::paintEvent(e);
    QPainter painter(this);
    /*painter.drawLine(currentLine.start.x, currentLine.start.y, currentLine.end.x, currentLine.end.y);
    for(int i=0; i<index; i++) {
        painter.drawLine(lines[i].start.x, lines[i].start.y, lines[i].end.x, lines[i].end.y);
    }*/

    drawStroke(painter, currentStroke);
    for(Stroke &s : strokes) {
        drawStroke(painter, s);
    }
}

void DrawingArea::drawStroke(QPainter &painter, Stroke &stroke) {
    QPen pen {};
    pen.setColor(stroke.color);
    pen.setWidth(stroke.size);
    painter.setPen(pen);

    if(stroke.points.size() <= 1) return;
    for(int i=0; i<stroke.points.size()-1; i++) {
        Point &p1 = stroke.points[i];
        Point &p2 = stroke.points[i+1];

        painter.drawLine(p1.x, p1.y, p2.x, p2.y);
    }
}

void DrawingArea::mousePressEvent(QMouseEvent *e) {
    currentStroke = Stroke { };
    currentStroke.points.push_back({e->pos().x(), e->pos().y()});
    currentStroke.color = currentColor;
    currentStroke.size = currentSize;
    mouseDown = true;
}

void DrawingArea::mouseReleaseEvent(QMouseEvent *e) {
    mouseDown = false;
    currentStroke.points.push_back({e->pos().x(), e->pos().y()});
    strokes.push_back(currentStroke);

    currentStroke = {};
}

void DrawingArea::mouseMoveEvent(QMouseEvent *e) {
    if (mouseDown) {
        currentStroke.points.push_back({e->pos().x(), e->pos().y()});

        this->update();
    }
}
