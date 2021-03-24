#include "Server.hpp"

Server::Server(quint16 port, int limit, int lowBound, int highBound, QObject *parent)
: QWebSocketServer("MoreOrLessSrv", NonSecureMode, parent), _port(port), _limit(limit), _bounds(lowBound, highBound)
{
    qInfo("Server instance created with options: ");
    qInfo("\t- limit: %d", _limit);
    qInfo("\t- bounds: %d ~ %d", _bounds.first, _bounds.second);

    if (listen(QHostAddress::LocalHost, _port)) {
        qInfo("Listening on port %d...", _port);
        connect(this, SIGNAL(newConnection()), SLOT(connectHandle()));
    }
}

void Server::connectHandle()
{
    QWebSocket *socket = nextPendingConnection();
    Lobby *lby = new Lobby(_limit, _bounds, socket, this);

    connect(socket, &QWebSocket::disconnected, this, &Server::killLobby);

    _lobbyArr[socket] = lby;
}

void Server::killLobby()
{
    QWebSocket *toDelete = qobject_cast<QWebSocket *>(sender());
    QMap<QWebSocket *, Lobby *>::iterator i = _lobbyArr.begin();
    QMap<QWebSocket *, Lobby *>::iterator iEnd = _lobbyArr.end();
    for (int idx = 0; i != iEnd; ++i, ++idx) {
        if (i.key() == toDelete) {
            qDebug() << "deleting lobby";
            delete *i;
            _lobbyArr.erase(i);
            break;
        }
    }
    if (i == iEnd) {
        qWarning("An error happened while trying to delete a lobby. This could cause memory leaks, so be careful.");
    }
}