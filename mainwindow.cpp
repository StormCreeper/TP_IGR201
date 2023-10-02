#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) , ui(new Ui::MainWindow) {
    area = new DrawingArea();
    setCentralWidget(area);
}

MainWindow::~MainWindow() {
    delete ui;
    delete area;
}

