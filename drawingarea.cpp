#include "drawingarea.h"
#include "QPainter"

DrawingArea::DrawingArea(QWidget *parent) : QWidget{parent} {
    setMinimumSize(QSize(500, 500));
}

void DrawingArea::paintEvent(QPaintEvent *e) {
    QWidget::paintEvent(e);
    QPainter painter(this);

    QPalette pal {};
    pal.setColor(QPalette::Window, Qt::white);

    setAutoFillBackground(true);
    setPalette(pal);

    for(DrawingShape *ds : shapes) {
        if(ds)
            ds->paint(painter);
    }
    if(currentShape) currentShape->paint(painter);
}

void DrawingArea::mousePressEvent(QMouseEvent *e) {
    switch(currentTool) {
    case ShapeType::Brush:
        currentShape = new Stroke {currentColor, currentSize};
        break;
    case ShapeType::Line:
        currentShape = new Line {currentColor, currentSize};
        break;
    case ShapeType::Rect:
        currentShape = new Rectangle {currentColor, currentSize};
        break;
    case ShapeType::Ellipse:
        currentShape = new Ellipse {currentColor, currentSize};
        break;
    }

    currentShape->onMouseDown(e->pos());

    mouseDown = true;
}

void DrawingArea::mouseMoveEvent(QMouseEvent *e) {
    if (mouseDown) {
        currentShape->onMouseMove(e->pos());

        this->update();
    }
}

void DrawingArea::setCurrentTool(ShapeType newCurrentTool) {
    currentTool = newCurrentTool;
}

void DrawingArea::clearAll() {
    for(DrawingShape *shape : shapes) {
        delete shape;
    }
    shapes.clear();

    this->update();
}

void DrawingArea::mouseReleaseEvent(QMouseEvent *e) {
    mouseDown = false;

    currentShape->onMouseUp(e->pos());

    shapes.push_back(currentShape);

    currentShape = nullptr;
}


