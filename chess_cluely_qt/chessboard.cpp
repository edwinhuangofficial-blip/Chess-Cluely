#include "chessboard.h" //include our class declaration

#include <QColor> //represent square colors
#include <QPainter> //provide drawing tools
#include <QRectF> //represent rectangles with fractional coordinates
#include <QFont> //provide font settings
#include <QString> //provide qt's text type
#include <QDebug>
#include <QMouseEvent>

#include <algorithm> //provide std::min
#include <cstdlib> //provide std::abs for integers

ChessBoard::ChessBoard(QWidget *parent) //run when a board is created
    : QWidget(parent) //initialize the qt widget with its parent
{
    setMinimumSize(240, 240); //prevent the board widget from becoming too small
}
const double labelSpace = 24.0; //reserve space around the board for labels

QRectF ChessBoard::boardRect() const
{
    const double boardSize = std::min(width(), height()) - 2.0 * labelSpace;
    const double offsetX = (width() - boardSize) / 2.0;
    const double offsetY = (height() - boardSize) / 2.0;

    return QRectF(offsetX, offsetY, boardSize, boardSize); //return the centered board rectangle
}

void ChessBoard::mousePressEvent(QMouseEvent *event)
{
    if (event->button() != Qt::LeftButton) {
        return; //only handle left clicks
    }

    const QRectF board = boardRect();
    const QPointF click = event->position();
    if (click.x() < board.left() || click.x() >= board.right() ||
        click.y() < board.top() || click.y() >= board.bottom()) {
        return; //ignore clicks on the labels or outside the board
    }

    const double squareSize = board.width() / 8.0;
    const int column = static_cast<int>((click.x() - board.left()) / squareSize);
    const int row = static_cast<int>((click.y() - board.top()) / squareSize);

    const QString piece = positionRows[row].mid(column, 1);
    const bool empty = piece == ".";

    if (selectedRow == -1 && empty) {
        return; //there is no piece to select or move
    }

    QString movingPiece;
    if (selectedRow != -1) {
        movingPiece = positionRows[selectedRow].mid(selectedColumn, 1);
    }

    const bool canTryCapture = selectedRow != -1 && !empty
        && piece != "K" && piece != "k"
        && (piece.at(0).isUpper() != movingPiece.at(0).isUpper());

    if (empty || canTryCapture) {
        if (movingPiece == "P" || movingPiece == "p") {
            const bool white = movingPiece == "P";
            const int nextRow = selectedRow + (white ? -1 : 1);

            if (empty) {
                const bool openingRows = white ? (selectedRow == 6 && row == 4)
                                            : (selectedRow == 1 && row == 3);
                const bool canMoveTwo = openingRows
                    && positionRows[nextRow].mid(selectedColumn, 1) == ".";

                if (column != selectedColumn || (row != nextRow && !canMoveTwo)) {
                    return; //a pawn moves straight into an empty square
                }
            } else {
                const int columnDistance = std::abs(column - selectedColumn);
                if (row != nextRow || columnDistance != 1) {
                    return; //a pawn captures one square diagonally forward
                }
            }
        }
    if (piece == ".") {
        if (selectedRow == -1) {
            return; //an empty square only acts as a destination if a piece is selected
        }

        const QString movingPiece = positionRows[selectedRow].mid(selectedColumn, 1);

    if (movingPiece == "P" || movingPiece == "p") {//PAWN RULES OF MOVING
        const bool white = movingPiece == "P";
        const int nextRow = selectedRow + (white ? -1 : 1);
        const bool openingRows = white ? (selectedRow == 6 && row == 4)
                                    : (selectedRow == 1 && row == 3);
        const bool canMoveTwo = openingRows && positionRows[nextRow].mid(selectedColumn, 1) == ".";

        if (column != selectedColumn || (row != nextRow && !canMoveTwo)) {
            return; //reject sideways moves and moves beyond the allowed distance
        }
    }

    if (movingPiece == "N" || movingPiece == "n") {//KNIGHT RULES
        const int rowDistance = std::abs(row - selectedRow);
        const int columnDistance = std::abs(column - selectedColumn);

        const bool twoRowsOneColumn = rowDistance == 2 && columnDistance == 1;
        const bool oneRowTwoColumns = rowDistance == 1 && columnDistance == 2;

        if (!twoRowsOneColumn && !oneRowTwoColumns) {
            return; //keep the knight selected if neither l-shape matches
        }
    }
    const bool isRook = movingPiece == "R" || movingPiece == "r";
    const bool isQueen = movingPiece == "Q" || movingPiece == "q";
    const bool isBishop = movingPiece == "B" || movingPiece == "b";

    if (isRook || isQueen || isBishop) {
        const int rowChange = row - selectedRow;
        const int columnChange = column - selectedColumn;
        const bool straight = rowChange == 0 || columnChange == 0;
        const bool diagonal = std::abs(rowChange) == std::abs(columnChange);

        const bool allowedStraight = straight && (isRook || isQueen);
        const bool allowedDiagonal = diagonal && (isBishop || isQueen);

        if (!allowedStraight && !allowedDiagonal) {
            return; //the move shape must match the selected piece
        }

        const int distance = std::max(std::abs(rowChange), std::abs(columnChange));
        const int rowStep = rowChange / distance;
        const int columnStep = columnChange / distance;

        for (int step = 1; step < distance; ++step) {
            const int pathRow = selectedRow + step * rowStep;
            const int pathColumn = selectedColumn + step * columnStep;

            if (positionRows[pathRow].mid(pathColumn, 1) != ".") {
                return; //a piece blocks the path
            }
        }
    }
    if (movingPiece == "K" || movingPiece == "k") {
    const int rowDistance = std::abs(row - selectedRow);
    const int columnDistance = std::abs(column - selectedColumn);

    if (rowDistance > 1 || columnDistance > 1) {
        return; //keep the king selected if either distance is too large
    }
}
        positionRows[row].replace(column, 1, movingPiece); //put the piece on the clicked square
        positionRows[selectedRow].replace(selectedColumn, 1, "."); //empty the original square

        selectedRow = -1;
        selectedColumn = -1;
        update(); //redraw the board from the updated position
        return; //finish the move without selecting the destination
    }

    selectedRow = row;
    selectedColumn = column;
    update(); //ask qt to redraw the widget with the new selection
}

void ChessBoard::paintEvent(QPaintEvent *) //draw whenever qt requests a repaint
{
    QPainter painter(this); //draw onto this board widget

    const QColor lightSquare("#7b7979"); //set the light square color
    const QColor darkSquare("#65c9ff"); //set the dark square color

    const QRectF board = boardRect(); //use the same rectangle for drawing and clicks
    const double boardSize = board.width();
    const double squareSize = boardSize / 8.0;
    const double offsetX = board.x();
    const double offsetY = board.y();

    for (int row = 0; row < 8; ++row) { //visit each row from top to bottom
        for (int column = 0; column < 8; ++column) { //visit each column from left to right
            const bool isLightSquare = (row + column) % 2 == 0; //alternate colors using even and odd sums

            const QRectF square( //describe the position and size of this square
                offsetX + column * squareSize, //calculate its horizontal position
                offsetY + row * squareSize, //calculate its vertical position
                squareSize, //set its width
                squareSize //set its height
            );

            if (row == selectedRow && column == selectedColumn) {
                painter.fillRect(square, QColor("#d4a72c")); //highlight the selected square
            } else {
                painter.fillRect(square, isLightSquare ? lightSquare : darkSquare);
            }
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
