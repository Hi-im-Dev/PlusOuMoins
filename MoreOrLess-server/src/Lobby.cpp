#include "Lobby.hpp"

Lobby::Lobby(int limit, QPair<int, int> bounds, QWebSocket *socket, QObject *parent)
: QObject(parent), _limit(limit), _counter(0), _bounds(bounds), _socket(socket), _gameData(nullptr)
{
    qDebug() << "Creating new lobby.";
    connect(_socket, &QWebSocket::binaryMessageReceived, this, &Lobby::handleUserData);
    createRandomToFind();
}

Lobby::~Lobby()
{
    _socket->close();
    delete _socket;
    qDebug() << "Socket destructed.";
    delete _gameData;
    qDebug() << "Json gamedata destructed.";
}

void Lobby::createRandomToFind()
{
    _toFind = QRandomGenerator::global()->bounded(_bounds.first, _bounds.second);
    qInfo("New value to find: %d", _toFind);
}

void Lobby::handleGame(QByteArray msg)
{
    const QJsonObject data = QJsonDocument::fromJson(msg).object();
    QJsonObject res;
    
    if (data.contains("guess")) {
        int guess = data["guess"].toInt();
        _counter++;
        
        if (guess < _bounds.first || guess > _bounds.second) {
            QJsonObject err;
            err["title"] = "Error: invalid guess.";
            err["message"] = "Guessed value must be between " + QString(_bounds.first) + " and " + QString(_bounds.second) + ".";
            err["critical"] = false;
            res["error"] = err;
        } else if (guess == _toFind) {
            res["status"] = "WIN";
        } else if (_counter == _limit) {
            res["status"] = "LOSE";
        } else if (guess > _toFind) {
            res["status"] = "-";
        } else if (guess < _toFind) {
            res["status"] = "+";
        }
    } else {
        QJsonObject err;
        err["title"] = "Error: couldn't parse guess.";
        err["message"] = "Please enter a valid value.";
        err["critical"] = false;
        res["error"] = err;
    }
    if (res["status"].toString() == "WIN" || res["status"].toString() == "LOSE") {
        if (_gameData) {
            QDateTime endDate = QDateTime::currentDateTime();
            (*_gameData)["startDate"] = _startDate.toString();
            (*_gameData)["duration"] = _startDate.secsTo(endDate);
            (*_gameData)["tries"] = _counter;
            (*_gameData)["status"] = res["status"].toString();
            
            QJsonObject hist;
            hist["actual"] = (*_gameData);
            res["history"] = hist;
        }
    }
    _socket->sendBinaryMessage(QJsonDocument(res).toJson());
}

void Lobby::handleUserData(QByteArray msg)
{
    const QJsonObject data = QJsonDocument::fromJson(msg).object();
    if (data.contains("username") && data.contains("bot")) {
        if (data["bot"].toBool() == true || data["username"].toString() == "") {
            _gameData = nullptr;
        } else {
            _gameData = new QJsonObject();
            (*_gameData)["username"] = data["username"].toString();
        }
    } else {
        QJsonObject res;
        QJsonObject err;
        err["title"] = "Error: couldn't parse users settings.";
        err["message"] = "Please try to reboot the client.";
        err["critical"] = true;
        res["error"] = err;
        _socket->sendBinaryMessage(QJsonDocument(res).toJson());
        return;
    }
    QJsonObject res;
    res["status"] = "OK";
    _socket->sendBinaryMessage(QJsonDocument(res).toJson());
    disconnect(_socket, &QWebSocket::binaryMessageReceived, this, &Lobby::handleUserData);
    connect(_socket, &QWebSocket::binaryMessageReceived, this, &Lobby::handleGame);
    _startDate = QDateTime::currentDateTime();
}