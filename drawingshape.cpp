/**
 * @file drawingshape.cpp
 * @brief This file contains the implementation for the DrawingShape class.
 * @date 2023/10/29
 * @author Telo PHILIPPE
*/

#include "drawingshape.h"


// General functions

bool pointInEllipse(QPoint p, QPoint center, int rx, int ry) {
    float dx = p.x() - center.x();
    float dy = p.y() - center.y();

    return (dx*dx)/(rx*rx) + (dy*dy)/(ry*ry) <= 1.0f;
}

bool pointOnLine(QPoint p, QPoint p1, QPoint p2, float tolerance) {
    float dx = p2.x() - p1.x();
    float dy = p2.y() - p1.y();

    float d = std::sqrt(dx*dx + dy*dy);

    float u = ((p.x() - p1.x()) * dx + (p.y() - p1.y()) * dy) / (d*d);

    if(u < 0.0f || u > 1.0f) return false;

    float x = p1.x() + u * dx;
    float y = p1.y() + u * dy;

    float dist = std::sqrt((x-p.x())*(x-p.x()) + (y-p.y())*(y-p.y()));

    return dist <= tolerance;
}

// --------- Stroke class -------------

void Stroke::paint(QPainter &painter) {
    if(points.size() <= 1) return;
    for(int i=0; i<points.size()-1; i++) {
        QPoint &p1 = points[i];
        QPoint &p2 = points[i+1];

        painter.drawLine(p1.x(), p1.y(), p2.x(), p2.y());
    }
}

std::string Stroke::toString()  {
    std::string result = "";
    result += "S ";
    result += std::to_string(getColor().red()) + " ";
    result += std::to_string(getColor().green()) + " ";
    result += std::to_string(getColor().blue()) + " ";
    result += std::to_string(getSize()) + " ";
    for(QPoint p : points) {
        result += std::to_string(p.x()) + " ";
        result += std::to_string(p.y()) + " ";
    }
    return result;
}

void Stroke::fromString(std::string line)  {
    std::istringstream iss(line);
    std::vector<std::string> tokens{std::istream_iterator<std::string>{iss}, std::istream_iterator<std::string>{}};

    for(int i = 5; i < tokens.size(); i+=2) {
        int x = std::stoi(tokens[i]);
        int y = std::stoi(tokens[i+1]);
        points.push_back(QPoint(x, y));
    }
}

// --------------- Rectangle class -----------------

void Rectangle::paint(QPainter &painter) {
    painter.drawRect(getBoundingBox());
}

QRect Rectangle::getBoundingBox() {
    int x = std::min(startPoint.x(), endPoint.x());
    int y = std::min(startPoint.y(), endPoint.y());

    int w = std::abs(startPoint.x() - endPoint.x());
    int h = std::abs(startPoint.y() - endPoint.y());

    return QRect {x, y, w, h};
}

bool Rectangle::contains(QPoint p) {
    QRect bb = getBoundingBox();
    QRect rect1 = bb.adjusted(-5, -5, 5, 5);
    QRect rect2 = bb.adjusted(5, 5, -5, -5);

    return rect1.contains(p) && !rect2.contains(p);
}

void Rectangle::moveBy(int dx, int dy) {
    startPoint.setX(startPoint.x() + dx);
    startPoint.setY(startPoint.y() + dy);

    endPoint.setX(endPoint.x() + dx);
    endPoint.setY(endPoint.y() + dy);
}

std::string Rectangle::toString(std::string prefix) {
    std::string result = "";
    result += prefix + " ";
    result += std::to_string(getColor().red()) + " ";
    result += std::to_string(getColor().green()) + " ";
    result += std::to_string(getColor().blue()) + " ";
    result += std::to_string(getSize()) + " ";
    result += std::to_string(startPoint.x()) + " ";
    result += std::to_string(startPoint.y()) + " ";
    result += std::to_string(endPoint.x()) + " ";
    result += std::to_string(endPoint.y()) + " ";
    return result;
}

void Rectangle::fromString(std::string line) {
    std::istringstream iss(line);
    std::vector<std::string> tokens{std::istream_iterator<std::string>{iss}, std::istream_iterator<std::string>{}};

    startPoint.setX(std::stoi(tokens[5]));
    startPoint.setY(std::stoi(tokens[6]));
    endPoint.setX(std::stoi(tokens[7]));
    endPoint.setY(std::stoi(tokens[8]));
}

// ---------------------- Ellipse classe ----------------------


void Ellipse::paint(QPainter &painter) {
    int x = std::min(startPoint.x(), endPoint.x());
    int y = std::min(startPoint.y(), endPoint.y());

    int w = std::abs(startPoint.x() - endPoint.x());
    int h = std::abs(startPoint.y() - endPoint.y());

    painter.drawEllipse(x, y, w, h);
}

bool Ellipse::contains(QPoint p) {
    QRect bb = getBoundingBox();
    QPoint center = bb.center();
    int rx = bb.width()/2;
    int ry = bb.height()/2;

    return pointInEllipse(p, center, rx + 5, ry + 5) && !pointInEllipse(p, center, rx - 5, ry - 5);
}


// ----------------------- Line class ---------------------------

void Line::paint(QPainter &painter) {
    painter.drawLine(startPoint, endPoint);
}

bool Line::contains(QPoint p) {
    QRect bb = getBoundingBox();
    QPoint p1 = bb.topLeft();
    QPoint p2 = bb.bottomRight();

    return pointOnLine(p, p1, p2, 5);
}
