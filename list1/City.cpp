#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cmath>
#include <algorithm>
#include <random>
#include <ctime>

std::random_device randomDev;
std::mt19937 gen(randomDev());

struct City //jkao wierzcholek
{
    int id;
    float x;
    float y;
};

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


int calculateDistance(const City& a, const City& b)
{
    float dx = a.x - b.x;
    float dy = a.y - b.y;

    return std::round(std::sqrt(dx * dx + dy * dy));
}

void randomPermutation(std::vector<City>& cities)
{
    std::shuffle(cities.begin(), cities.end(), gen);
}

int calculateTotalDistance(std::vector<City>& cities)
{
    int dimension = cities.size();
    int totalDistance = 0;

    for (int i=0; i<dimension-1; i++) 
    {
        totalDistance += calculateDistance(cities[i], cities[i+1]);
    }
    totalDistance += calculateDistance(cities[dimension-1], cities[0]);

    return totalDistance;
}

int main()
{
    std::string filename = "dj38.tsp"; 
    std::vector<City> cities = loadCitiesFile(filename);
    int dimension = cities.size();
    int sum = 0;
    float average = 0.0f;

    if (!cities.empty())
    {
        std::cout << "Wczytane niasta: " << cities.size() << std::endl;
        
        int distance = calculateDistance(cities[0], cities[1]);
        
        std::cout << "wspolrzedne 0: " << cities[0].x << ", " << cities[0].y << "\n";
        std::cout << "wspolrzedne 1: " << cities[1].x << ", " << cities[1].y << "\n";

        std::cout << "Odleglosc miedzy mistem 0 - 1: " << distance << "\n";

        
        for (int i=0; i<1000; i++)
        {
            randomPermutation(cities);
            sum += calculateTotalDistance(cities);
        }
        average = sum/1000.0f;
        std::cout << "Srednia: " << average << "\n";
    }

    return 0;
}