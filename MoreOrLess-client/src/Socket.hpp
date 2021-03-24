#pragma once

#include <QWebSocket>
#include <QJsonObject>
#include <QJsonDocument>
#include <QObject>
#include <QUrl>
#include <QVector>
#include <QDebug>
#include "views.hpp"

class Socket : public QWebSocket
{
    Q_OBJECT
public:
    Socket();
    ~Socket();

    void connectToServer(const QString &host, const QString &port);
    void sendData(QJsonObject data);

signals:
    void connectAccepted();
    void dataReceived(QJsonObject data);
    void errorReceived(QJsonObject error);
    void updated();

public slots:
    void binaryMessageHandle(QByteArray msg);
    void connectAcceptedHandle();
    void connectRefusedHandle(QAbstractSocket::SocketError err);

private:
    QUrl _url;
};

