#pragma once

#include <set>

#include <QString>
#include <QVariant>
#include <QMap>

#include "../data/point.h"

class IDataSource
{
public:
    virtual ~IDataSource() {}
    virtual std::set<Point>&& getNextPoints() = 0;
    virtual bool applyConfig(const QMap<QString, QVariant>& config) = 0;
    virtual bool isStream() const = 0;

};
