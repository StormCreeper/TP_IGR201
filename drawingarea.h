#ifndef DRAWINGAREA_H
#define DRAWINGAREA_H

#include "drawingshape.h"

#include <QWidget>
#include <QPaintEvent>
#include <QMouseEvent>
#include <vector>
#include <memory>


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
    std::shared_ptr<DrawingShape> currentShape {};
    std::vector<std::shared_ptr<DrawingShape>> shapes {};
    bool mouseDown {};
    int index = 0;

    QColor currentColor;
    int currentSize = 1;

    ShapeType currentTool = ShapeType::Brush;

    std::shared_ptr<DrawingShape> selected {};

    bool selecting {};
    int selectLastPosX {};
    int selectLastPosY {};

public:
    ~DrawingArea() {
    }

public:
    void setColor(QColor color) {
        if(selecting && selected) {
            selected->setColor(color);
            this->update();
        }
        else currentColor = color;
    }
    void setSize(int size) {
        if(selected && selected) {
            selected->setSize(size);
            this->update();
        }
        else currentSize = size;
    }

    void setTool(ShapeType tool) { currentTool = tool; }
    void setCurrentTool(ShapeType newCurrentTool);
    void clearAll();
    void toogleSelect();

    void save(QString filename);
    void load(QString filename);
};

#endif // DRAWINGAREA_H
