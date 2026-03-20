#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cmath>
#include <algorithm>
#include <random>
#include <ctime>
#include <limits>

std::mt19937 gen(time(nullptr));
int hugeNumber = std::numeric_limits<int>::max();

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

    int min_10 = hugeNumber;
    int min_50 = hugeNumber;
    int min = hugeNumber;
 
    int sum_10 = 0;
    int sum_50 = 0;
    int minimum = 0;

    float average_10 = 0.0f;
    float average_50 = 0.0f;

    if (!cities.empty())
    {
        for (int i=1; i<=1000; i++)
        {
            randomPermutation(cities);
            int distance1 = calculateTotalDistance(cities);

            min_10 = std::min(min_10, distance1);
            min_50 = std::min(min_50, distance1);
            min = std::min(min, distance1);

            if (i % 10 == 0)
            {
                sum_10 += min_10;
                min_10 = hugeNumber;
            }
            if (i % 50 == 0)
            {
                sum_50 += min_50;
                min_50 = hugeNumber;
            }
        }
        average_10 = sum_10/100.0f;
        average_50 = sum_50/20.0f;
        minimum = min;

        std::cout << "Srednia: " << average_10 << "\n";
        std::cout << "Srednia: " << average_50 << "\n";
        std::cout << "Minimum: " << minimum << "\n";
    }
    return 0;
}