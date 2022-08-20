#include "maptilenetwork.h"

#include <QMutexLocker>
#include <QNetworkRequest>
#include <QThread>
#include <QtDebug>

const QByteArray DEFAULT_USER_AGENT = "MapGraphics";

QHash<QThread *, MapTileNetwork *> MapTileNetwork::_instances = QHash<QThread *, MapTileNetwork*>();
QMutex MapTileNetwork::_mutex;

MapTileNetwork *MapTileNetwork::getInstance()
{
    QMutexLocker lock(&_mutex);
    QThread * current = QThread::currentThread();
    if (!MapTileNetwork::_instances.contains(current))
        MapTileNetwork::_instances.insert(current, new MapTileNetwork());
    return MapTileNetwork::_instances.value(current);
}

MapTileNetwork::~MapTileNetwork()
{
    delete _manager;
    _manager = 0;
}

QNetworkReply *MapTileNetwork::get(QNetworkRequest &request)
{
    request.setRawHeader("User-Agent",
                         _userAgent);
    QNetworkReply * toRet = _manager->get(request);
    return toRet;
}

void MapTileNetwork::setUserAgent(const QByteArray &agent)
{
    _userAgent = agent;
}

QByteArray MapTileNetwork::userAgent() const
{
    return _userAgent;
}

MapTileNetwork::MapTileNetwork()
{
    _manager = new QNetworkAccessManager();
    this->setUserAgent(DEFAULT_USER_AGENT);
}
