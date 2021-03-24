#pragma once

#include <QObject>

namespace ViewType
{
    Q_NAMESPACE
    enum type {
        CONNECTION,
        PROFILE,
        GAME,
        END_WIN,
        END_LOSE
    };
    Q_ENUM_NS(type)
}