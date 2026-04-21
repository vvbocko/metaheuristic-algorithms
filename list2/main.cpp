#include "LocalSearch.h"
#include "FileLoadSave.h"
#include "lab0.h"

#include <limits>
#include <vector>
#include <iostream>
#include <fstream>
#include <string>

int main()
{
    std::ofstream resultsFile("wyniki3_0.txt", std::ios::app); 
    // "wi29.tsp", "dj38.tsp", "qa194.tsp", "uy734.tsp", "zi929.tsp"
    std::vector<std::string> filenames = { "wi29.tsp", "dj38.tsp", "qa194.tsp", "uy734.tsp", "zi929.tsp" }; //"mu1979", "ca4663", "tz6117", "eg7146", "ei8246"
    LocalSearch activeAlgorithm = LocalSearch_Swap; //LocalSearch_Swap , LocalSearch_RandomInvert , LocalSearch_Invert
    std::string taskSuffix = "_zad3";


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

        std::cout << "\n macierz odleglosci " << currentFile;
        std::vector<std::vector<int>> distMatrix(n, std::vector<int>(n, 0));
        for (int i = 0; i < n; i++) 
        {
            for (int j = 0; j < n; j++) 
            {
                distMatrix[i][j] = calculateDistance(originalCities[i], originalCities[j]);
            }
        }

        int iterations = n; //std::min(n, 100);
        long long distance_sum = 0;
        long long steps_sum = 0;
        int bestDistance = std::numeric_limits<int>::max();

        std::cout << "\n" << currentFile << "\n";

        for(int i=0; i<iterations ; i++)
        {
            std::vector<City> Copy = originalCities;
            
            Result result = activeAlgorithm(Copy, distMatrix);

            distance_sum += result.totalDistance;
            steps_sum += result.improvementSteps;
            
            if(result.totalDistance < bestDistance)
            {
                bestDistance = result.totalDistance;
                bestRoute = result.bestRoute;
            }

            if ((i + 1) % 10 == 0) //(i + 1) % 10 == 0
            {
                std::cout << "postep:  "<< (i + 1) << " / " << iterations << "\n";
            }
        }
        
        double distance_mean = static_cast<double>(distance_sum) / iterations;
        double steps_mean = static_cast<double>(steps_sum) / iterations;
        
        resultsFile << "Wyniki dla: " << currentFile << "\n";
        resultsFile << "Srednia wartosc: " << distance_mean << "\n";
        resultsFile << "Srednie kroki: " << steps_mean << "\n";
        resultsFile << "Najlepsze rozwiazanie: " << bestDistance << "\n\n";

        std::string baseFileName = currentFile.substr(0, currentFile.find('.'));
        std::string outputFileName = "bestRoute_" + baseFileName + taskSuffix + ".txt";
        
        saveRouteToFile(bestRoute, outputFileName);
    }
    return 0;
}