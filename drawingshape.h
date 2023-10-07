#ifndef DRAWINGSHAPE_H
#define DRAWINGSHAPE_H

#include <QPoint>

class DrawingShape {
public:
    DrawingShape();

    virtual void paint() = 0;

    virtual void onMouseDown(QPoint &) = 0;
    virtual void onMouseMove(QPoint &) = 0;
    virtual void onMouseUp(QPoint &) = 0;
};

#endif // DRAWINGSHAPE_H
