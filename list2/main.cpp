#include "LocalSearch.h"
#include "FileLoadSave.h"
#include "lab0.h"

#include <limits>
#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>

int main()
{
    std::ofstream resultsFile("wyniki1.txt", std::ios::app); 
    std::vector<std::string> filenames = { //"mu1979.tsp", "ca4663.tsp", "tz6117.tsp", "eg7146.tsp", "ei8246.tsp"
        "mu1979.tsp", "ca4663.tsp", "tz6117.tsp", "eg7146.tsp"
    };

    for (size_t f=0; f<filenames.size(); f++)
    {
        std::string currentFile = filenames[f];
        std::vector<City> originalCities = loadCitiesFile(currentFile);
        if (originalCities.empty()) std::cout << "Puste: " << currentFile << "\n";

        std::vector<City> bestRoute = originalCities;
        
        int n = originalCities.size();

        for (int i = 0; i < n; i++) 
        {
            originalCities[i].id = i;
        }

        std::cout << "\nMacierz odleglosci dla " << currentFile << "...";
        std::vector<std::vector<int>> distMatrix(n, std::vector<int>(n, 0));
        for (int i = 0; i < n; i++) 
        {
            for (int j = 0; j < n; j++) 
            {
                distMatrix[i][j] = calculateDistance(originalCities[i], originalCities[j]);
            }
        }
        std::cout << " koniec\n";

        int iterations = std::min(n, 10);
        long long distance_sum = 0;
        long long steps_sum = 0;
        int bestDistance = std::numeric_limits<int>::max();

        std::cout << "\n" << currentFile << "\n";

        #pragma omp parallel for reduction(+:distance_sum, steps_sum)
        for(int i=0; i<iterations ; i++)
        {
            std::vector<City> Copy = originalCities;
            Result result = LocalSearch(Copy, distMatrix);

            distance_sum += result.totalDistance;
            steps_sum += result.improvementSteps;
            
            #pragma omp critical
            if(result.totalDistance < bestDistance)
            {
                bestDistance = result.totalDistance;
                bestRoute = result.bestRoute;
            }

            if (true) //(i + 1) % 10 == 0
            {
                #pragma omp critical
                std::cout << "Postep:  "<< (i + 1) << " / " << iterations << "\n";
            }
        }
        double distance_mean = static_cast<double>(distance_sum) / iterations;
        double steps_mean = static_cast<double>(steps_sum) / iterations;
        
        resultsFile << "Wyniki dla: " << currentFile << "\n";
        resultsFile << "Srednia wartosc: " << distance_mean << "\n";
        resultsFile << "Srednie kroki: " << steps_mean << "\n";
        resultsFile << "Najlepsze rozwiazanie: " << bestDistance << "\n\n";

        std::string outputFileName = "bestRoute_" + currentFile.substr(0, currentFile.find('.')) + ".txt";
        saveRouteToFile(bestRoute, outputFileName);
    }
    return 0;
}