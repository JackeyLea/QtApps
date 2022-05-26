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
    boardMap[kbBlockSize/2][kbBlockSize/2]=1;
    boardMap[kbBlockSize/2][kbBlockSize/2+1]=-1;
    boardMap[kbBlockSize/2+1][kbBlockSize/2]=-1;
    boardMap[kbBlockSize/2+1][kbBlockSize/2+1]=1;
}

void FIRWidget::setStart(bool state)
{
    isStart = state;
}

void FIRWidget::setSize(int value)
{
    kbBlockSize=value;
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
    //判断条件为1棋盘全满2哪种颜色多哪个胜
   //落子之后，进行黑白棋反转
    int rc = boardMap[clickPosRow][clickPosCol];
    int left=-1,top=-1,bottom=-1,right=-1;
    //水平左侧
    for(int i=clickPosCol-1;i>=0;i--){
        if(boardMap[clickPosRow][i]==0){
            left=-1;
            break;
        }else if(boardMap[clickPosRow][i]==rc){
            left=i;
            break;
        }
    }
    //修改睡觉
    if(left!=-1){
        for(int i=left;i<clickPosCol;i++){
                            boardMap[clickPosRow][i]=rc;
        }
    }
    //水平右侧
    for(int i=clickPosCol+1;i<kbBlockSize+1;i++){
        if(boardMap[clickPosRow][i]==0){
            right=-1;
            break;
        }else if(boardMap[clickPosRow][i]==rc){
            right=i;
            break;
        }
    }
    if(right!=-1){
        for(int i=clickPosCol+1;i<right;i++){
                boardMap[clickPosRow][i]=rc;
        }
    }
    //垂直上方
    for(int i=clickPosRow-1;i>=0;i--){
        if(boardMap[i][clickPosCol]==0){
            top=-1;
            break;
        }else if(boardMap[i][clickPosCol]==rc){
            top=i;
            break;
        }
    }
    if(top!=-1){
        for(int i=top+1;i<clickPosRow;i++){
            boardMap[i][clickPosCol]=rc;
        }
    }
    //垂直下方
    for(int i=clickPosRow+1;i<kbBlockSize+1;i++){
        if(boardMap[i][clickPosCol]==0){
            bottom=-1;
            break;
        }else if(boardMap[i][clickPosCol]==rc){
            bottom=i;
            break;
        }
    }
    if(bottom!=-1){
        for(int i=clickPosRow+1;i<bottom;i++){
            boardMap[i][clickPosCol]=rc;
        }
    }

    top=-1;
    left=-1;
    bottom=-1;
    right=-1;
    //左斜上方
    for(int i=clickPosRow-1,j=clickPosCol-1;i>=0,j>=0;i--,j--){
        if(boardMap[i][j]==0){
            top=-1;
            left=-1;
            break;
        }else if(boardMap[i][j]==rc){
            top=i;
            left=j;
            break;
        }
    }
    if(top!=-1 && left!=-1){
        for(int i=top,j=left;i<clickPosRow,j<clickPosCol;i++,j++){
            boardMap[i][j]=rc;
        }
    }

    //左斜下方
    for(int i=clickPosRow+1,j=clickPosCol+1;i<kbBlockSize+1,j<kbBlockSize+1;i++,j++){
        if(boardMap[i][j]==0){
            bottom=-1;
            right=-1;
            break;
        }else if(boardMap[i][j]==rc){
            bottom=i;
            right=j;
            break;
        }
    }
    if(bottom!=-1 && right!=-1){
        for(int i=clickPosRow+1,j=clickPosCol+1;i<=bottom,j<=right;i++,j++){
            boardMap[i][j]=rc;
        }
    }
    top=-1;
    left=-1;
    bottom=-1;
    right=-1;
    //右斜下方
    for(int i=clickPosRow+1,j=clickPosCol-1;i<kbBlockSize+1,j>=0;i++,j--){
        if(boardMap[i][j]==0){
            bottom=-1;
            left=-1;
            break;
        }else if(boardMap[i][j]==rc){
            bottom=i;
            left=j;
            break;
        }
    }
    if(bottom!=-1 && left!=-1){
        for(int i=clickPosRow+1,j=clickPosCol-1;i<=bottom,j>=left;i++,j--){
            boardMap[i][j]=rc;
        }
    }

    //右斜上方
    for(int i=clickPosRow-1,j=clickPosCol+1;i>=0,j<kbBlockSize+1;i--,j++){
        if(boardMap[i][j]==0){
            top=-1;
            right=-1;
            break;
        }else if(boardMap[i][j]==rc){
            top=i;
            right=j;
            break;
        }
    }
    if(top!=-1 && right!=-1){
        for(int i=clickPosRow-1,j=clickPosCol+1;i>=top,j<=right;i--,j++){
            boardMap[i][j]=rc;
        }
    }

    int whiteNum=0,blackNum=0;
    for(int i=0;i<kbBlockSize+1;i++){
        for(int j=0;j<kbBlockSize+1;j++){
            if(boardMap[i][j]==1){
                whiteNum++;
            }else if(boardMap[i][j]==-1){
                blackNum++;
            }else{
                return false;
            }
        }
    }
    if(whiteNum==blackNum) return 0;
    if(whiteNum>blackNum) return 1;
    if(whiteNum<blackNum) return -1;

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
