#include "mapviewprivate.h"

MapViewPrivate::MapViewPrivate(QWidget *parent)
    : QGraphicsView(parent)
{
    this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
}

MapViewPrivate::MapViewPrivate(QGraphicsScene *scene, QWidget *parent)
    : QGraphicsView(scene,parent)
{
    this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
}

MapViewPrivate::~MapViewPrivate()
{

}

void MapViewPrivate::mouseDoubleClickEvent(QMouseEvent *event)
{
    event->setAccepted(false);
    this->hadMouseDoubleClickEvent(event);
    if (!event->isAccepted())
        QGraphicsView::mouseDoubleClickEvent(event);
}

void MapViewPrivate::mouseMoveEvent(QMouseEvent *event)
{
    event->setAccepted(false);
    this->hadMouseMoveEvent(event);
    if (!event->isAccepted())
        QGraphicsView::mouseMoveEvent(event);
}

void MapViewPrivate::mousePressEvent(QMouseEvent *event)
{
    event->setAccepted(false);
    this->hadMousePressEvent(event);
    if (!event->isAccepted())
        QGraphicsView::mousePressEvent(event);
}

void MapViewPrivate::mouseReleaseEvent(QMouseEvent *event)
{
    event->setAccepted(false);
    this->hadMouseReleaseEvent(event);
    if (!event->isAccepted())
        QGraphicsView::mouseReleaseEvent(event);
}

void MapViewPrivate::contextMenuEvent(QContextMenuEvent *event)
{
    event->setAccepted(false);
    this->hadContextMenuEvent(event);
    if (!event->isAccepted())
        QGraphicsView::contextMenuEvent(event);
}

void MapViewPrivate::wheelEvent(QWheelEvent *event)
{
    event->setAccepted(false);
    this->hadWheelEvent(event);
    if (!event->isAccepted())
        QGraphicsView::wheelEvent(event);
}
