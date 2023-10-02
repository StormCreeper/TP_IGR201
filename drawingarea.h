#ifndef DRAWINGAREA_H
#define DRAWINGAREA_H

#include <QWidget>
#include <QPaintEvent>
#include <QMouseEvent>
#include <vector>

#define MAX_LINES 100

struct Line {
    int startX;
    int startY;
    int endX;
    int endY;
};

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
    Line currentLine {};
    bool mouseDown {};
    Line lines[MAX_LINES] {};
    int index = 0;
};

#endif // DRAWINGAREA_H
