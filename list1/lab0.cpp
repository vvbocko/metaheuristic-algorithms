#include "lab0.h"

#include <iostream>
#include <cmath>
#include <algorithm>
#include <limits>
#include <random>
#include <ctime>

std::mt19937 gen(time(nullptr));
int hugeNumber = std::numeric_limits<int>::max();


void randomPermutation(std::vector<City>& cities)
{
    std::shuffle(cities.begin(), cities.end(), gen);
}

int calculateDistance(const City& a, const City& b)
{
    float dx = a.x - b.x;
    float dy = a.y - b.y;

    return std::round(std::sqrt(dx * dx + dy * dy));
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
    std::vector<std::string> filenames = {
        "wi29.tsp", "dj38.tsp", "qa194.tsp", "uy734.tsp", "zi929.tsp"
    };

    for (size_t f = 0; f < filenames.size(); f++)
    {
        std::string currentFile = filenames[f];
        std::vector<City> cities = loadCitiesFile(currentFile);
        std::vector<City> bestRoute;

        std::cout << "\n" << currentFile << "\n";

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
                if (distance1 < min)
                {
                    min = distance1;
                    bestRoute = cities;
                }

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
        std::string outputFileName = "bestRoute_" + currentFile.substr(0, currentFile.find('.')) + ".txt";
        saveRouteToFile(bestRoute, outputFileName);
    }
    return 0;
}