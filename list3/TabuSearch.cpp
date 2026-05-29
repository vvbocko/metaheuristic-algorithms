#include "LocalSearch.h"
#include "../list0/FileLoadSave.h"
#include "../list0/lab0.h"

#include <iostream>
#include <algorithm>
#include <random>
#include <limits>

Result TabuSearch(std::vector<City>& cities, const std::vector<std::vector<int>>& distMatrix, int tabu_length)
{
    int n = cities.size();
    int improvementSteps = 0;
    randomPermutation(cities);

    std::vector<int> tabuList(n * n, 0);
    int cooldown = tabu_length;

    long long currentDistance = 0;
    for (int i = 0; i < n - 1; i++) 
    {
        currentDistance += distMatrix[cities[i].id][cities[i+1].id];
    }
    currentDistance += distMatrix[cities[n-1].id][cities[0].id];

    long long globalBestDistance = currentDistance;
    std::vector<City> globalBestRoute = cities;

    int maxIterations = (n < 1000) ? std::max(800, 2*n) : 2 * n; 
    int maxStepsWithoutImprovement = (n < 1000) ? std::max(100, n/3) : 250;
    int idleSteps = 0;

    std::mt19937 rng(12345); 

    for (int iter = 0; iter < maxIterations; iter++)
    {
        long long bestDelta = std::numeric_limits<long long>::max();
        int best_i = 0; 
        int best_j = 0;
        bool foundMove = false; 

        //MAŁĘ DANE
        if (n < 1000) 
        {
            for (int i = 0; i < n - 1; i++) 
            {
                for (int j = i + 1; j < n; j++) 
                {    
                    if (i == 0 && j == n - 1) continue;  
                    
                    int prev_i = (i == 0) ? n - 1 : i - 1; 
                    int next_j = (j == n - 1) ? 0 : j + 1;

                    long long currentEdges = distMatrix[cities[prev_i].id][cities[i].id] + distMatrix[cities[j].id][cities[next_j].id];
                    long long newEdges     = distMatrix[cities[prev_i].id][cities[j].id] + distMatrix[cities[i].id][cities[next_j].id];
                    long long delta_candidate = newEdges - currentEdges;

                    int id_i = cities[i].id;
                    int id_j = cities[j].id;
                    bool isTabu = (tabuList[id_i * n + id_j] > iter) || (tabuList[id_j * n + id_i] > iter);
                    bool isBreakingRecord = (currentDistance + delta_candidate < globalBestDistance);

                    if (isTabu && !isBreakingRecord) continue;

                    if (delta_candidate < bestDelta) 
                    {
                        bestDelta = delta_candidate;
                        best_i = i;
                        best_j = j;
                        foundMove = true;
                    }
                }
            }
        }
        // DUŻE DANE
        else 
        {
            int samples = 3 * n; 
            for (int s = 0; s < samples; s++) 
            {
                int i = rng() % n;
                int j = rng() % n;

                if (i == j) continue;
                if (i > j) std::swap(i, j);

                if (i == 0 && j == n - 1) continue;

                int prev_i = (i == 0) ? n - 1 : i - 1; 
                int next_j = (j == n - 1) ? 0 : j + 1;

                long long currentEdges = distMatrix[cities[prev_i].id][cities[i].id] + distMatrix[cities[j].id][cities[next_j].id];
                long long newEdges     = distMatrix[cities[prev_i].id][cities[j].id] + distMatrix[cities[i].id][cities[next_j].id];
                long long delta_candidate = newEdges - currentEdges;

                int id_i = cities[i].id;
                int id_j = cities[j].id;
                bool isTabu = (tabuList[id_i * n + id_j] > iter) || (tabuList[id_j * n + id_i] > iter);
                bool isBreakingRecord = (currentDistance + delta_candidate < globalBestDistance);

                if (isTabu && !isBreakingRecord) continue;

                if (delta_candidate < bestDelta) 
                {
                    bestDelta = delta_candidate;
                    best_i = i;
                    best_j = j;
                    foundMove = true;
                }
            }
        }
        
        if (!foundMove) break; 

        int best_id_i = cities[best_i].id;
        int best_id_j = cities[best_j].id;
        tabuList[best_id_i * n + best_id_j] = iter + cooldown;
        tabuList[best_id_j * n + best_id_i] = iter + cooldown;

        std::reverse(cities.begin() + best_i, cities.begin() + best_j + 1);
        currentDistance += bestDelta;
        
        if (currentDistance < globalBestDistance)
        {
            globalBestDistance = currentDistance;
            globalBestRoute = cities;
            improvementSteps++;
            idleSteps = 0;
        }
        else idleSteps++;

        if (idleSteps >= maxStepsWithoutImprovement) break;
    }
    
    return {globalBestRoute, improvementSteps, static_cast<int>(globalBestDistance)};
}