#ifndef MAPSCENEPRIVATE_H
#define MAPSCENEPRIVATE_H

#include "mapscene.h"
#include "mapobject.h"
#include "mapobjectprivate.h"
#include "maptilesourceprivate.h"

#include "conversion.h"

#include <QGraphicsScene>
#include <QHash>
#include <QPointer>
#include <QWeakPointer>

class MapObjectPrivate;
class Conversion;

class MapScenePrivate : public QGraphicsScene
{
    Q_OBJECT
public:
    explicit MapScenePrivate(MapScene * mgScene,
                             MapTileSourcePrivate * infoSource,
                             QObject *parent = 0);


private slots:
    void handleMGObjectAdded(MapObject *);
    void handleMGObjectRemoved(MapObject *);
    void handleZoomLevelChanged();

    void handleSelectionChanged();

private:
    void setMapGraphicsScene(MapScene * mgScene);

    QPointer<MapScene> _mgScene;
    MapTileSourcePrivate * _infoSource;

    QHash<MapObject *,MapObjectPrivate *> _mgToqg;

    QList<QGraphicsItem *> _oldSelections;
};

#endif // MAPSCENEPRIVATE_H
