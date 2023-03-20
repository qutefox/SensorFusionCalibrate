#include "id_matcher.h"

#include <map>
#include <string>
#include <sstream>
#include <iomanip>

#include <QFile>
#include <QString>

std::map<std::string, SerialPortIdMatch> cache;

std::string generateSearchKey(int vendorId, int productId)
{
    std::ostringstream oss;

    if (vendorId > 0) oss << std::hex << std::setw(4) << std::setfill('0') << vendorId << "_";
    else oss << "unknown_";

    if (productId > 0) oss << std::hex << std::setw(4) << std::setfill('0') << productId;
    else oss << "unknown";

    return oss.str();
}

SerialPortIdMatch findInUsbIdsResourceFile(const std::string& searchKey, int vendorId, int productId)
{
    SerialPortIdMatch match(searchKey);

    QFile file(":/usb.ids");
    if(!file.open(QIODevice::ReadOnly))
    {
        return match;
    }

    QString vendorIdStr = QStringLiteral("%1").arg(vendorId, 4, 16, QLatin1Char('0'));
    QString productIdStr = QStringLiteral("%1").arg(productId, 4, 16, QLatin1Char('0'));

    QString line;
    bool vendorIdFound = false;
    while (!file.atEnd())
    {
        line = file.readLine();

        if (line.isEmpty() || line.startsWith("#")) continue;
        if (vendorIdFound)
        {
            if (line[0] != '\t') break;
            line.remove(0, 1);
            if(line.startsWith(productIdStr))
            {
                line.remove(0, 4);
                match.m_productName = line.trimmed().toStdString();
                break;
            }
        }
        else if (line.startsWith(vendorIdStr))
        {
            line.remove(0, 4);
            match.m_vendorName = line.trimmed().toStdString();
            vendorIdFound = true;
        }
    }

    file.close();

    return match;
}

SerialPortIdMatch findMatch(int vendorId, int productId)
{
    std::string searchKey = generateSearchKey(vendorId, productId);
    auto it = cache.find(searchKey);
    if (it != cache.end()) return it->second;
    SerialPortIdMatch match = findInUsbIdsResourceFile(searchKey, vendorId, productId);
    if (!match.m_vendorName.empty() || !match.m_productName.empty())
    {
        cache.emplace(searchKey, match);
    }
    return match;
}
