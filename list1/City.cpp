#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cmath>

struct City //jkao wierzcholek
{
    int id;
    float x;
    float y;
};

std::vector<City> loadCitiesFile(const std::string& filename)
{
    std::vector<City> citiesList;
    std::ifstream file(filename);
    std::string word;

    if (!file.is_open())
    {
        std::cerr << "error: " << filename << "\n";
        return citiesList;
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
        citiesList.push_back(city);
    }

    file.close();
    return citiesList;
}


int calculateDistance(const City& a, const City& b)
{
    float dx = a.x - b.x;
    float dy = a.y - b.y;

    return std::round(std::sqrt(dx * dx + dy * dy));
}


int main()
{
    std::string filename = "dj38.tsp"; 
    std::vector<City> cities = loadCitiesFile(filename);

    if (!cities.empty())
    {
        std::cout << "Wczytane niasta: " << cities.size() << std::endl;
        
        int distance = calculateDistance(cities[0], cities[1]);
        
        std::cout << "wspolrzedne 0: " << cities[0].x << ", " << cities[0].y << "\n";
        std::cout << "wspolrzedne 1: " << cities[1].x << ", " << cities[1].y << "\n";

        std::cout << "Odleglosc miedzy mistem 0 - 1: " << distance << "\n";
    }

    return 0;
}