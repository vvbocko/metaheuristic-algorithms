#include "CalculationTools.h"

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

    return std::lround(std::sqrt(dx * dx + dy * dy)); // round zwraca zmiennoprzecinkową, a lround bezpośrednio rzutuje na całkowitą
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