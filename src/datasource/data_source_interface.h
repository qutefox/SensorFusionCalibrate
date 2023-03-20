#pragma once

#include <set>
#include <vector>

#include <QObject>
#include <QWidget>
#include <QString>

#include <data/point.h>

class IDataSource : public QObject
{
    Q_OBJECT

public:
    IDataSource(QObject* parent = nullptr) : QObject(parent) {};
    virtual ~IDataSource() {}
    virtual bool getNextPoints(std::vector<std::set<Point>>& devicePoints) = 0;
    virtual bool isStream() const = 0;
    virtual bool canSignalDataAvailable() const { return false; }
    virtual QString getTypeName() const { return ""; }
    virtual QWidget* widget() const = 0;
    virtual int numberOfTabs() const { return 0; }
    virtual QWidget* getTab(int tabId) const { return nullptr; }

signals:
    void dataSourceStarted();
    void errorOccurred(QString);
    void dataSourceEnded();
    void dataAvailable();

};
