#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "chessboard.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setCentralWidget(new ChessBoard);
    setWindowTitle("Chess Cluely");
    resize(900, 700);
}

MainWindow::~MainWindow()
{
    delete ui;
}
