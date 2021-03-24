#include <QCoreApplication>
#include <QCommandLineParser>
#include "Server.hpp"

void handleArgs(QCommandLineParser &parser, quint16 &port, int &limit, int &lowBound, int &highBound)
{
    port = parser.isSet("port") ? parser.value("port").toUInt() : (4242);
    if (port != parser.value("port").toInt()) {
        qCritical("Error: args: invalid port value specified.");
        parser.showHelp(84);
    }
    limit = parser.isSet("limit") ? parser.value("limit").toInt() : (-1);
    if (limit < -1) {
        qCritical("Error: args: invalid limit value specified.");
        parser.showHelp(84);
    }
    lowBound = 1, highBound = 100;
    if (parser.isSet("bounds")) {
        QStringList tmp = parser.value("bounds").split(',');
        if (tmp.size() != 2) {
            qCritical("Error: args: invalid bounds syntax.");
            parser.showHelp(84);
        }
        lowBound = tmp.at(0).toInt();
        highBound = tmp.at(1).toInt();
        if (lowBound > highBound) {
            qCritical("Error: args: invalid bounds. (x > y)");
            parser.showHelp(84);
        }
    }
}

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    QCoreApplication::setApplicationName("MoreOrLess-server");

    // Parser creation with help flag support
    QCommandLineParser parser;
    parser.setApplicationDescription("A Qt-based server for the More Or Less game.");
    parser.addHelpOption();

    // Optional args (cf. QCommandLineOption)
    parser.addOption({{"p", "port"}, "The port on which to wait for clients.", "value", "4242"});
    parser.addOption({{"l", "limit"}, "Limit of attempts per game (-1 for unlimited).", "value", "-1"});
    parser.addOption({{"b", "bounds"}, "Limits of the interval (signed integers, x & y included).", "x,y", "1,100"});

    parser.process(a);
    quint16 port;
    int limit, lowBound, highBound;

    handleArgs(parser, port, limit, lowBound, highBound);
    Server srv(port, limit, lowBound, highBound, &a);

    return a.exec();
}
