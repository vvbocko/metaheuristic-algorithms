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

void LocalSearch(std::vector<City>& cities)
{
    randomPermutation(cities);  // 1. Wybierz rozwiązanie początkowe (w dowolny sposób), które staje się rozwiązaniem aktualnym. // A C D B
        std::vector<City> citiesCopy = cities; // zrób kopię
        std::vector<City> bestRoute = cities; // zrób kopię

        int startSolution = calculateTotalDistance(cities); // policz łączną trasę
        int bestCandidate = startSolution;

        bool foundBetterSolution = true;

    while (foundBetterSolution) 
    {
        foundBetterSolution = false;

        for(int i = 0; i < citiesCopy.size() -1; i++) 
        {
            for(int j = i +1; j < citiesCopy.size(); j++) 
            {
                std::reverse(citiesCopy.begin() + i, citiesCopy.begin() + j + 1); //add +1,cause when it takes a second argument as a point before which the function must end
                int candidateSolution = calculateTotalDistance(citiesCopy); // SUPER NIEOPTYMALNE (n^3) - cały środek inverta nie zmienia długości |C D E F|=|F E D C| dla ( A B |C D E F| G --> A B |F E D C| G), jedyne co się zminia to dwa graniczne połączenia (B C --> B F) oraz (F G --> C G)

                if(candidateSolution < bestCandidate)
                {
                    bestCandidate = candidateSolution;
                    bestRoute = citiesCopy;
                    foundBetterSolution = true;
                }

                std::reverse(citiesCopy.begin() + i, citiesCopy.begin() + j + 1);
            }
        }
    }
    if(foundBetterSolution)
    {
        citiesCopy = bestRoute;
    }
}