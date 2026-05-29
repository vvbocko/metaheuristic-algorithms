#include "LocalSearch.h"
#include "../list0/FileLoadSave.h"
#include "../list0/lab0.h"

#include <limits>
#include <vector>
#include <unordered_map>
#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
#include <chrono>


int main()
{
    std::ofstream resultsFile("wyniki_tabuD.txt", std::ios::app); 
    std::vector<std::string> filenames = { "eg7146.tsp"};// "tz6117.tsp", "eg7146.tsp", "ei8246.tsp"}; //"wi29.tsp" ,  "dj38.tsp", "qa194.tsp", "uy734.tsp", "zi929.tsp", "mu1979.tsp", "ca4663.tsp", "tz6117.tsp", "eg7146.tsp", "ei8246.tsp"  }; ////// "wi29.tsp", "dj38.tsp", "qa194.tsp", "uy734.tsp", "zi929.tsp" "mu1979.tsp", "ca4663.tsp", "tz6117.tsp", "ei8246.tsp"
    //LocalSearch activeAlgorithm = Simulated_Annealing; //Simulated_Annealing, TabuSearch
    std::string taskSuffix = "_tabuD";

    std::unordered_map<std::string, int> improvement_num =
    {
        {"wi29.tsp", 25},
        {"dj38.tsp", 34},
        {"qa194.tsp", 212},
        {"uy734.tsp", 923},
        {"zi929.tsp", 1174},
        {"mu1979.tsp", 2784},
        {"ca4663.tsp", 6776},
        {"tz6117.tsp", 8868},
        {"eg7146.tsp", 10259},
        {"ei8246.tsp", 12014}
    };

    
    for (size_t f=0; f<filenames.size(); f++)
    {
        std::string currentFile = filenames[f];
        std::vector<City> originalCities = loadCitiesFile(currentFile);
        if (originalCities.empty()) std::cout << "Puste: " << currentFile << "\n";

        int n = originalCities.size();
        for (int i = 0; i < n; i++) 
        {
            originalCities[i].id = i;
        }
        std::vector<City> bestRoute = originalCities;
        
        
        //długość listy tabu (można wykorzystać wyniki z poprzedniego zadania, czyli ustalić jakąś wielokrotność liczby poprawek prowadzących do lokalnego minimum)
        int tabu_length = std::max(10, static_cast<int>(std::sqrt(improvement_num.at(currentFile))));
        
        std::cout << "\n macierz odleglosci " << currentFile;
        std::vector<std::vector<int>> distMatrix(n, std::vector<int>(n, 0));
        for (int i = 0; i < n; i++) 
        {
            for (int j = 0; j < n; j++) 
            {
                distMatrix[i][j] = calculateDistance(originalCities[i], originalCities[j]);
            }
        }

        int iterations = 100; //std::min(n, 100); // 100 - dla dużych // 50 - dla testowych
        long long distance_sum = 0;
        long long steps_sum = 0;
        int bestDistance = std::numeric_limits<int>::max();

        std::cout << "\n" << currentFile << "\n";

        auto start = std::chrono::high_resolution_clock::now();
        for(int i=0; i<iterations ; i++)
        {
            std::vector<City> Copy = originalCities;
            
            //Result result = activeAlgorithm(Copy, distMatrix); //
            Result result = TabuSearch(Copy, distMatrix, tabu_length); //

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
        auto end = std::chrono::high_resolution_clock::now();
        double elapsed = std::chrono::duration<double>(end - start).count();
        
        double distance_mean = static_cast<double>(distance_sum) / iterations;
        double steps_mean = static_cast<double>(steps_sum) / iterations;
        
        resultsFile << "Wyniki dla: " << currentFile << "\n";
        resultsFile << "Czas: " << elapsed << "\n";
        resultsFile << "Srednia wartosc: " << distance_mean << "\n";
        resultsFile << "Srednie kroki: " << steps_mean << "\n";
        resultsFile << "Najlepsze rozwiazanie: " << bestDistance << "\n\n";

        std::string baseFileName = currentFile.substr(0, currentFile.find('.'));
        std::string outputFileName = "bestRoute_" + baseFileName + taskSuffix + ".txt";
        
        saveRouteToFile(bestRoute, outputFileName);
    }
    return 0;
}