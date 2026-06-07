#include "LocalSearch.h"
#include "../list0/FileLoadSave.h"
#include "../list0/CalculationTools.h"

#include <iostream>
#include <algorithm>
#include <random>
#include <cmath>

double alpha = 0.98;

Result Simulated_Annealing(std::vector<City>& cities, const std::vector<std::vector<int>>& distMatrix)
{
    int n = cities.size();
    int improvementSteps = 0;
    randomPermutation(cities);

    static std::random_device rd;
    static std::mt19937 gen(rd());

    double delta_sum = 0.0;
    int count_worse = 0;

    for(int step = 0; step < 100; step++)
    {
        int rand1 = 0;
        int rand2 = 0;
        do {
            rand1 = gen() % n;
            rand2 = gen() % n;
        } while (rand1 == rand2);

        int i = std::min(rand1, rand2);
        int j = std::max(rand1, rand2);

        if (i == 0 && j == n-1) continue;

        int prev_i = (i == 0) ? n-1 : i-1;
        int next_j = (j == n-1) ? 0 : j+1;

        int currentEdges = distMatrix[cities[prev_i].id][cities[i].id] + distMatrix[cities[j].id][cities[next_j].id];
        int newEdges = distMatrix[cities[prev_i].id][cities[j].id] + distMatrix[cities[i].id][cities[next_j].id];
        int delta_candidate = newEdges - currentEdges;

        if (delta_candidate > 0)
        {
            delta_sum += delta_candidate;
            count_worse++;
        }
    }

    double Temp = (count_worse > 0) ? (delta_sum / count_worse) / (-std::log(0.95)) : 1000.0;

    int currentDistance = 0;
    for (int i = 0; i < n - 1; i++) 
    {
        currentDistance += distMatrix[cities[i].id][cities[i+1].id];
    }
    currentDistance += distMatrix[cities[n-1].id][cities[0].id];

    int global_best_distance = currentDistance;
    std::vector<City> global_best_cities = cities;

    while(Temp > 0.0005)
    {
        int epoch_length = n * 30;
        for(int step = 0; step < epoch_length; step++)
        {
            int rand1 = 0;
            int rand2 = 0;
            do {
                rand1 = gen() % n;
                rand2 = gen() % n;
            } while (rand1 == rand2);

            int i = std::min(rand1, rand2);
            int j = std::max(rand1, rand2);

            if (i == 0 && j == n-1) continue;

            int prev_i = (i == 0) ? n-1 : i-1;
            int next_j = (j == n-1) ? 0 : j+1;

            int currentEdges = distMatrix[cities[prev_i].id][cities[i].id] + distMatrix[cities[j].id][cities[next_j].id];
            int newEdges = distMatrix[cities[prev_i].id][cities[j].id] + distMatrix[cities[i].id][cities[next_j].id];

            int delta_candidate = newEdges - currentEdges;

            if(delta_candidate < 0)
            {
                std::reverse(cities.begin() + i, cities.begin() + j + 1);
                currentDistance += delta_candidate;
                improvementSteps++;
            }
            else 
            {
                double probability = std::exp(-delta_candidate / Temp);

                std::bernoulli_distribution distribution{probability};
                if (distribution(gen)) 
                {
                    std::reverse(cities.begin() + i, cities.begin() + j + 1);
                    currentDistance += delta_candidate;
                    improvementSteps++;
                }
            }

            if(currentDistance < global_best_distance)
            {
                global_best_distance = currentDistance;
                global_best_cities = cities;
            }
        }

        Temp *= alpha;
    }

    return {global_best_cities, improvementSteps, global_best_distance};
}