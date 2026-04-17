#pragma once

#include "City.h"
#include <vector>

struct Result
{
    std::vector<City> bestRoute; 
    int improvementSteps;
    int totalDistance;
};

Result LocalSearch(std::vector<City>& cities, const std::vector<std::vector<int>>& distMatrix);




// void invert(std::vector<City>& cities, int start, int end) // A, B, C, D, E, F, G      // C 2   E  = 5/2 = 2 + 1 = 3
// {
//     int mid = start + (end - start)/ 2;
//     int swaps = mid - start;
//     for(int i = 0; i < mid; i++)
//     {
//         City temp = cities[start + i];
//         cities[start + i] = cities[end - i];
//         cities[end - i] = temp;
//     }
// }