#pragma once

#include <set>
#include <vector>

#include <QObject>
#include <QWidget>

#include <data/point.h>

class IDataSource : public QObject
{
    Q_OBJECT

public:
    IDataSource(QObject* parent = nullptr) : QObject(parent) {};
    virtual ~IDataSource() {}
    virtual bool getNextPoints(std::vector<std::set<Point>>& devicePoints) = 0;
    virtual bool isStream() const = 0;
    virtual QWidget* widget() const = 0;

signals:
    void opened();
    void errorOccurred(QString);
    void closed();
    void dataAvailable();

};
