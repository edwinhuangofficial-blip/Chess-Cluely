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
private:
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