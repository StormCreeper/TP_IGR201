/**
 * @file drawingshape.h
 * @brief This file contains the declaration for the DrawingShape class.
 * @date 2023/10/29
 * @author Telo PHILIPPE
*/

#ifndef DRAWINGSHAPE_H
#define DRAWINGSHAPE_H

#include <QPoint>
#include <QColor>
#include <vector>
#include <QPainter>
#include <sstream>
#include <QDebug>

bool pointInEllipse(QPoint p, QPoint center, int rx, int ry);
bool pointOnLine(QPoint p, QPoint p1, QPoint p2, float tolerance);

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

    // Callback functions used for the drawing
    virtual void onMouseDown(QPoint) = 0;
    virtual void onMouseMove(QPoint) = 0;
    virtual void onMouseUp(QPoint) = 0;

    // Functions for the shape selection
    virtual bool contains(QPoint p) { return getBoundingBox().contains(p); }
    virtual QRect getBoundingBox() = 0;
    // Handles are key points in a shape, that will be visible when selecting
    //  Moving them will help redefine the shape in a simple way
    virtual std::vector<QPoint *> getHandles() = 0;

    // Translates the entire shape
    virtual void moveBy(int dx, int dy) = 0;


    // Used for saving / loading the image file in our custom format
    virtual std::string toString() = 0;
    virtual void fromString(std::string ) = 0;

public: // getters and setters
    QColor getColor() { return color; }
    void setColor(QColor color) { this->color = color; }
    int getSize() { return size; }
    void setSize(int size) { this->size = size; }
};

// Stroke shape defined by a list of points
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

    virtual QRect getBoundingBox();

    virtual bool contains(QPoint p);

    virtual void moveBy(int dx, int dy);

    virtual std::string toString();

    virtual void fromString(std::string line);

    virtual std::vector<QPoint *> getHandles() { return {}; }

};

// General class for things that can be define by two extremities
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

    virtual QRect getBoundingBox();

    virtual bool contains(QPoint p);

    virtual void moveBy(int dx, int dy);

    virtual std::string toString(std::string prefix);

    virtual std::string toString() { return toString("R"); }

    virtual void fromString(std::string line);

    virtual std::vector<QPoint *> getHandles() { return {&startPoint, &endPoint} ; }
};

// What is an ellipse but a rounded rectangle ?
class Ellipse : public Rectangle {
public:
    Ellipse(QColor color, int size) : Rectangle(color, size) {}

public:
    void paint(QPainter &);

    virtual bool contains(QPoint p);

    virtual std::string toString() { return Rectangle::toString("E"); }
};

// What is a line but a ... squished rectangle ?
class Line : public Rectangle {
public:
    Line(QColor color, int size) : Rectangle(color, size) {}

public:
    virtual void paint(QPainter &);

    virtual bool contains(QPoint p);

    virtual std::string toString() { return Rectangle::toString("L"); }

};

#endif // DRAWINGSHAPE_H
