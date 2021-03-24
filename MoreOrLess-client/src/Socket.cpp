#include "Socket.hpp"

Socket::Socket()
: _url()
{
    connect(this, &QWebSocket::connected, this, &Socket::connectAcceptedHandle);
    connect(this, &QWebSocket::binaryMessageReceived, this, &Socket::binaryMessageHandle);
    connect(this, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(connectRefusedHandle(QAbstractSocket::SocketError)));
}

Socket::~Socket()
{
    if (state() == QAbstractSocket::ConnectedState)
        close();
    else if (state() != QAbstractSocket::UnconnectedState)
        abort();
}

void Socket::connectToServer(const QString &host, const QString &port)
{
    if (host == "localhost" || host == "127.0.0.1")
        _url = QUrl(QStringLiteral("ws://localhost:") + port);
    else
        _url = QUrl(QStringLiteral("ws://") + host + QStringLiteral(":") + port);
    _url.setPort(port.toUShort());
    open(_url);
}

void Socket::binaryMessageHandle(QByteArray msg)
{
    const QJsonObject data = QJsonDocument::fromJson(msg).object();
    if (data.contains("error"))
        emit errorReceived(data["error"].toObject());
    else
        emit dataReceived(data);
    emit updated();
}

void Socket::connectAcceptedHandle()
{
    emit connectAccepted();
    emit updated();
}

void Socket::connectRefusedHandle(QAbstractSocket::SocketError err)
{
    QJsonObject error;
    error["critical"] = false;
    error["title"] = "Warning: Socket.cpp: couldn't connect to server.";
    error["message"] = errorString();
    error["reset"] = true;
    emit errorReceived(error);
    emit updated();
}

void Socket::sendData(QJsonObject data)
{
    sendBinaryMessage(QJsonDocument(data).toJson());
}
