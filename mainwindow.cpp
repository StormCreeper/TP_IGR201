#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QColorDialog>
#include <QActionGroup>
#include <QToolBar>
#include <QFileDialog>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) , ui(new Ui::MainWindow) {

    QMenuBar *menuBar = this->menuBar();
    QMenu *menuFile = menuBar->addMenu(tr("&File"));
    QMenu *menuDraw = menuBar->addMenu(tr("&Draw"));

    QAction *actionClearAll = new QAction(QIcon(":/images/clear.png"), tr("&Clear all"));
    QAction *actionChooseColor = new QAction(QIcon(":/images/color.png"), tr("&Choose color"), this);

    menuDraw->addAction((actionClearAll));

    QMenu *menuChooseSize = menuDraw->addMenu(tr("&Size"));
    QMenu *menuChooseTool = menuDraw->addMenu(tr("&Tool"));

    QActionGroup *sizeGroup = new QActionGroup(this);
    connect(sizeGroup, SIGNAL(triggered(QAction*)), this, SLOT(chooseSize(QAction*)));

    actionSizeSmall = sizeGroup->addAction(QIcon(":/images/small.png"), tr("&Small"));
    actionSizeMedium = sizeGroup->addAction(QIcon(":/images/medium.png"), tr("&Medium"));
    actionSizeBig = sizeGroup->addAction(QIcon(":/images/large.png"), tr("&Big"));

    QActionGroup *toolGroup = new QActionGroup(this);
    connect(toolGroup, SIGNAL(triggered(QAction*)), this, SLOT(chooseTool(QAction*)));

    actionToolBrush = toolGroup->addAction(QIcon(":/images/brush.png"), tr("&Brush"));
    actionToolLine = toolGroup->addAction(QIcon(":/images/line.png"), tr("&Line"));
    actionToolRect = toolGroup->addAction(QIcon(":/images/rectangle.png"), tr("&Rectangle"));
    actionToolEllipse = toolGroup->addAction(QIcon(":/images/ellipse.png"), tr("&Ellipse"));

    menuDraw->addAction(actionChooseColor);

    menuChooseSize->addAction(actionSizeSmall);
    menuChooseSize->addAction(actionSizeMedium);
    menuChooseSize->addAction(actionSizeBig);

    menuChooseTool->addAction((actionToolBrush));
    menuChooseTool->addAction((actionToolLine));
    menuChooseTool->addAction((actionToolRect));
    menuChooseTool->addAction((actionToolEllipse));

    connect(actionChooseColor, SIGNAL(triggered()), this, SLOT(chooseColor()));
    connect(actionClearAll, SIGNAL(triggered()), this, SLOT(clearAll()));

    QAction *actionSave = new QAction(QIcon(":/images/save.png"), tr("&Save"));
    QAction *actionOpen = new QAction(QIcon(":/images/open.png"), tr("&Open"));
    QAction *actionSelect = new QAction(QIcon(":/images/select.png"), tr("&Select"));

    actionSave->setShortcut(Qt::Key_S | Qt::CTRL);
    actionOpen->setShortcut(Qt::Key_O | Qt::CTRL);

    actionSelect->setCheckable(true);

    menuFile->addAction(actionSave);
    menuFile->addAction(actionOpen);

    connect(actionSave, SIGNAL(triggered()), this, SLOT(save()));
    connect(actionOpen, SIGNAL(triggered()), this, SLOT(load()));
    connect(actionSelect, SIGNAL(triggered()), this, SLOT(select()));

    QToolBar *toolbar = this->addToolBar(tr("File"));
    toolbar->addAction(actionOpen);
    toolbar->addAction(actionSave);
    toolbar->addAction(actionSelect);

    area = new DrawingArea();
    setCentralWidget(area);

    area->setModified(true);
}

void MainWindow::chooseColor() {
    area->setColor(QColorDialog::getColor(Qt::white, this, "Drawing color"));
}
void MainWindow::clearAll() {
    area->clearAll();
}

void MainWindow::save() {
    if(area->filename.isEmpty()) {
        area->filename = QFileDialog::getSaveFileName(this, tr("Save drawing as"), "C://Users/telop/Desktop", tr("Text files (*.txt)"));
        if(area->filename.isEmpty())
            QMessageBox::information(this, tr("Unable to open file"), "No file selected");
        else
            area->save();
    } else {
        if(area->isModified())
            area->save();
    }
}

void MainWindow::load() {
    area->filename = QFileDialog::getOpenFileName(this, tr("Open drawing"), area->filename.isEmpty() ? "C://Users/telop/Desktop" : area->filename.toStdString().c_str(), tr("Text files (*.txt)"));
    if(area->filename.isEmpty())
        QMessageBox::information(this, tr("Unable to open file"), "No file selected");
    else
        area->load();
}

void MainWindow::select() {
    area->toogleSelect();
}

void MainWindow::chooseSize(QAction *action) {
    if(action == actionSizeSmall) area->setSize(1);
    if(action == actionSizeMedium) area->setSize(4);
    if(action == actionSizeBig) area->setSize(10);
}

void MainWindow::chooseTool(QAction *action) {
    if(action == actionToolBrush) area->setTool(ShapeType::Brush);
    if(action == actionToolLine) area->setTool(ShapeType::Line);
    if(action == actionToolRect) area->setTool(ShapeType::Rect);
    if(action == actionToolEllipse) area->setTool(ShapeType::Ellipse);
}

MainWindow::~MainWindow() {
    delete ui;
    delete area;

    delete actionSizeBig;
    delete actionSizeMedium;
    delete actionSizeSmall;
    delete actionToolBrush;
    delete actionToolLine;
    delete actionToolRect;
    delete actionToolEllipse;
}

