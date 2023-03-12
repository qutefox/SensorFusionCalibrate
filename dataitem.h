#pragma once

#include <QDebug>

class DataItem
{
public:

    double d0;
    double d1;
    double d2;

    DataItem();
    DataItem(double d0, double d1, double d2);

    QString toString() const;
};

