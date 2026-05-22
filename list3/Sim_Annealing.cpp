#include "LocalSearch.h"
#include "../list0/FileLoadSave.h"
#include "../list0/lab0.h"

#include <iostream>
#include <algorithm>
#include <random>
#include <cmath>

double alpha = 0.98;

double Temperature(std::vector<City>& cities, const std::vector<std::vector<int>>& distMatrix, int n)
{
    // średnia delta odległości między miastami? dla 100 sąsiadów dla miasta
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_int_distribution<> randDistribution(0,n-1);

    double delta_sum = 0.0;
    int count_worse = 0;

    for(int step = 0; step < 100; step++)
    {
        int rand1 = 0;
        int rand2 = 0;
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

        if (delta_candidate > 0)
        {
            delta_sum += delta_candidate;
            count_worse++;
        }
    }
    
    if (count_worse == 0) return 1000.0;

    double Temp = (-delta_sum / count_worse) / std::log(0.95);  //Temp = (delta_sum/100.0) * 0.95
    return Temp;
}

Result Simulated_Annealing(std::vector<City>& cities, const std::vector<std::vector<int>>& distMatrix)
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

    double Temp = Temperature(cities, distMatrix, n); //
    int no_change_count = 0;
    int global_best_distance = currentDistance;
    std::vector<City> global_best_cities = cities;

    while (no_change_count < 50 && Temp >= 0.001)
    {
        bool foundBetterGlobal = false;
        int delta_candidate = 0;

        for(int step=0; step < 10*n; step++) // ---------- 10*n
        {
            int rand1 = 0;
            int rand2 = 0;
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

            delta_candidate = newEdges - currentEdges;

            if(delta_candidate < 0)
            {
                std::reverse(cities.begin() + i, cities.begin() + j + 1);
                currentDistance += delta_candidate;
                improvementSteps++;
            }
            else //
            {
                double probability = std::exp(-delta_candidate / Temp);

                std::bernoulli_distribution distribution{probability};
                if (distribution(gen)) 
                {
                    std::reverse(cities.begin() + i, cities.begin() + j + 1);
                    currentDistance += delta_candidate;
                    improvementSteps++;
                }
                //prawdopodobieństwo przyjęcia nowego rozwiązania używając wzoru e( f(X) - f(X') )/T i z tym prawdopodobieństwem X := X'
            }

            if(currentDistance < global_best_distance)
            {
                global_best_distance = currentDistance;
                global_best_cities = cities;
                foundBetterGlobal = true;
            }
        }

        Temp *= alpha;

        if(!foundBetterGlobal)
        {
            no_change_count++;
        }
        else no_change_count = 0;
    }

    return {global_best_cities, improvementSteps, global_best_distance};
}