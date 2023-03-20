#include "serialport_id_match.h"

SerialPortIdMatch::SerialPortIdMatch(const std::string& id)
    : m_id{ id }
    , m_vendorName{ "" }
    , m_productName{ "" }
{

}

SerialPortIdMatch::SerialPortIdMatch(const std::string& id, const std::string& vendorName, const std::string& productName)
    : m_id{ id }
    , m_vendorName{ vendorName }
    , m_productName{ productName }
{

}

bool SerialPortIdMatch::operator<(const SerialPortIdMatch& rhs) const
{
    return m_id < rhs.m_id;
}

bool SerialPortIdMatch::operator==(const SerialPortIdMatch& rhs) const
{
    return m_id == rhs.m_id;
}

bool SerialPortIdMatch::operator!=(const SerialPortIdMatch& rhs) const
{
    return m_id != rhs.m_id;
}

std::ostream& operator<<(std::ostream& os, const SerialPortIdMatch& spim)
{
    os << "id: " << spim.m_id << ", vendor name: " << spim.m_vendorName << ", product name: " << spim.m_productName;
    return os;
}
