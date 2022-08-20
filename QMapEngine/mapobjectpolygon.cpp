#include "mapobjectpolygon.h"

#include <QDebug>

MapObjectPolygon::MapObjectPolygon(QPolygonF geoPoly, QColor fillColor, QObject *parent)
    : MapObject(parent)
    , _geoPoly(geoPoly)
    , _fillColor(fillColor)
{
    this->setFlag(MapObject::ObjectIsMovable);
    this->setFlag(MapObject::ObjectIsSelectable,false);
    this->setFlag(MapObject::ObjectIsFocusable);
    this->setGeoPoly(geoPoly);
}

MapObjectPolygon::~MapObjectPolygon()
{
    qDebug() << this << "destroying";
    foreach(MapObject * circle, _editCircles)
        this->destroyEditCircle(circle);
    _editCircles.clear();

    foreach(MapObject * circle, _addVertexCircles)
        this->destroyAddVertexCircle(circle);
    _addVertexCircles.clear();
}

QRectF MapObjectPolygon::boundingRect() const
{
    QRectF latLonRect = _geoPoly.boundingRect();
    QPointF latLonCenter = latLonRect.center();
    Position latLonCenterPos(latLonCenter,0.0);
    Position topLeftPos(latLonRect.topLeft(),0.0);
    Position bottomRightPos(latLonRect.bottomRight(),0.0);

    QPointF topLeftENU = Conversion::lla2enu(topLeftPos,latLonCenterPos).toPointF();
    QPointF bottomRightENU = Conversion::lla2enu(bottomRightPos,latLonCenterPos).toPointF();

    return QRectF(topLeftENU,bottomRightENU);
}

bool MapObjectPolygon::contains(const QPointF &geoPos) const
{
    return _geoPoly.containsPoint(geoPos,
                                  Qt::OddEvenFill);
}

void MapObjectPolygon::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option)
    Q_UNUSED(widget)

    painter->setRenderHint(QPainter::Antialiasing,true);

    QPolygonF enuPoly;

    Position latLonCenterPos(_geoPoly.boundingRect().center(),0);
    foreach(QPointF latLon, _geoPoly)
    {
        Position latLonPos(latLon,0.0);
        QPointF enu = Conversion::lla2enu(latLonPos,latLonCenterPos).toPointF();
        enuPoly << enu;
    }

    painter->setBrush(_fillColor);
    painter->drawPolygon(enuPoly);


    //Populate edit and add-vertex handles if necessary.
    //Is there a better place to do this? Most likely, yes.
    if (_editCircles.isEmpty())
    {
        //Create objects to edit the polygon!
        for (int i = 0; i < _geoPoly.size(); i++)
        {
            //Edit circles - to change the shape
            MapObjectCircle * circle = this->constructEditCircle();
            circle->setPos(_geoPoly.at(i));
            _editCircles.append(circle);

            QPointF current = _geoPoly.at(i);
            QPointF next = _geoPoly.at((i+1) % _geoPoly.size());
            QPointF avg((current.x() + next.x())/2.0,
                        (current.y() + next.y())/2.0);

            //Add vertex circles - to add new vertices
            MapObjectCircle * betweener = this->constructAddVertexCircle();
            betweener->setPos(avg);
            _addVertexCircles.append(betweener);
        }
    }
}

void MapObjectPolygon::setPos(const QPointF &nPos)
{
    /*
      If the new position moved the object from the center of the bounding box made by the geo poly
      then we need to move the geo poly
    */
    if (nPos != _geoPoly.boundingRect().center())
    {
        const QPointF diff = nPos - this->pos();
        //_geoPoly.translate(diff);

        //We should also move our edit handles
        foreach(MapObject * circle, _editCircles)
            circle->setPos(circle->pos() + diff);

        //And our "add vertex" handles
        this->fixAddVertexCirclePos();
    }

    MapObject::setPos(nPos);

    //If this isn't here, we get TEARING when we edit our polygons
    this->posChanged();
}

QPolygonF MapObjectPolygon::geoPoly() const
{
    return _geoPoly;
}

void MapObjectPolygon::setGeoPoly(const QPolygonF &newPoly)
{
    if (newPoly == _geoPoly)
        return;

    _geoPoly = newPoly;

    foreach(MapObject * obj, _editCircles)
        this->destroyEditCircle(obj);
    foreach(MapObject * obj, _addVertexCircles)
        this->destroyAddVertexCircle(obj);
    _editCircles.clear();
    _addVertexCircles.clear();

    this->setPos(newPoly.boundingRect().center());
    this->polygonChanged(newPoly);
}

void MapObjectPolygon::setFillColor(const QColor &color)
{
    if (_fillColor == color)
        return;

    _fillColor = color;
    this->redrawRequested();
}

void MapObjectPolygon::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    /*
      This method is a bit of a hack --- ideally contains() or some other method
      we "reimplement" from PrivateQGraphicsObject should take care of this, but it
      hasn't been working. This prevents the polygon from being grabbed when the user
      click inside the bounding box but outside of the actual polygon
    */
    const QPointF geoPos = event->scenePos();

    //If the geo point is within our geo polygon, we might accept it
    if (_geoPoly.containsPoint(geoPos,Qt::OddEvenFill))
        MapObject::mousePressEvent(event);
    //Otherwise we ignore it
    else
        event->ignore();
}

void MapObjectPolygon::keyReleaseEvent(QKeyEvent *event)
{
    if (event->matches(QKeySequence::Delete))
    {
        this->deleteLater();
        event->accept();
    }
    else
        event->ignore();
}

void MapObjectPolygon::handleEditCirclePosChanged()
{
    QObject * sender = QObject::sender();
    if (sender == 0)
        return;
    MapObjectCircle * circle = qobject_cast<MapObjectCircle *>(sender);
    if (circle == 0)
        return;

    int index = _editCircles.indexOf(circle);
    if (index == -1)
        return;

    QPointF newPos = circle->pos();
    _geoPoly.replace(index,newPos);
    this->setPos(_geoPoly.boundingRect().center());

    //We need to update the positions of our "add vertex" controllers
    this->fixAddVertexCirclePos();

    //Emit a signal so everyone knows that the polygon changed
    this->polygonChanged(this->geoPoly());
}

void MapObjectPolygon::handleAddVertexCircleSelected()
{
    QObject * sender = QObject::sender();
    if (sender == 0)
        return;
    MapObjectCircle * circle = qobject_cast<MapObjectCircle *>(sender);
    if (circle == 0)
        return;

    //If the circle isn't selected, something is wrong
    if (!circle->isSelected())
        return;

    //Now that we know the circle was selected, just deselect it. We don't need it selected actually
    circle->setSelected(false);

    //Get the position at which we should add a vertex
    QPointF geoPos = circle->pos();

    //The index at which we should insert the new vertex
    int index = _addVertexCircles.indexOf(circle);
    if (index == -1)
        return;
    index++;

    //Put the vertex in the polygon
    _geoPoly.insert(index,geoPos);

    //Create a new "Edit Circle" and put it in the right spot
    MapObjectCircle * editCircle = this->constructEditCircle();
    editCircle->setPos(geoPos);
    _editCircles.insert(index,editCircle);

    editCircle->setSelected(true);


    //Create a new "Add vertex" circle and put it in the right spot
    MapObjectCircle * addVertexCircle = this->constructAddVertexCircle();
    QPointF current = _geoPoly.at(index-1);
    QPointF next = _geoPoly.at(index);
    QPointF avg((current.x() + next.x())/2.0,
                (current.y() + next.y())/2.0);
    addVertexCircle->setPos(avg);
    _addVertexCircles.insert(index,addVertexCircle);

    this->fixAddVertexCirclePos();

    //Emit a signal so everyone knows that the polygon changed
    this->polygonChanged(this->geoPoly());
}

void MapObjectPolygon::handleEditCircleDestroyed()
{
    QObject * sender = QObject::sender();
    if (sender == 0)
    {
        qWarning() << "Can't process desyroyed edit circle. Sender is null";
        return;
    }
    MapObjectCircle * circle = (MapObjectCircle *) sender;

    int index = _editCircles.indexOf(circle);
    if (index == -1)
    {
        qWarning() << "Can't process destroyed edit circle. Not contained in edit circle list";
        return;
    }

    _geoPoly.remove(index);
    _editCircles.removeAt(index);
    _addVertexCircles.takeAt(index)->deleteLater();

    this->fixAddVertexCirclePos();
    this->redrawRequested();
    this->setPos(_geoPoly.boundingRect().center());
}

void MapObjectPolygon::fixAddVertexCirclePos()
{
    for (int i = 0; i < _geoPoly.size(); i++)
    {
        QPointF current = _geoPoly.at(i);
        QPointF next = _geoPoly.at((i+1) % _geoPoly.size());
        QPointF avg((current.x() + next.x())/2.0,
                    (current.y() + next.y())/2.0);
        _addVertexCircles.at(i)->setPos(avg);
    }
}

MapObjectCircle *MapObjectPolygon::constructEditCircle()
{
    MapObjectCircle * toRet = new MapObjectCircle(8);
    connect(toRet,
            SIGNAL(posChanged()),
            this,
            SLOT(handleEditCirclePosChanged()));
    connect(toRet,
            SIGNAL(destroyed()),
            this,
            SLOT(handleEditCircleDestroyed()));

    this->newObjectGenerated(toRet);
    return toRet;
}

void MapObjectPolygon::destroyEditCircle(MapObject *obj)
{
    disconnect(obj,
               SIGNAL(posChanged()),
               this,
               SLOT(handleEditCirclePosChanged()));
    disconnect(obj,
               SIGNAL(destroyed()),
               this,
               SLOT(handleEditCircleDestroyed()));
    obj->deleteLater();
}

MapObjectCircle *MapObjectPolygon::constructAddVertexCircle()
{
    MapObjectCircle * toRet = new MapObjectCircle(3,
                                            true,
                                            QColor(100,100,100,255));
    toRet->setFlag(MapObject::ObjectIsMovable,false);
    connect(toRet,
            SIGNAL(selectedChanged()),
            this,
            SLOT(handleAddVertexCircleSelected()));

    this->newObjectGenerated(toRet);
    toRet->setToolTip("Single-click (don't drag!) to add vertex.");
    return toRet;
}

void MapObjectPolygon::destroyAddVertexCircle(MapObject *obj)
{
    disconnect(obj,
               SIGNAL(selectedChanged()),
               this,
               SLOT(handleAddVertexCircleSelected()));
    obj->deleteLater();
}
