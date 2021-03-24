#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include "Core.hpp"
#include "views.hpp"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);
    QQmlApplicationEngine engine;
    Core core;
    engine.rootContext()->setContextProperty("core", &core);
    
    //enums registering for QML access
    qmlRegisterUncreatableMetaObject(ViewType::staticMetaObject, "view.type", 1, 0, "ViewType", "Error: only enums");

    // App initialization
    QGuiApplication::setApplicationName("MoreOrLess-Client");
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
    if (engine.rootObjects().isEmpty())
        return (84);
    core.setup(qobject_cast<QQuickWindow *>(engine.rootObjects().first()));
    return app.exec();
}