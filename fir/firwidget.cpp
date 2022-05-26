#include "firwidget.h"

FIRWidget::FIRWidget(QWidget *parent)
    : QFrame(parent)
    , isStart(false)
    , clickPosCol(0)
    , clickPosRow(0)
    , isWhitePlayer(true)
{
    setMouseTracking(true);//捕获鼠标的事件
    for(int i=0;i<kbBlockSize + 1;i++){
        for(int j=0;j<kbBlockSize+1;j++){
            boardMap[i][j]=0;
        }
    }
}

void FIRWidget::setStart(bool state)
{
    isStart = state;
}

void FIRWidget::resetGame()
{
    //结束之后将所有变量复位
    for(int i=0;i<kbBlockSize + 1;i++){
        for(int j=0;j<kbBlockSize+1;j++){
            boardMap[i][j]=0;
        }
    }
    clickPosCol=-1;
    clickPosRow=-1;
}

void FIRWidget::setWhiteFirst(bool state)
{
    isWhitePlayer=state;
}

void FIRWidget::chessOneByPerson()
{
    //根据当前存储的坐标下棋，坐标要有效
    if(clickPosRow !=-1 && clickPosCol !=-1 && boardMap[clickPosRow][clickPosCol]==0){
        boardMap[clickPosRow][clickPosCol]=isWhitePlayer?1:-1;
        //判断是否结束
        if(isWin(clickPosRow,clickPosCol)){
            emit showMsg(isWhitePlayer?1:-1);
        }
        isWhitePlayer=!isWhitePlayer;
        update();
    }
}

bool FIRWidget::isWin(int row, int col)
{
    //需要判断横/竖/斜上/斜下
    //水平方向
    for(int i=0;i<5;i++){
        if(col -i > 0 &&
            col -i + 4 < kbBlockSize+1 &&
            boardMap[row][col -i]== boardMap[row][col - i +1] &&
            boardMap[row][col -i]== boardMap[row][col - i +2] &&
            boardMap[row][col -i]== boardMap[row][col - i +3] &&
            boardMap[row][col -i]== boardMap[row][col - i +4] )
            return true;
    }

    //竖直方向
    for(int i=0;i<5;i++){
        if(row -i > 0 &&
            row -i + 4 < kbBlockSize+1 &&
            boardMap[row-i][col]== boardMap[row - i + 1][col] &&
            boardMap[row-i][col]== boardMap[row - i + 2][col] &&
            boardMap[row-i][col]== boardMap[row - i + 3][col] &&
            boardMap[row-i][col]== boardMap[row - i + 4][col] )
            return true;
    }

    // 左斜方向
    for (int i = 0; i < 5; i++){
        if (row + i < kbBlockSize+1 &&
            (row + i - 4 > 0) &&
            (col - i > 0) &&
            col - i + 4 < kbBlockSize+1 &&
            boardMap[row + i][col - i] == boardMap[row + i - 1][col - i + 1] &&
            boardMap[row + i][col - i] == boardMap[row + i - 2][col - i + 2] &&
            boardMap[row + i][col - i] == boardMap[row + i - 3][col - i + 3] &&
            boardMap[row + i][col - i] == boardMap[row + i - 4][col - i + 4])
            return true;
    }

    // 右斜方向
    for (int i = 0; i < 5; i++){
        if (row - i > 0 &&
            row - i + 4 < kbBlockSize+1 &&
            col - i > 0 &&
            col - i + 4 < kbBlockSize+1 &&
            boardMap[row - i][col - i] == boardMap[row - i + 1][col - i + 1] &&
            boardMap[row - i][col - i] == boardMap[row - i + 2][col - i + 2] &&
            boardMap[row - i][col - i] == boardMap[row - i + 3][col - i + 3] &&
            boardMap[row - i][col - i] == boardMap[row - i + 4][col - i + 4])
            return true;
    }

    return false;
}

void FIRWidget::mouseMoveEvent(QMouseEvent *event)
{
    //通过鼠标的hover确定落子标记
    int x = event->x();
    int y = event->y();

    clickPosCol = -1;
    clickPosRow = -1;

    //先把棋盘外的坐标过滤掉
    if(x>= kbMargin &&
            x <= width() - kbMargin &&
            y >= kbMargin &&
            y <= height() - kbMargin){
        //获取最近的左上角的点
        int col = x / kbBlockLength;
        int row = y /kbBlockLength;

        //计算得到棋盘中靠近点
        int leftTopPosX = kbMargin + kbBlockLength * col;
        int leftTopPosY = kbMargin + kbBlockLength * row;

        int len = 0;//计算结果取整

        //计算距离，根据半径选择
        //最靠近点
        len = sqrt(pow(x-leftTopPosX,2)+pow(y-leftTopPosY,2));
        if(len < kbPosDelta){
            clickPosRow = row;
            clickPosCol = col;
        }
        //最靠近点水平右侧的点
        len = sqrt(pow(x-leftTopPosX - kbBlockLength,2)+pow(y - leftTopPosY,2));
        if (len < kbPosDelta)
        {
            clickPosRow = row;
            clickPosCol = col + 1;
        }
        //最靠近点垂直下方的点
        len = sqrt(pow(x - leftTopPosX,2) + pow(y - leftTopPosY - kbBlockLength,2) );
        if (len < kbPosDelta)
        {
            clickPosRow = row + 1;
            clickPosCol = col;
        }
        //最靠近点右斜下方点
        len = sqrt(pow(x - leftTopPosX - kbBlockLength,2) + pow(y - leftTopPosY - kbBlockLength,2));
        if (len < kbPosDelta)
        {
            clickPosRow = row + 1;
            clickPosCol = col + 1;
        }
    }

    //存了坐标后要重绘
    update();
}

void FIRWidget::mouseReleaseEvent(QMouseEvent *event)
{
    chessOneByPerson();
}

void FIRWidget::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setPen(Qt::green);
    painter.setRenderHint(QPainter::Antialiasing);

    //绘制网格
    for(int i=0;i<kbBlockSize+1;i++){
        painter.drawLine(kbMargin + kbBlockLength * i, kbMargin ,kbMargin + kbBlockLength * i, kbMargin + kbBlockLength * kbBlockSize);
        painter.drawLine(kbMargin, kbMargin + kbBlockLength * i, kbMargin + kbBlockLength * kbBlockSize , kbMargin + kbBlockLength * i);
    }

    if(!isStart) return;

    //落子标志
    if(clickPosRow >= 0 && clickPosRow < kbBlockSize+1 &&
            clickPosCol >= 0 && clickPosCol < kbBlockSize+1 &&
            boardMap[clickPosRow][clickPosCol]==0){
        painter.setBrush(isWhitePlayer?Qt::white:Qt::black);
        painter.drawRect(kbMargin + kbBlockLength * clickPosCol - kbMarkRadius /2,
                         kbMargin + kbBlockLength * clickPosRow - kbMarkRadius/2,
                         kbMarkRadius, kbMarkRadius);
    }

    //绘制棋子
    for( int i=0;i< kbBlockSize+ 1;i++){
        for(int j=0;j<kbBlockSize + 1;j++){
            if(boardMap[i][j]==1){
                painter.setBrush(Qt::white);
                painter.drawEllipse(kbMargin + kbBlockLength * j - kbRadius ,
                                    kbMargin + kbBlockLength * i - kbRadius,
                                    kbRadius * 2, kbRadius *2);
            }else if(boardMap[i][j]==-1){
                painter.setBrush(Qt::black);
                painter.drawEllipse(kbMargin + kbBlockLength * j - kbRadius ,
                                    kbMargin + kbBlockLength * i - kbRadius,
                                    kbRadius * 2, kbRadius *2);
            }
        }
    }
}
