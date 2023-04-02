#pragma once

#include <memory>
#include <vector>
#include <string>

#include <QColor>

namespace Ansi
{

std::shared_ptr<QColor> xParseColor(const std::string& in);
std::shared_ptr<QColor> xParseColor(const std::vector<char>& in);

} // namespace Ansi
