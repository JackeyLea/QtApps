#ifndef MAPSCENE_H
#define MAPSCENE_H

#include <QObject>
#include <QList>
#include <QSet>

#include "mapobject.h"

class MapScene : public QObject
{
    Q_OBJECT
public:
    MapScene(QObject *parent = nullptr);

    virtual ~MapScene();

    /**
     * @brief Adds or re-parents the given MapGraphicsObject to this scene.
     *
     * @param item
     */
    void addObject(MapObject * object);

    /**
     * @brief Returns a list of pointers to all MapGraphicsObject objects in the scene
     *
     * @return QList<MapGraphicsObject *>
     */
    QList<MapObject *> objects() const;

    void removeObject(MapObject * object);

signals:
    /**
     * @brief Fired when a MapGraphicsObject is added to the scene
     *
     * @param the object that was added
     */
    void objectAdded(MapObject *);

    /**
     * @brief Fired when a MapGraphicsObject is removed from the scene.
     *
     * @param the object that was removed
     */
    void objectRemoved(MapObject *);

private slots:
    void handleNewObjectGenerated(MapObject * newObject);
    void handleObjectDestroyed(QObject * object);

private:
    QSet<MapObject *> _objects;


};

#endif // MAPSCENE_H
