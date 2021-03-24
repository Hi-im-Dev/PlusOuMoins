#pragma once

#include <QWebSocketServer>
#include <QWebSocket>
#include <QObject>
#include <QMap>
#include <QPair>
#include <QDebug>

#include "Lobby.hpp"

class Server : public QWebSocketServer
{
    Q_OBJECT
public:
    Server(quint16 port = 4242, int limit = -1, int lowBound = 1, int highBound = 100, QObject *parent = Q_NULLPTR);

public slots:
    void killLobby();
    void connectHandle();
private:
    quint16 _port;
    int _limit;
    QPair<int, int> _bounds;
    QMap<QWebSocket *, Lobby *> _lobbyArr;
};

