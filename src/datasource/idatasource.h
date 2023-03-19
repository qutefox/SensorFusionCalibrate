#pragma once

#include <set>
#include <vector>

#include <QWidget>

#include "../data/point.h"

class IDataSource : public QWidget
{
    Q_OBJECT

public:
    IDataSource(QWidget* parent = nullptr) : QWidget(parent) {};
    virtual ~IDataSource() {}
    virtual bool getNextPoints(std::vector<std::set<Point>>& devicePoints) = 0;
    virtual bool isStream() const = 0;


signals:
    void opened();
    void failed(QString errorMessage);
    void closed();
    void dataAvailable();

};
