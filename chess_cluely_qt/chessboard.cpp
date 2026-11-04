#include "chessboard.h"

#include <QPainter>
#include <algorithm>

void ChessBoard::paintEvent(QPaintEvent *)
{
    constexpr int squaresPerSide = 8;
    const int squareSize = std::min(width(), height()) / squaresPerSide;
    const int boardSize = squareSize * squaresPerSide;
    const int left = (width() - boardSize) / 2;
    const int top = (height() - boardSize) / 2;

    QPainter painter(this);
    for (int row = 0; row < squaresPerSide; ++row) {
        for (int col = 0; col < squaresPerSide; ++col) {
            const int x = left + col * squareSize;
            const int y = top + row * squareSize;
            const auto color = (row + col) % 2 == 0 ? Qt::lightGray : Qt::darkGray;
            painter.fillRect(x, y, squareSize, squareSize, color);
        }
    }
}
