#ifndef MAPGRAPHICSSCENE_H
#define MAPGRAPHICSSCENE_H

#include <QObject>

class MapGraphicsScene : public QObject
{
    Q_OBJECT
public:
    explicit MapGraphicsScene(QObject *parent = 0);
    virtual ~MapGraphicsScene();

    /*
     * @brief
     */

signals:

public slots:
};

#endif // MAPGRAPHICSSCENE_H
