#include "FileLoadSave.h"

#include <iostream>
#include <fstream>

std::vector<City> loadCitiesFile(const std::string& filename)
{
    std::vector<City> cities;
    std::ifstream file(filename);
    std::string word;

    if (!file.is_open())
    {
        std::cerr << "error: " << filename << "\n";
        return cities;
    }

    while (file >> word)
    {
        if (word == "NODE_COORD_SECTION")
        {
            break;
        }
    }

    City city;
    while (file >> city.id >> city.x >> city.y)
    {
        cities.push_back(city);
    }

    file.close();
    return cities;
}

void saveRouteToFile(const std::vector<City>& route, const std::string& filename)
{
    std::ofstream outFile(filename);

    if (!outFile.is_open())
    {
        std::cerr << "Error: " << filename << "\n";
        return;
    }

    for (size_t i = 0; i < route.size(); i++)
    {
        outFile << route[i].x << " " << route[i].y << "\n";
    }

    if (!route.empty())
    {
        outFile << route[0].x << " " << route[0].y << "\n";
    }

    outFile.close();
}