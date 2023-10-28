#include "drawingarea.h"
#include "QPainter"
#include <iostream>
#include <fstream>
#include <QFile>
#include <QMessageBox>


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

    for(auto ds : shapes) {
        if(ds) {
            ds->paint(painter);
        }
    }
    if(currentShape) currentShape->paint(painter);

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
        for (auto shape : shapes) {
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
            currentShape = std::make_shared<Stroke>(currentColor, currentSize);
            break;
        case ShapeType::Line:
            currentShape = std::make_shared<Line>(currentColor, currentSize);
            break;
        case ShapeType::Rect:
            currentShape = std::make_shared<Rectangle>(currentColor, currentSize);
            break;
        case ShapeType::Ellipse:
            currentShape = std::make_shared<Ellipse>(currentColor, currentSize);
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
    this->update();
}

void DrawingArea::mouseReleaseEvent(QMouseEvent *e) {
    mouseDown = false;

    if(selecting) {

    } else {
        currentShape->onMouseUp(e->pos());

        shapes.push_back(currentShape);

        currentShape = nullptr;
    }
}

void DrawingArea::setCurrentTool(ShapeType newCurrentTool) {
    currentTool = newCurrentTool;
}

void DrawingArea::clearAll() {
    shapes.clear();

    this->update();
}

void DrawingArea::toogleSelect() {
    if(currentShape) currentShape = nullptr;
    selecting = !selecting;
}

// Save the current drawing to a text file
// The format of the file is as follows:
// 1. The first line contains the number of shapes
// 2. Each subsequent line contains the information for a single shape
// 3. The information for a shape is as follows:
//    a. The first character is the shape type (L, R, E, S)
//    b. The next three numbers are the color (RGB)
//    c. The next number is the size
//    d. The next numbers are the shape-specific information
//       i. Line: x1, y1, x2, y2
//       ii. Rectangle: x, y, width, height
//       iii. Ellipse: x, y, width, height
//       iv. Stroke: x1, y1, x2, y2, ..., xn, yn
void DrawingArea::save(QString filename) {
    QFile file(filename);
    if (!file.open(QIODevice::WriteOnly)) {
        QMessageBox::information(this, tr("Unable to open file"), file.errorString());
        return;
    }

    QTextStream out(&file);

    out << shapes.size() << "\n";

    for(auto shape : shapes) {
        out << shape->toString().c_str() << "\n";
    }

    file.close();
}










