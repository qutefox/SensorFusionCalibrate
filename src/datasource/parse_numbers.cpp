#include "parse_numbers.h"

#include <regex>

std::regex numRegex(R"((.*?((-?)([\d]+)(\.[\d]+)?)))");

std::vector<double> parseNumbersFromLine(const std::string& line)
{
    std::vector<double> result;
    if (line.empty()) return result;

    result.reserve(6);
    std::smatch res;

    std::string::const_iterator searchStart(line.cbegin());
    while(std::regex_search(searchStart, line.cend(), res, numRegex))
    {
        try
        {
            result.push_back(std::stod(res[2]));
        }
        catch(...)
        {
            // ignore
        }

        searchStart = res.suffix().first;
    }

    return result;
}

bool parseLineToDeviceData(const std::string& line, std::vector<std::set<Point>>& devicePoints)
{
    std::vector<double> doubles = parseNumbersFromLine(line);
    if (doubles.empty()) return false;

    std::size_t deviceCount = doubles.size() / 3;

    while(deviceCount > devicePoints.size())
    {
        // Need to add more point set instance.
        devicePoints.push_back(std::set<Point>());
    }

    for (std::size_t i = 0 ; i < deviceCount ; ++i)
    {
        Point point(
            doubles[(i*3)+0],
            doubles[(i*3)+1],
            doubles[(i*3)+2]
        );
        devicePoints[i].insert(point);
    }

    return true;
}
