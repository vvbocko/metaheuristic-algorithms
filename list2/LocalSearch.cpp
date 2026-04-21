#include "LocalSearch.h"
#include "FileLoadSave.h"
#include "lab0.h"

#include <iostream>
#include <algorithm>
#include <random>

// ZADANIE 1
Result LocalSearch_Invert(std::vector<City>& cities, const std::vector<std::vector<int>>& distMatrix)
{
    int n = cities.size();
    int improvementSteps = 0;
    randomPermutation(cities);

    int currentDistance = 0;
    for (int i = 0; i < n - 1; i++) 
    {
        currentDistance += distMatrix[cities[i].id][cities[i+1].id];
    }
    currentDistance += distMatrix[cities[n-1].id][cities[0].id];

    bool foundBetterSolution = true;
    while (foundBetterSolution) 
    {
        foundBetterSolution = false;
        int bestDelta = 0;
        int best_i = 0; 
        int best_j = 0;

        for(int i=0; i<n-1; i++) 
        {
            for(int j=i+1; j<n; j++) 
            {    
                if (i == 0 && j == n-1) continue;  
                
                int prev_i = (i == 0) ? n-1 : i-1; 
                int next_j = (j == n-1) ? 0 : j+1;

                int currentEdges = distMatrix[cities[prev_i].id][cities[i].id] + distMatrix[cities[j].id][cities[next_j].id];
                int newEdges = distMatrix[cities[prev_i].id][cities[j].id] + distMatrix[cities[i].id][cities[next_j].id];
                int delta_candidate = newEdges - currentEdges;

                if(delta_candidate < bestDelta)
                {
                    bestDelta = delta_candidate;
                    best_i = i;
                    best_j = j;
                }
            }
        }

        if(bestDelta < 0)
        {
            std::reverse(cities.begin() + best_i, cities.begin() + best_j + 1);
            currentDistance += bestDelta;
            improvementSteps++;
            foundBetterSolution = true;
        }
    }
    return {cities, improvementSteps, currentDistance};
}


// ZADANIE 2
Result LocalSearch_RandomInvert(std::vector<City>& cities, const std::vector<std::vector<int>>& distMatrix)
{
    int n = cities.size();
    int improvementSteps = 0;
    randomPermutation(cities);

    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_int_distribution<> randDistribution(0,n-1);

    int currentDistance = 0;
    for (int i = 0; i < n - 1; i++) 
    {
        currentDistance += distMatrix[cities[i].id][cities[i+1].id];
    }
    currentDistance += distMatrix[cities[n-1].id][cities[0].id];

    bool foundBetterSolution = true;
    while (foundBetterSolution) 
    {
        foundBetterSolution = false;
        int bestDelta = 0;
        int best_i = 0; 
        int best_j = 0;

        for(int step=0; step<n; step++)
        {
            int rand1, rand2;
            do {
                rand1 = randDistribution(gen);
                rand2 = randDistribution(gen);
            } while (rand1 == rand2);

            int i = std::min(rand1, rand2);
            int j = std::max(rand1, rand2);

            if (i == 0 && j == n-1) continue;

            int prev_i = (i == 0) ? n-1 : i-1;
            int next_j = (j == n-1) ? 0 : j+1;

            int currentEdges = distMatrix[cities[prev_i].id][cities[i].id] + distMatrix[cities[j].id][cities[next_j].id];
            int newEdges = distMatrix[cities[prev_i].id][cities[j].id] + distMatrix[cities[i].id][cities[next_j].id];
            int delta_candidate = newEdges - currentEdges;

            if(delta_candidate < bestDelta)
            {
                bestDelta = delta_candidate;
                best_i = i;
                best_j = j;
            }
        }

        if(bestDelta < 0)
        {
            std::reverse(cities.begin() + best_i, cities.begin() + best_j + 1);
            currentDistance += bestDelta;
            improvementSteps++;
            foundBetterSolution = true;
        }
    }
    return {cities, improvementSteps, currentDistance};
}


// ZADANIE 3
Result LocalSearch_Swap(std::vector<City>& cities, const std::vector<std::vector<int>>& distMatrix)
{
    int n = cities.size();
    int improvementSteps = 0;
    randomPermutation(cities);

    int currentDistance = 0;
    for (int i = 0; i < n - 1; i++) 
    {
        currentDistance += distMatrix[cities[i].id][cities[i+1].id];
    }
    currentDistance += distMatrix[cities[n-1].id][cities[0].id];

    bool foundBetterSolution = true;
    while (foundBetterSolution) 
    {
        foundBetterSolution = false;
        int bestDelta = 0;
        int best_i = 0; 
        int best_j = 0;
        
        for(int i=0; i<n-1; i++) 
        {
            for(int j=i+1; j<n; j++) 
            {               
                int prev_i = (i == 0) ? n-1 : i-1;
                int next_i = (i == n-1) ? 0 : i+1;
                
                int prev_j = (j == 0) ? n-1 : j-1;
                int next_j = (j == n-1) ? 0 : j+1;

                int currentEdges = 0;
                int newEdges = 0;

                if(next_i == j)
                {
                    currentEdges = distMatrix[cities[prev_i].id][cities[i].id] + distMatrix[cities[j].id][cities[next_j].id];
                    newEdges = distMatrix[cities[prev_i].id][cities[j].id] + distMatrix[cities[i].id][cities[next_j].id];
                }
                else if (i == 0 && j == n - 1)
                {
                    currentEdges = distMatrix[cities[i].id][cities[next_i].id] + distMatrix[cities[prev_j].id][cities[j].id];
                    newEdges = distMatrix[cities[j].id][cities[next_i].id] + distMatrix[cities[prev_j].id][cities[i].id];
                }         
                else
                {
                    currentEdges = distMatrix[cities[prev_i].id][cities[i].id] + distMatrix[cities[i].id][cities[next_i].id] +
                                    distMatrix[cities[prev_j].id][cities[j].id] + distMatrix[cities[j].id][cities[next_j].id];

                    newEdges = distMatrix[cities[prev_i].id][cities[j].id] + distMatrix[cities[j].id][cities[next_i].id] +
                                distMatrix[cities[prev_j].id][cities[i].id] + distMatrix[cities[i].id][cities[next_j].id];
                }

                int delta_candidate = newEdges - currentEdges;

                if(delta_candidate < bestDelta)
                {
                    bestDelta = delta_candidate;
                    best_i = i;
                    best_j = j;
                }
            }
        }

        if(bestDelta < 0)
        {
            std::swap(cities[best_i], cities[best_j]);
            currentDistance += bestDelta;
            improvementSteps++;
            foundBetterSolution = true;
        }
    }
    return {cities, improvementSteps, currentDistance};
}