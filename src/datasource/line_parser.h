#pragma once

#include <vector>
#include <string>
#include <set>

#include "../data/point.h"

bool parseLineToDeviceData(const std::string& line, std::vector<std::set<Point>>& devicePoints);
