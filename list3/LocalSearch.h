#pragma once

#include "../list0/City.h"
#include <vector>
#include <functional>

struct Result
{
    std::vector<City> bestRoute; 
    int improvementSteps;
    int totalDistance;
};

using LocalSearch = std::function<Result(std::vector<City>&, const std::vector<std::vector<int>>&)>;

Result LocalSearch_Invert(std::vector<City>& cities, const std::vector<std::vector<int>>& distMatrix);
Result LocalSearch_RandomInvert(std::vector<City>& cities, const std::vector<std::vector<int>>& distMatrix);
Result LocalSearch_Swap(std::vector<City>& cities, const std::vector<std::vector<int>>& distMatrix);

Result Simulated_Annealing(std::vector<City>& cities, const std::vector<std::vector<int>>& distMatrix);
Result TabuSearch(std::vector<City>& cities, const std::vector<std::vector<int>>& distMatrix, int tabu_lenght);



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