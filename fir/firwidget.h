/////////////////////////////////////////////////
/// \date 20220520
/// \author jackey lea
/// \note 五子棋界面绘制类
/////////////////////////////////////////////////

#ifndef FIRWIDGET_H
#define FIRWIDGET_H

#include <QFrame>
#include <QPainter>
#include <QPaintEvent>
#include <QPen>
#include <QDebug>
#include <QMouseEvent>
#include <QtMath>

//全局常量
const int kbMargin = 20;//棋盘边缘空白
const int kbRadius = 10;//棋子半径
const int kbMarkRadius = 6;//落子标记边长
const int kbBlockLength = 30;//格子大小
const int kbBlockSize = 20;//格子数量
const int kbPosDelta = 15;//鼠标点击模糊距离上限

class FIRWidget : public QFrame
{
    Q_OBJECT
public:
    FIRWidget(QWidget *parent = nullptr);

    void setStart(bool state);

    void resetGame();

    void setWhiteFirst(bool state);

    void chessOneByPerson();

    bool isWin(int row,int col);

signals:
    void showMsg(int type);

protected:
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void paintEvent(QPaintEvent *event);

private:
    int boardMap[kbBlockSize+1][kbBlockSize+1];//保存棋盘状态的数组
    int clickPosRow,clickPosCol;//当前处理点的行和列
    bool isWhitePlayer;
    bool isStart;
};

#endif // FIRWIDGET_H
