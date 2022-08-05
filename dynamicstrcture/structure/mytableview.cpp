#include "mytableview.h"

MyTableView::MyTableView(QWidget *parent)
    :QTableView(parent)
{

}

void MyTableView::mousePressEvent(QMouseEvent *event)
{
    int xPos = event->x();
    int yPos = event->y();

    QModelIndex idx = indexAt(QPoint(xPos,yPos));
    if(idx.isValid()){
    }else{
        emit clickBlankArea();
    }
    QTableView::mousePressEvent(event);
}

