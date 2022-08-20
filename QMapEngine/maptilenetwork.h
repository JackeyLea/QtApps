/////////////////////////
/// \author jackey lea
/// \date 2022.08.17
/// \note 用于获取地图瓦片
/////////////////////////

#ifndef MAPTILENETWORK_H
#define MAPTILENETWORK_H

#include <QMutex>
#include <QNetworkAccessManager>
#include <QHash>

class MapTileNetwork
{
public:
    static MapTileNetwork * getInstance();

    ~MapTileNetwork();

    QNetworkReply * get(QNetworkRequest& request);

    void setUserAgent(const QByteArray& agent);
    QByteArray userAgent() const;

protected:
    MapTileNetwork();

private:
    static QHash<QThread *, MapTileNetwork *> _instances;
    static QMutex _mutex;
    QNetworkAccessManager * _manager;

    QByteArray _userAgent;
};

#endif // MAPTILENETWORK_H
