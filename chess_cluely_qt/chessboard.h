#ifndef CHESSBOARD_H //prevent this header from being included more than once
#define CHESSBOARD_H

#include <QWidget> //provide qt's base class for visible components
#include <QString>

class ChessBoard : public QWidget //make our board a type of qt widget
{
public:
    explicit ChessBoard(QWidget *parent = nullptr); //declare how to create a board

protected:
    void paintEvent(QPaintEvent *event) override; //declare our custom drawing function
    void mousePressEvent(QMouseEvent *event) override; //handle mouse presses on the board widget
private:
    QRectF boardRect() const; //calculate the board rectangle from the current widget size
    
    int selectedRow = -1; //start with no selected row
    int selectedColumn = -1; //start with no selected column
    
    QString positionRows[8] = { //keep the current position between repaints
        "rnbqkbnr",
        "pppppppp",
        "........",
        "........",
        "........",
        "........",
        "PPPPPPPP",
        "RNBQKBNR"
    };
};

#endif //end the header guard