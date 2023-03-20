#pragma once

#include <string>
#include <iostream>

class SerialPortIdMatch
{
public:
    std::string m_id;
    std::string m_vendorName;
    std::string m_productName;

    SerialPortIdMatch(const std::string& id);
    SerialPortIdMatch(const std::string& id, const std::string& vendorName, const std::string& productName);

    bool operator<(const SerialPortIdMatch& rhs) const;
    bool operator==(const SerialPortIdMatch& rhs) const;
    bool operator!=(const SerialPortIdMatch& rhs) const;

    friend std::ostream& operator<<(std::ostream& os, const SerialPortIdMatch& spim);
};

std::ostream& operator<<(std::ostream& os, const SerialPortIdMatch& spim);
