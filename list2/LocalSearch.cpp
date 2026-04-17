#include "LocalSearch.h"
#include "FileLoadSave.h"
#include "lab0.h"

#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>

// 1. Wybierz rozwiązanie początkowe (w dowolny sposób), które staje się rozwiązaniem aktualnym.
// 2. Wyznacz wartość funkcji celu wszystkich sąsiadów rozwiązania aktualnego (dla otoczenia invert). --> invert to algorytm 2-opt
// 3. Jako kandydata do poprawy wybierz najlepszego z ocenionych sąsiadów.
// 4. Jeśli kandydat nie jest lepszy od aktualnego rozwiązania, to zakończ algorytm.
// 5. Zastąp aktualne rozwiązanie kandydatem i przejdź do kroku 2.

//zwróci solution - wynik/trasę
Result LocalSearch(std::vector<City>& cities, const std::vector<std::vector<int>>& distMatrix)
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
                if (i == 0 && j == n-1) continue;  // odwrócona cała tablica
                
                int prev_i = (i == 0) ? n-1 : i-1; // zachowanie cykliczności
                int next_j = (j == n-1) ? 0 : j+1;

                //std::reverse(citiesCopy.begin() + i, citiesCopy.begin() + j + 1);
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
    return {cities ,improvementSteps, currentDistance};
}