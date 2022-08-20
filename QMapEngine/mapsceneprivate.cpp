#include "mapsceneprivate.h"

#include <QDebug>

MapScenePrivate::MapScenePrivate(MapScene *mgScene, MapTileSourcePrivate *infoSource, QObject *parent)
    : QGraphicsScene(parent)
    , _infoSource(infoSource)
{
    this->setMapGraphicsScene(mgScene);

    connect(this,
            SIGNAL(selectionChanged()),
            this,
            SLOT(handleSelectionChanged()));
}

void MapScenePrivate::handleMGObjectAdded(MapObject *added)
{
    MapObjectPrivate * qgObj = new MapObjectPrivate(added,_infoSource);
    this->addItem(qgObj);

    //We need a mapping of MapGraphicsObject : QGraphicsObject, so put this in the map
    _mgToqg.insert(added,qgObj);
}

void MapScenePrivate::handleMGObjectRemoved(MapObject *removed)
{
    if (!_mgToqg.contains(removed))
    {
        qWarning() << "There is no QGraphicsObject in the scene for" << removed;
        return;
    }

    QGraphicsObject * qgObj = _mgToqg.take(removed);
    delete qgObj;

    /*
      It turns out that removing or "deleting later" the PrivateQGraphicsObjects here was causing crashes.
      Instead, the PrivateQGraphicsObject watches the MapGraphicsObject's destroyed signal to
      delete itself. QGraphicsScene is smart enough to remove deleted objects in this case.
    */
    /*
    if (!this->items().contains(qgObj))
    {
        qWarning() << this << "does not contain PrivateQGraphicsObject" << qgObj;
        return;
    }
    //qgObj->deleteLater();
    //this->removeItem(qgObj);
    */
}

void MapScenePrivate::handleZoomLevelChanged()
{
    foreach(MapObjectPrivate * obj, _mgToqg.values())
        obj->handleZoomLevelChanged();
}

void MapScenePrivate::handleSelectionChanged()
{
    QList<QGraphicsItem *> selectedList = this->selectedItems();
    QSet<QGraphicsItem *> selected;
    foreach(QGraphicsItem * item, selectedList)
        selected.insert(item);

    QList<QGraphicsItem *> newSelections;
    foreach(MapObjectPrivate * obj, _mgToqg.values())
    {
        QGraphicsItem * casted = (QGraphicsItem *) obj;

        bool doSelect = selected.contains(casted) && !_oldSelections.contains(obj);
        obj->setSelected(doSelect);

        if (doSelect)
            newSelections.append(obj);
    }

    _oldSelections = newSelections;
}

void MapScenePrivate::setMapGraphicsScene(MapScene *mgScene)
{
    _mgScene = mgScene;

    if (_mgScene.isNull())
    {
        qWarning() << this << "got a null MapGraphicsScene";
        return;
    }

    connect(_mgScene.data(),
            SIGNAL(objectAdded(MapObject*)),
            this,
            SLOT(handleMGObjectAdded(MapObject*)));
    connect(_mgScene.data(),
            SIGNAL(objectRemoved(MapObject*)),
            this,
            SLOT(handleMGObjectRemoved(MapGObject*)));
}
