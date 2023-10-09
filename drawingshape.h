#ifndef DRAWINGSHAPE_H
#define DRAWINGSHAPE_H

#include <QPoint>
#include <QColor>
#include <vector>
#include <QPainter>

enum class ShapeType { Brush, Line, Rect, Ellipse };

class DrawingShape {
private:
    QColor color {};
    int size {};
public:
    DrawingShape(QColor color, int size) {
        this->color = color;
        this->size = size;
    }

    virtual void paint(QPainter &) = 0;

    virtual void onMouseDown(QPoint) = 0;
    virtual void onMouseMove(QPoint) = 0;
    virtual void onMouseUp(QPoint) = 0;

    virtual bool contains(QPoint) = 0;

    virtual ~DrawingShape() {};

    QColor getColor() { return color; }
    int getSize() { return size; }
};

class Stroke : public DrawingShape {
private:
    std::vector<QPoint> points {};

public:
    Stroke(QColor color, int size) : DrawingShape(color, size) {}

public:
    virtual void paint(QPainter &);

    virtual void onMouseDown(QPoint point) { points.push_back(point); }
    virtual void onMouseMove(QPoint point) { points.push_back(point); };
    virtual void onMouseUp(QPoint point) { points.push_back(point); };

    virtual bool contains(QPoint p) { return 0; }
};

class Rectangle : public DrawingShape {
protected:
    QPoint startPoint {};
    QPoint endPoint {};

public:
    Rectangle(QColor color, int size) : DrawingShape(color, size) {}

public:
    virtual void paint(QPainter &);

    void onMouseDown(QPoint point) { startPoint = point; endPoint = point; }
    void onMouseMove(QPoint point) { endPoint = point; }
    void onMouseUp(QPoint point) { endPoint = point; }

    virtual bool contains(QPoint p) { return 0; }
};

class Ellipse : public Rectangle {
public:
    Ellipse(QColor color, int size) : Rectangle(color, size) {}

public:
    void paint(QPainter &);

    virtual bool contains(QPoint p) { return 0; }
};

class Line : public DrawingShape {
private:
    QPoint startPoint {};
    QPoint endPoint {};

public:
    Line(QColor color, int size) : DrawingShape(color, size) {}

public:
    virtual void paint(QPainter &);

    void onMouseDown(QPoint point) { startPoint = point; endPoint = point; }
    void onMouseMove(QPoint point) { endPoint = point; }
    void onMouseUp(QPoint point) { endPoint = point; }

    virtual bool contains(QPoint p) { return 0; }
};

#endif // DRAWINGSHAPE_H
