#include "qmapengine.h"

QMapEngine::QMapEngine(QWidget *parent)
    : QWidget(parent)
    , _mapPath("")
    , _center(QPointF())
    , _scaleratio(1.0)
    , _menu(nullptr)
{

}

QMapEngine::~QMapEngine()
{
    if(!_menu) _menu->deleteLater();
}

QString QMapEngine::getMapPath()
{
    return _mapPath;
}

void QMapEngine::setMapPath(QString path)
{
    _mapPath = path;
}

QPointF QMapEngine::getCenter()
{
    return _center;
}

void QMapEngine::setCenter(QPointF p)
{
    _center = p;
}

float QMapEngine::getScaleRatio()
{
    return _scaleratio;
}

void QMapEngine::setScaleRatio(float ratio)
{
    _scaleratio = ratio;
}

QMapEngine::MAPSTATE QMapEngine::getState()
{
    return _mapstate;
}

void QMapEngine::setState(MAPSTATE state)
{
    _mapstate = state;
}

QRectF QMapEngine::getGeoRect()
{
    return _maprect;
}

void QMapEngine::setGeoRect(QRectF rect)
{
    _maprect = rect;
}

float QMapEngine::getRotate()
{
    return _rotate;
}

void QMapEngine::setRotate(float rotate)
{
    _rotate = rotate;
}

QPointF QMapEngine::transfGeo2View(QPointF geo)
{
    return QPointF();
}

QPointF QMapEngine::transfView2Geo(QPointF view)
{
    return QPointF();
}

float QMapEngine::getAltitude()
{
    return 1.0;
}

float QMapEngine::getDistance(QPointF p1, QPointF p2)
{
    return 1.0;
}

float QMapEngine::getAngle(QPointF p1, QPointF p2)
{
    return 1.0;
}

float QMapEngine::getArea(QList<QPointF> ps)
{
    return 1.0;
}

void QMapEngine::setGridVisible(bool b)
{

}

void QMapEngine::addLayer(QMapLayer *layer)
{

}

void QMapEngine::removeLayer(QString name)
{

}

void QMapEngine::removeLayer(QMapLayer *layer)
{

}

QMapEngine::QMapLayer QMapEngine::findLayerByName(QString name)
{
    return QMapLayer();
}

QList<QMapEngine::QMapLayer> QMapEngine::getAllLayers()
{
    return _maplayers;
}

QMenu *QMapEngine::getMenu()
{
    return _menu;
}

void QMapEngine::setMenu(QMenu *m)
{
    _menu = m;
}

void QMapEngine::setDrawBackground(bool b)
{
    _drawbg = b;
}

void QMapEngine::paintEvent(QPaintEvent *)
{
    if(_drawbg){

    }
}

void QMapEngine::mousePressEvent(QMouseEvent *e)
{

}
