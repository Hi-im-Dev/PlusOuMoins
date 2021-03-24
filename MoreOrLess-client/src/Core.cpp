#include "Core.hpp"

Core::Core()
:_window(nullptr)
{
}

void Core::setup(QQuickWindow * const window)
{
    _window = window;
    connect(&_socket, &Socket::updated, this, &Core::triggerUpdate);
    connect(&_socket, &Socket::connectAccepted, this, &Core::pushToProfile);
    connect(&_socket, &Socket::dataReceived, this, &Core::dataReceivedHandle);
    connect(&_socket, &Socket::errorReceived, this, &Core::errorReceivedHandle);
    QQuickItem *view = _window->findChild<QQuickItem *>("connectView");
    if (view) {
        connect(view, SIGNAL(sendData(QVariant)), this, SLOT(selectDataParser(QVariant)));
    } else {
        qCritical("Error: Core.cpp: Couldn't load view (connectView @ Connect.qml)");
        emit displayError("The program is going to abort.", "An app page could not load. Please try again or try reinstalling the app.", true);
    }
}

Core::~Core()
{
    if (_window)
        delete _window;
    qDebug() << "Deleted window instance.";
}

void Core::pushToProfile()
{
    emit pushView(ViewType::PROFILE);
    QQuickItem *view = _window->findChild<QQuickItem *>("profileView");
    if (view) {
        connect(view, SIGNAL(sendData(QVariant)), this, SLOT(selectDataParser(QVariant)));
    } else {
        qCritical("Error: Core.cpp: Couldn't load view (profileView @ Profile.qml)");
        emit displayError("The program is going to abort.", "An app page could not load. Please try again or try reinstalling the app.", true);
    }
}

void Core::selectDataParser(QVariant data)
{
    int viewType = data.toMap().value("view", -1).toInt();
    switch (viewType) {
    case ViewType::type::CONNECTION:
        processConnectionData(data.toMap());
        break;
    case ViewType::type::PROFILE:
        processData(data.toMap());
        break;
    case ViewType::type::GAME:
        processData(data.toMap());
        break;
    default:
        qCritical("Error: Core.cpp: Couldn't select correct data parser.");
        emit displayError("The program is going to abort.", "An app page could not be selected. Please try again or try reinstalling the app.", true);
    }
}

void Core::processConnectionData(QVariantMap data)
{
    QString host = data.value("host").toString();
    QString port = data.value("port").toString();

    if (host.isEmpty() && port.isEmpty()) {
        emit displayError("Warning: missing fields", "The host & port fields are empty !", false);
    } else if (host.isEmpty()) {
        emit displayError("Warning: missing fields", "The host field is empty !", false);
    } else if (port.isEmpty()) {
        emit displayError("Warning: missing fields", "The port field is empty !", false);
    } else {
        _socket.connectToServer(host, port);
    }
}

void Core::processData(QVariantMap data)
{
    QJsonObject jsonData = QJsonObject::fromVariantMap(data);
    _socket.sendData(jsonData);
}

void Core::errorReceivedHandle(QJsonObject error)
{
    emit displayError(error["title"].toString(), error["message"].toString(), error["critical"].toBool());
    if (error.contains("reset"))
        emit reset();
}

void Core::dataReceivedHandle(QJsonObject data)
{
    if (data["status"].toString() == "OK") {
        emit pushView(ViewType::GAME);
        QQuickItem *view = _window->findChild<QQuickItem *>("gameView");
        if (view) {
            connect(view, SIGNAL(sendData(QVariant)), this, SLOT(selectDataParser(QVariant)));
        } else {
            qCritical("Error: Core.cpp: Couldn't load view (gameView @ Game.qml)");
            emit displayError("The program is going to abort.", "An app page could not load. Please try again or try reinstalling the app.", true);
        }
    } else if (data["status"].toString() == "+") {
        emit valueChanged("+");
    } else if (data["status"].toString() == "-") {
        emit valueChanged("-");
    } else if (data["status"].toString() == "WIN" || data["status"].toString() == "LOSE") {
        processEndData(data["status"].toString(), data["history"].toObject());
    }
}

void Core::processEndData(QString status, QJsonObject history)
{
    if (status == "WIN")
        emit pushView(ViewType::END_WIN);
    else
        emit pushView(ViewType::END_LOSE);

    emit sendEndData(QJsonDocument(history["actual"].toObject()).toJson(QJsonDocument::Compact));
}

void Core::triggerUpdate()
{
    emit updated();
}