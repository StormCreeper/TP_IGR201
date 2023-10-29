/**
 * @file drawingarea.h
 * @brief This file contains the declaration for the DrawingArea class.
 * @date 2023/10/29
 * @author Telo PHILIPPE
*/

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

    virtual void keyPressEvent(QKeyEvent*);

private:
    std::shared_ptr<DrawingShape> currentShape {};
    std::vector<std::shared_ptr<DrawingShape>> shapes {};
    bool mouseDown {};

    QColor currentColor;
    int currentSize = 1;

    ShapeType currentTool = ShapeType::Brush;

    std::shared_ptr<DrawingShape> selected {};

    QPoint *selectedHandle {};

    bool selecting {};
    int selectLastPosX {};
    int selectLastPosY {};

    bool modified = true;

    bool getHovering(QPoint p);

public:
    QString filename {};
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

    void save();
    void load();

    void setModified(bool modified) {
        this->modified = modified;
        std::string title = "Currently editing : " + (filename.isEmpty() ? "Unnamed " : filename.toStdString() + " ") + (modified ? "*" : "");
        window()->setWindowTitle(QString::fromStdString(title));
    }
    bool isModified() {return modified;}
};

#endif // DRAWINGAREA_H
