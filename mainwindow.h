#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "drawingarea.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    DrawingArea *area;

    QAction *actionSizeSmall {};
    QAction *actionSizeMedium {};
    QAction *actionSizeBig {};

public slots:
    void chooseColor();
    void chooseSize(QAction *action);
};
#endif // MAINWINDOW_H
