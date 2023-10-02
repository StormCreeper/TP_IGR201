#include "drawingarea.h"
#include "QPainter"

DrawingArea::DrawingArea(QWidget *parent) : QWidget{parent} {
    setMinimumSize(QSize(500, 500));
}

void DrawingArea::paintEvent(QPaintEvent *e) {
    QWidget::paintEvent(e);
    QPainter painter(this);
    painter.drawLine(currentLine.startX, currentLine.startY, currentLine.endX, currentLine.endY);
    for(int i=0; i<index; i++) {
        painter.drawLine(lines[i].startX, lines[i].startY, lines[i].endX, lines[i].endY);
    }
}

void DrawingArea::mousePressEvent(QMouseEvent *e) {
    currentLine = Line {e->pos().x(), e->pos().y(), e->pos().x(), e->pos().y()};
    mouseDown = true;
}

void DrawingArea::mouseReleaseEvent(QMouseEvent *e) {
    mouseDown = false;
    lines[index++] = currentLine;
    currentLine = {0, 0, 0, 0};
}

void DrawingArea::mouseMoveEvent(QMouseEvent *e) {
    if (mouseDown) {
        currentLine.endX = e->pos().x();
        currentLine.endY = e->pos().y();
        this->update();
    }
}
