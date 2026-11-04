#pragma once

#include <QWidget>

class ChessBoard : public QWidget
{
protected:
    void paintEvent(QPaintEvent *event) override;
};
