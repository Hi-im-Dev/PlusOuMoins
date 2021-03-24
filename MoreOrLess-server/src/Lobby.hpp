#pragma once

#include <QObject>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QPair>
#include <QWebSocket>
#include <QRandomGenerator>
#include <QtMath>

class Lobby : public QObject
{
    Q_OBJECT
public:
    explicit Lobby(int limit, QPair<int, int> bounds, QWebSocket *socket, QObject *parent = nullptr);
    ~Lobby();

public slots:
    void handleUserData(QByteArray msg);
    void handleGame(QByteArray msg);

private:
    void createRandomToFind();

private:
    int _limit;
    int _counter;
    int _toFind;
    QPair<int, int> _bounds;
    QWebSocket *_socket;
    QDateTime _startDate;
    QJsonObject *_gameData;
};

