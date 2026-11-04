#include "chessboard.h" //include our class declaration

#include <QColor> //represent square colors
#include <QPainter> //provide drawing tools
#include <QRectF> //represent rectangles with fractional coordinates
#include <QFont> //provide font settings
#include <QString> //provide qt's text type

#include <algorithm> //provide std::min

ChessBoard::ChessBoard(QWidget *parent) //run when a board is created
    : QWidget(parent) //initialize the qt widget with its parent
{
    setMinimumSize(240, 240); //prevent the board widget from becoming too small
}

void ChessBoard::paintEvent(QPaintEvent *) //draw whenever qt requests a repaint
{
    QPainter painter(this); //draw onto this board widget

    const QColor lightSquare("#636161"); //set the light square color
    const QColor darkSquare("#389547"); //set the dark square color

    const double labelSpace = 24.0; //reserve space for labels on each edge
    const double boardSize = std::min(width(), height()) - 2.0 * labelSpace; //leave room for labels on both sides
    const double squareSize = boardSize / 8.0; //divide the board into eight equal parts

    const double offsetX = (width() - boardSize) / 2.0; //center the board horizontally
    const double offsetY = (height() - boardSize) / 2.0; //center the board vertically

    for (int row = 0; row < 8; ++row) { //visit each row from top to bottom
        for (int column = 0; column < 8; ++column) { //visit each column from left to right
            const bool isLightSquare = (row + column) % 2 == 0; //alternate colors using even and odd sums

            const QRectF square( //describe the position and size of this square
                offsetX + column * squareSize, //calculate its horizontal position
                offsetY + row * squareSize, //calculate its vertical position
                squareSize, //set its width
                squareSize //set its height
            );

            painter.fillRect( //fill the square with its chosen color
                square,
                isLightSquare ? lightSquare : darkSquare //choose light when true and dark otherwise
            );
        }
    }
    painter.save(); //remember the painter's current settings

    QFont pieceFont = painter.font();
    pieceFont.setPixelSize(static_cast<int>(squareSize * 0.8)); //use 80 percent of the square height
    painter.setFont(pieceFont);
    painter.setPen(Qt::black); //draw the piece in black

    const QString pieceIds = "kqrbnpKQRBNP";
    const QString pieceSymbols = QStringLiteral(
        "\u265A\u265B\u265C\u265D\u265E\u265F" //black king, queen, rook, bishop, knight, pawn
    );

    for (int row = 0; row < 8; ++row) {
        for (int column = 0; column < 8; ++column) {
            const QString pieceId = positionRows[row].mid(column, 1); //read this square from the current position
            const int symbolIndex = pieceIds.indexOf(pieceId);
            if (symbolIndex == -1) {
                continue; //skip this square and move to the next column
            }

            const QRectF pieceSquare(
                offsetX + column * squareSize,
                offsetY + row * squareSize,
                squareSize,
                squareSize
            );

            int shapeIndex = symbolIndex;
            if (symbolIndex >= 6) {
                shapeIndex = symbolIndex - 6; //look up the matching solid shape for white
                painter.setPen(Qt::white);
            } else {
                painter.setPen(Qt::black);
            }

            painter.drawText(pieceSquare, Qt::AlignCenter, pieceSymbols.mid(shapeIndex, 1));
                    }
                }

    painter.restore(); //return to the previous settings before drawing the labels

    QFont labelFont = painter.font(); //copy the painter's current font
    labelFont.setPixelSize(18); //set the text size to eighteen pixels
    painter.setFont(labelFont); //use the updated font for subsequent text

    const QString files = "abcdefgh"; //store the eight column letters

    for (int column = 0; column < 8; ++column) { //visit columns zero through seven
        const QString label = files.mid(column, 1); //extract this column's letter

        const QRectF bottomLabel( //define the text area below this column
            offsetX + column * squareSize, //move right by one square for each column
            offsetY + boardSize, //start the label area at the board's bottom edge
            squareSize, //match the column's width
            labelSpace //use the reserved border height
        );

        painter.drawText(bottomLabel, Qt::AlignCenter, label); //center this column's letter
    }
    const QString nums = "87654321"; //store the rank numbers from top to bottom

    for (int row = 0; row < 8; ++row) { //visit each row from top to bottom
        const QString label = nums.mid(row, 1); //extract this row's number

        const QRectF sideLabel( //define an area to the right of this row
            offsetX - labelSpace, //x stays at the board's right edge
            offsetY + row * squareSize, //y moves down with each row
            labelSpace, //make the area narrow
            squareSize //make it one row tall
        );

        painter.drawText(sideLabel, Qt::AlignCenter, label); //center this row's number
    }
}
