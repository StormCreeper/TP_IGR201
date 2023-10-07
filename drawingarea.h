#ifndef DRAWINGAREA_H
#define DRAWINGAREA_H

#include <QWidget>
#include <QPaintEvent>
#include <QMouseEvent>
#include <vector>

#define MAX_LINES 1000

struct Point {
    int x;
    int y;
};

struct Line {
    Point start;
    Point end;
};

struct Stroke {
    std::vector<Point> points;
    QColor color;
    int size;
};

enum class Tool { Brush, Rect, Ellipse };

class DrawingArea : public QWidget {
    Q_OBJECT
public:
    explicit DrawingArea(QWidget *parent = nullptr);

protected:
    virtual void paintEvent(QPaintEvent *e);

    virtual void mousePressEvent(QMouseEvent*);
    virtual void mouseReleaseEvent(QMouseEvent*);
    virtual void mouseMoveEvent(QMouseEvent*);

    void drawStroke(QPainter &painter, Stroke &stroke);

private:
    Stroke currentStroke {};
    std::vector<Stroke> strokes {};
    bool mouseDown {};
    int index = 0;

    QColor currentColor;
    int currentSize = 1;

    Tool tool;

public:
    void setColor(QColor color) {currentColor = color;}
    void setSize(int size) {currentSize = size;}
};

#endif // DRAWINGAREA_H
