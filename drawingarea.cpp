#include "drawingarea.h"
#include "QPainter"

DrawingArea::DrawingArea(QWidget *parent) : QWidget{parent} {
    setMinimumSize(QSize(500, 500));

    setMouseTracking(true);
}

void DrawingArea::paintEvent(QPaintEvent *e) {
    QWidget::paintEvent(e);
    QPainter painter(this);

    QPalette pal {};
    pal.setColor(QPalette::Window, Qt::white);

    setAutoFillBackground(true);
    setPalette(pal);

    for(DrawingShape *ds : shapes) {
        if(ds) {
            ds->paint(painter);
        }
    }
    if(currentShape) currentShape->paint(painter);

    qDebug() << selected;
    if(selected) {
        QPen pen {};
        pen.setColor(Qt::blue);
        pen.setWidth(4);
        painter.setPen(pen);

        painter.drawRect(selected->getBoundingBox());
    }
}

void DrawingArea::mousePressEvent(QMouseEvent *e) {

    if(selecting) {
        selected = nullptr;
        for (DrawingShape *shape : shapes) {
            if(shape->contains(e->pos())) {
                selected = shape;
            }
        }
        selectLastPosX = e->pos().x();
        selectLastPosY = e->pos().y();
        this->update();
    } else {

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

    }
    mouseDown = true;
}

void DrawingArea::mouseMoveEvent(QMouseEvent *e) {

    if(selecting) {
        int currentX = e->pos().x();
        int currentY = e->pos().y();

        if(mouseDown && selected) {
            selected->moveBy(currentX - selectLastPosX, currentY - selectLastPosY);
            selectLastPosX = currentX;
            selectLastPosY = currentY;
            this->update();
        }
    } else {

        if (mouseDown && currentShape) {
            currentShape->onMouseMove(e->pos());
            this->update();
        }
    }
}

void DrawingArea::mouseReleaseEvent(QMouseEvent *e) {

    if(selecting) {
        selected = nullptr;
    } else {

        mouseDown = false;

        currentShape->onMouseUp(e->pos());

        shapes.push_back(currentShape);

        currentShape = nullptr;
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

void DrawingArea::toogleSelect() {
    if(currentShape) delete currentShape;
    selecting = !selecting;
}

