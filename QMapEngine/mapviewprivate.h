#ifndef MAPVIEWPRIVATE_H
#define MAPVIEWPRIVATE_H

#include <QGraphicsView>
#include <QKeyEvent>
#include <QWheelEvent>
#include <QContextMenuEvent>
#include <QtDebug>

class MapViewPrivate : public QGraphicsView
{
    Q_OBJECT
public:
    explicit MapViewPrivate(QWidget *parent = 0);
    MapViewPrivate(QGraphicsScene* scene, QWidget * parent=0);
    virtual ~MapViewPrivate();

protected:
    //virtual from QGraphicsView
    virtual void mouseDoubleClickEvent(QMouseEvent *event);

    //virtual from QGraphicsView
    virtual void mouseMoveEvent(QMouseEvent *event);

    //virtual from QGraphicsView
    virtual void mousePressEvent(QMouseEvent *event);

    //virtual from QGraphicsView
    virtual void mouseReleaseEvent(QMouseEvent *event);

    //virtual from QGraphicsView
    virtual void contextMenuEvent(QContextMenuEvent *event);

    //virtual from QGraphicsView
    virtual void wheelEvent(QWheelEvent *event);

signals:
    void hadMouseDoubleClickEvent(QMouseEvent* event);
    void hadMouseMoveEvent(QMouseEvent * event);
    void hadMousePressEvent(QMouseEvent * event);
    void hadMouseReleaseEvent(QMouseEvent * event);
    void hadContextMenuEvent(QContextMenuEvent *);
    void hadWheelEvent(QWheelEvent *);
};

#endif // MAPVIEWPRIVATE_H
