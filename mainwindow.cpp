#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QColorDialog>
#include <QActionGroup>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) , ui(new Ui::MainWindow) {

    QMenuBar *menuBar = this->menuBar();
    //QMenu *menuFile = menuBar->addMenu(tr("&File"));
    QMenu *menuDraw = menuBar->addMenu(tr("&Draw"));
    QMenu *menuChooseSize = menuDraw->addMenu(tr("&Choose size"));

    QAction *actionChooseColor = new QAction(QIcon(), tr("&Choose color"), this);

    QActionGroup *group = new QActionGroup(this);
    connect(group, SIGNAL(triggered(QAction *)), this, SLOT(chooseSize(QAction *)));

    actionSizeSmall = group->addAction(tr("&Small"));
    actionSizeMedium = group->addAction(tr("&Medium"));
    actionSizeBig = group->addAction(tr("&Big"));

    menuDraw->addAction(actionChooseColor);
    menuChooseSize->addAction(actionSizeSmall);
    menuChooseSize->addAction(actionSizeMedium);
    menuChooseSize->addAction(actionSizeBig);

    connect(actionChooseColor, SIGNAL(triggered()), this, SLOT(chooseColor()));

    area = new DrawingArea();
    setCentralWidget(area);
}

void MainWindow::chooseColor() {
    area->setColor(QColorDialog::getColor(Qt::white, this, "Drawing color"));
}

void MainWindow::chooseSize(QAction *action) {
    if(action == actionSizeSmall) area->setSize(1);
    if(action == actionSizeMedium) area->setSize(4);
    if(action == actionSizeBig) area->setSize(10);
}

MainWindow::~MainWindow() {
    delete ui;
    delete area;
}

