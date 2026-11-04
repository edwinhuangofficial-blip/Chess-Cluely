#include "mainwindow.h" //include the main window declaration
#include "ui_mainwindow.h" //include the interface generated from the ui file

#include "chessboard.h" //make our board class available

#include <QVBoxLayout> //provide a layout that arranges widgets vertically
#include <QIcon> //provide qt's icon class

MainWindow::MainWindow(QWidget *parent) //run when the main window is created
    : QMainWindow(parent) //initialize the underlying qt window
    , ui(new Ui::MainWindow) //create the generated interface helper
{
    ui->setupUi(this); //build the interface described by the ui file

    setWindowTitle("Chess Cluely"); //set the title shown on the window
    QIcon appIcon(":/assets/icon.png"); //load the bundled icon
    setWindowIcon(appIcon); //use it as this window's icon
    
    auto *layout = new QVBoxLayout(ui->centralwidget); //install a layout in the central area
    layout->setContentsMargins(16, 16, 16, 16); //leave space around the board

    auto *board = new ChessBoard(ui->centralwidget); //create a board owned by the central widget
    layout->addWidget(board); //let the layout position and size the board
}

MainWindow::~MainWindow() //run when the main window is destroyed
{
    delete ui; //release the generated interface helper
}