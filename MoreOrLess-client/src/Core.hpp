#pragma once

#include <QObject>
#include <QQuickWindow>
#include <QQuickItem>
#include <QJsonArray>
#include "views.hpp"
#include "Socket.hpp"

class Core : public QObject
{
    Q_OBJECT
public:
    explicit Core();
    ~Core();

    void setup(QQuickWindow * const window);

public slots:
    void pushToProfile();
    void selectDataParser(QVariant data);
    void dataReceivedHandle(QJsonObject data);
    void errorReceivedHandle(QJsonObject data);
    void triggerUpdate();

signals:
    void pushView(int viewType);
    void displayError(QString title, QString message, bool critical);
    void updated();
    void reset();
    void valueChanged(QString value);
    void sendEndData(QString data);

private:
    void processConnectionData(QVariantMap data);
    void processData(QVariantMap data);
    void processEndData(QString status, QJsonObject history);
private:
    QQuickWindow *_window;
    Socket _socket;
};