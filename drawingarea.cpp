#include "drawingarea.h"
#include "QPainter"
#include <iostream>
#include <fstream>
#include <QFile>
#include <QMessageBox>
#include <sstream>


DrawingArea::DrawingArea(QWidget *parent) : QWidget{parent} {
    setMinimumSize(QSize(500, 500));

    setMouseTracking(true);
    setFocusPolicy(Qt::StrongFocus);
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
            QPen pen {};
            pen.setColor(ds->getColor());
            pen.setWidth(ds->getSize());
            painter.setPen(pen);

            ds->paint(painter);
        }
    }
    if(currentShape) {
        QPen pen {};
        pen.setColor(currentShape->getColor());
        pen.setWidth(currentShape->getSize());
        painter.setPen(pen);

        currentShape->paint(painter);
    }

    if(selected) {
        QPen pen {};
        pen.setColor(Qt::blue);
        pen.setWidth(4);
        painter.setPen(pen);

        selected->paint(painter);

        pen.setColor(Qt::black);
        pen.setWidth(1);
        painter.setPen(pen);

        auto handles = selected->getHandles();
        for(auto handle : handles) {
            painter.fillRect(handle->x()-4, handle->y()-4, 8, 8, Qt::white);
            painter.drawRect(handle->x()-4, handle->y()-4, 8, 8);
        }
    }
}

void DrawingArea::keyPressEvent(QKeyEvent *e) {
    qDebug() << "Key pressed: " << e->key();
    if(e->key() == Qt::Key_Delete) {
        if(selecting && selected) {
            shapes.erase(std::remove(shapes.begin(), shapes.end(), selected), shapes.end());
            selected = nullptr;
            this->update();
            setModified(true);
        }
    }
}

void DrawingArea::mousePressEvent(QMouseEvent *e) {
    mouseDown = true;

    if(selecting) {
        if(selected) {
            auto handles = selected->getHandles();
            for(auto handle : handles) {
                if(QRect(handle->x()-4, handle->y()-4, 8, 8).contains(e->pos())) {
                    selectedHandle = handle;
                    selectLastPosX = e->pos().x();
                    selectLastPosY = e->pos().y();
                    return;
                }
            }
        }
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
}

void DrawingArea::mouseMoveEvent(QMouseEvent *e) {

    if(selecting) {
        int currentX = e->pos().x();
        int currentY = e->pos().y();

        if(mouseDown) {
            if(selectedHandle) {
                selectedHandle->setX(currentX);
                selectedHandle->setY(currentY);
                this->update();
                setModified(true);
            } else if(selected) {
                selected->moveBy(currentX - selectLastPosX, currentY - selectLastPosY);
                selectLastPosX = currentX;
                selectLastPosY = currentY;
                this->update();
                setModified(true);
            }
        }

    } else {

        if (mouseDown && currentShape) {
            currentShape->onMouseMove(e->pos());
            this->update();
            setModified(true);
        }
    }
}

void DrawingArea::mouseReleaseEvent(QMouseEvent *e) {
    mouseDown = false;

    if(selecting) {
        selectedHandle = nullptr;
    } else {
        currentShape->onMouseUp(e->pos());

        shapes.push_back(currentShape);

        currentShape = nullptr;

        setModified(true);
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
void DrawingArea::save() {
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

    setModified(false);
}


void DrawingArea::load() {
    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly)) {
        QMessageBox::information(this, tr("Unable to open file"), file.errorString());
        return;
    }

    QTextStream in(&file);

    int shapeCount = in.readLine().toInt();

    for(int i = 0; i < shapeCount; i++) {
        std::string line = in.readLine().toStdString();

        std::istringstream iss(line);
        std::vector<std::string> tokens{std::istream_iterator<std::string>{iss}, std::istream_iterator<std::string>{}};

        ShapeType type;
        if(tokens[0] == "L") type = ShapeType::Line;
        if(tokens[0] == "R") type = ShapeType::Rect;
        if(tokens[0] == "E") type = ShapeType::Ellipse;
        if(tokens[0] == "S") type = ShapeType::Brush;

        QColor color {std::stoi(tokens[1]), std::stoi(tokens[2]), std::stoi(tokens[3])};
        int size = std::stoi(tokens[4]);

        std::shared_ptr<DrawingShape> shape = nullptr;

        switch(type) {
        case ShapeType::Line:
            shape = std::make_shared<Line>(color, size);
            break;
        case ShapeType::Rect:
            shape = std::make_shared<Rectangle>(color, size);
            break;
        case ShapeType::Ellipse:
            shape = std::make_shared<Ellipse>(color, size);
            break;
        case ShapeType::Brush:
            shape = std::make_shared<Stroke>(color, size);
            break;
        }

        if(shape) {
            shape->fromString(line);
            shapes.push_back(shape);
        }
    }

    file.close();

    this->update();

    setModified(true);
}









