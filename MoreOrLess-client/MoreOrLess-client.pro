QT += qml quick gui websockets

CONFIG += c++11
CONFIG -= app_bundle
CONFIG(release, debug|release):DEFINES += QT_NO_DEBUG_OUTPUT

SOURCES += \
    src/Core.cpp \
    src/Socket.cpp \
    src/main.cpp

HEADERS += \
    src/Core.hpp \
    src/Socket.hpp \
    src/views.hpp

RESOURCES += \
    qml/resources.qrc

DISTFILES += \
    qml/Connect.qml \
    qml/End.qml \
    qml/Game.qml \
    qml/Profile.qml \
    qml/fonts/Roboto-Black.ttf \
    qml/main.qml
