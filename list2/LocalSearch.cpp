#include "City.h"
#include "FileLoadSave.h"
#include "lab0.h"

#include <iostream>
#include <fstream>
#include <vector>
#include <string>

// 1. Wybierz rozwiązanie początkowe (w dowolny sposób), które staje się rozwiązaniem aktualnym.
// 2. Wyznacz wartość funkcji celu wszystkich sąsiadów rozwiązania aktualnego (dla otoczenia invert).
// 3. Jako kandydata do poprawy wybierz najlepszego z ocenionych sąsiadów.
// 4. Jeśli kandydat nie jest lepszy od aktualnego rozwiązania, to zakończ algorytm.
// 5. Zastąp aktualne rozwiązanie kandydatem i przejdź do kroku 2.

int main()
{
    std::vector<std::string> filenames = {
        "mu1979.tsp", "ca4663.tsp", "tz6117.tsp", "eg7146.tsp", "ei8246.tsp"
    };

    for (size_t f = 0; f < filenames.size(); f++)
    {
        std::string currentFile = filenames[f];
        std::vector<City> cities = loadCitiesFile(currentFile);
        std::vector<City> bestRoute;

        std::cout << "\n" << currentFile << "\n";
        if (cities.empty()) std::cout << "Puste: " << currentFile << "\n";

        randomPermutation(cities);  // 1. Wybierz rozwiązanie początkowe (w dowolny sposób), które staje się rozwiązaniem aktualnym.
        

        // randomPermutation(cities);
        // int distance1 = calculateTotalDistance(cities);

        // std::string outputFileName = "bestRoute_" + currentFile.substr(0, currentFile.find('.')) + ".txt";
        // saveRouteToFile(bestRoute, outputFileName);
    }
    return 0;
}