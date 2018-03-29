#include <fstream>
#include <stdexcept>

#include <utils.hpp>

std::vector<double> Utils::readPoints(std::string fileName)
{
    std::ifstream input;
    input.open(fileName);
    if (!input.is_open())
    {
        throw std::runtime_error("Can't open file " + fileName);
    }

    double value = 0.0;
    std::vector<double> result;
    /// TODO: Check for reading error.
    while (input >> value)
    {
        result.push_back(value);
    }

    return result;
}
