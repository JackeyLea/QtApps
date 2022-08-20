#include "mapscene.h"

#include <QDebug>

MapScene::MapScene(QObject *parent)
    : QObject{parent}
{

}

MapScene::~MapScene()
{
    foreach(MapObject * obj, _objects)
        this->removeObject(obj);
}

void MapScene::addObject(MapObject *object)
{
    if (object == 0)
        return;

    connect(object,
            SIGNAL(newObjectGenerated(MapObject*)),
            this,
            SLOT(handleNewObjectGenerated(MapObject*)));
    connect(object,
            SIGNAL(destroyed(QObject*)),
            this,
            SLOT(handleObjectDestroyed(QObject*)));

    _objects.insert(object);
    this->objectAdded(object);
}

QList<MapObject *> MapScene::objects() const
{
    QList<MapObject *> toRet;

    return toRet;
}

void MapScene::removeObject(MapObject *object)
{
    _objects.remove(object);
    this->objectRemoved(object);
}

void MapScene::handleNewObjectGenerated(MapObject *newObject)
{
    this->addObject(newObject);
}

void MapScene::handleObjectDestroyed(QObject *object)
{
    /*
      We have to be careful with this casted pointer as technically at this point the MapGraphicsObject
      portion of it has been destroyed. This signal comes from the QObject destructor.
    */
    MapObject *mgObj = (MapObject*)object;

    this->removeObject(mgObj);
}
