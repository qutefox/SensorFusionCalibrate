#pragma once

#include <set>

#include <QWidget>
#include <QSet>

#include "../data/point.h"

class IDataSource : public QWidget
{
    Q_OBJECT

public:
    IDataSource(QWidget* parent = nullptr) : QWidget(parent) {};
    virtual ~IDataSource() {}
    virtual bool getNextPoints(QSet<Point>& dev0, QSet<Point>& dev1, QSet<Point>& dev2) = 0;
    virtual bool isStream() const = 0;


signals:
    void opened();
    void failedToOpen(QString errorMessage);
    void closed();
    void readNewPoints();

};
