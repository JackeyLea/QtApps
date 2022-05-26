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
const int kbMarkRadius = 6;//落子标记边长
static int kbBlockSize = 10;//格子数量
static int kbBlockLength = 600/kbBlockSize;//格子大小
static int kbPosDelta = kbBlockLength/2;//鼠标点击模糊距离上限
static int kbRadius = kbBlockLength/3;//棋子半径

class FIRWidget : public QFrame
{
    Q_OBJECT
public:
    FIRWidget(QWidget *parent = nullptr);

    void setStart(bool state);

    void setSize(int value);

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
    int boardMap[100][100];//保存棋盘状态的数组
    int clickPosRow,clickPosCol;//当前处理点的行和列
    bool isWhitePlayer;
    bool isStart;
};

#endif // FIRWIDGET_H
