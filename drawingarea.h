#ifndef DRAWINGAREA_H
#define DRAWINGAREA_H

#include "drawingshape.h"

#include <QWidget>
#include <QPaintEvent>
#include <QMouseEvent>
#include <vector>


class DrawingArea : public QWidget {
    Q_OBJECT
public:
    explicit DrawingArea(QWidget *parent = nullptr);

protected:
    virtual void paintEvent(QPaintEvent *e);

    virtual void mousePressEvent(QMouseEvent*);
    virtual void mouseReleaseEvent(QMouseEvent*);
    virtual void mouseMoveEvent(QMouseEvent*);

private:
    DrawingShape *currentShape {};
    std::vector<DrawingShape*> shapes {};
    bool mouseDown {};
    int index = 0;

    QColor currentColor;
    int currentSize = 1;

    ShapeType currentTool = ShapeType::Brush;

public:
    ~DrawingArea() {
        for(DrawingShape *shape : shapes) {
            delete shape;
        }
    }

public:
    void setColor(QColor color) { currentColor = color; }
    void setSize(int size) { currentSize = size; }
    void setTool(ShapeType tool) { currentTool = tool; }
    void setCurrentTool(ShapeType newCurrentTool);
    void clearAll();
};

#endif // DRAWINGAREA_H
