#include "dataitem.h"

DataItem::DataItem()
    : d0{0.0}, d1{0.0}, d2{0.0}
{

}

DataItem::DataItem(double d0, double d1, double d2)
    : d0{d0}, d1{d1}, d2{d2}
{

}

QString DataItem::toString() const
{
    return QString("d0: %1, d1: %2, d2: %3").arg(d0).arg(d1).arg(d2);
}

