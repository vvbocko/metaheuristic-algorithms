#include <iostream>
#include <algorithm>
#include <random>
#include <vector>
#include <fstream>
#include <chrono>
#include <limits>

#include "../list0/FileLoadSave.h"
#include "../list0/CalculationTools.h"

// -------- ZADANIE ------
// 1. Wyznaczenie populacji początkowej
// 2. Ewaluacja i selekcja
// 3. Krzyżowanie
// 4. Mutacja
// 5. Jeśli nie osiągnięto warunku stopu to wracamy do 2.

// Krok 1: generacja populacji startowej przy użyciu algorytmów wyspowych
// Krok 2: Selekcja             - sposób wybrania populacji rodziców (silny osobnik może być wybrany kilka razy)
// Krok 3: Krzyżowanie          - powstałe dzieci/permutacje muszą być rozwiązaniami dopuszczalnymi (faktycznymi permutacjami)
// Krok 4: Prawdopod. Mutacji   - losowe zaburzenie - algorytm memetyczny 2-OPT
// Krok 5: Iteracje/Pokolenia   - Warunki stopu (liczba iteracji, brak poprawy)

const int POPULATION_SIZE = 100;   // pokolenie startowe - pokolenie 0
const int ISLANDS_NUM = 6;
const int TOURNAMENT_SIZE = 3; 
const double CROSSOVER_PROB = 0.8;
const double MUTATION_PROB = 0.05;

// Należy pamiętać, że rozmiar populacji w trakcie czasu pracy algorytmu najczęściej jest stały.
// Każdą selekcję poprzedza ewaluacja (ocena) populacji.

struct Specimen
{   
    std::vector<City> genotype;
    long long fenotype;

    Specimen() : genotype(), fenotype(std::numeric_limits<long long>::max()) {}

    Specimen(std::vector<City>& cities)
    {
        genotype = cities;
        randomPermutation(genotype);
        fenotype = calculateTotalDistance(genotype);
    };
};

//"Zbyt podobna populacja początkowa można prowadzić do przedwczesnej zbieżności algorytmu"
std::vector<Specimen> InitializePopulationZero(std::vector<City>& cities)
{
    std::vector<Specimen> population;
    population.reserve(POPULATION_SIZE);
    for(size_t i = 0; i < POPULATION_SIZE; i++) // vector to kontener więc zamiast int zaleca się size_t - 1. zawsze >=0, 2. 
    {
        Specimen specimen(cities);
        population.push_back(specimen);
    }
    return population;
}

std::vector<std::vector<Specimen>> InitializeIsland(std::vector<City>& cities) 
{
    std::vector<std::vector<Specimen>> island;
    island.reserve(ISLANDS_NUM);
    for(size_t i = 0; i < ISLANDS_NUM; i++)
    {
        island.push_back(InitializePopulationZero(cities));
    }
    return island;
}

std::vector<Specimen> SelectionTournament(const std::vector<Specimen>& currentPopulation)
{
    std::vector<Specimen> selectedParents;
    selectedParents.reserve(POPULATION_SIZE);

    static std::random_device rd;
    static std::mt19937 gen(rd());

    for (size_t i = 0; i < POPULATION_SIZE; i++)
    {
        size_t randomIndex = gen() % POPULATION_SIZE;
        Specimen bestInTournament = currentPopulation[randomIndex];

        for (int t = 1; t < TOURNAMENT_SIZE; t++)
        {
            size_t nextRandomIndex = gen() % POPULATION_SIZE;
            const Specimen& competitor = currentPopulation[nextRandomIndex];

            if (competitor.fenotype < bestInTournament.fenotype)
            {
                bestInTournament = competitor;
            }
        }
        selectedParents.push_back(bestInTournament);
    }
    return selectedParents;
}

void Selection(std::vector<std::vector<Specimen>>& islands)
{
    // iteracja po wyspach
    // dla każdej wyspy wybrać z populacji najsilniejszego osobnika

    for(size_t i = 0; i < islands.size(); i++)
    {
        islands[i] = SelectionTournament(islands[i]);
    }
}

// Osobnik - zbiór miast (permutacja miast)
// Populacja - zbiór osobników
// Wyspy - Zbiór Populacji
// Co ile pokoleń/iteracji osobniki z różnych wysp będą wymieniać się informacjami

Specimen GetChildrenOfParents(const Specimen& parent1, const Specimen& parent2)
{
    Specimen child;
    size_t numCities = parent1.genotype.size();
    child.genotype.resize(numCities);

    std::vector<bool> isAdded(numCities + 1, false);

    size_t cutPoint = numCities/2;
    for (size_t i = 0; i < cutPoint; i++)
    {
        child.genotype[i] = parent1.genotype[i];
        isAdded[parent1.genotype[i].id] = true;
    }

    size_t childIndex = cutPoint;
    for (size_t i = 0; i < numCities; i++)
    {
        City currentCityFromParent2 = parent2.genotype[i];
        
        if (!isAdded[currentCityFromParent2.id])
        {
            child.genotype[childIndex] = currentCityFromParent2;
            isAdded[currentCityFromParent2.id] = true;
            childIndex++;
        }
    }
    child.fenotype = calculateTotalDistance(child.genotype);
    return child;
}

std::vector<Specimen> Crossover(const std::vector<Specimen>& this_population)
{
    std::vector<Specimen> next_population;
    next_population.reserve(POPULATION_SIZE);

    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(0.0, 1.0);

    for (size_t i = 0; i < this_population.size(); i += 2)
    {
        const Specimen& parent1 = this_population[i];
        const Specimen& parent2 = this_population[i+1];

        if (dis(gen) < CROSSOVER_PROB)
        {
            next_population.push_back(GetChildrenOfParents(parent1, parent2));
            next_population.push_back(GetChildrenOfParents(parent2, parent1));
        }
        else
        {
            next_population.push_back(parent1);
            next_population.push_back(parent2);
        }
    }
    return next_population;
}

std::vector<City> MemeticMutation(std::vector<City>& route, const std::vector<std::vector<int>>& distMatrix)
{
    int n = route.size();
    if (n < 3) return route;

    static std::random_device rd;
    static std::mt19937 gen(rd());

    int rand1 = gen() % n;
    int rand2 = gen() % n;
    int startIdx = std::min(rand1, rand2);
    int endIdx = std::max(rand1, rand2);
    
    if (startIdx != endIdx && !(startIdx == 0 && endIdx == n - 1)) 
    {
        std::reverse(route.begin() + startIdx, route.begin() + endIdx + 1);
    }

    int localSearchAttempts = std::min(50, n);
    for (int step = 0; step < localSearchAttempts; step++)
    {
        int r1 = gen() % n;
        int r2 = gen() % n;
        if (r1 == r2) continue;

        int i = std::min(r1, r2);
        int j = std::max(r1, r2);
        if (i == 0 && j == n - 1) continue;

        int prev_i = (i == 0) ? n - 1 : i - 1;
        int next_j = (j == n - 1) ? 0 : j + 1;

        int currentEdges = distMatrix[route[prev_i].id][route[i].id] + distMatrix[route[j].id][route[next_j].id];
        int newEdges     = distMatrix[route[prev_i].id][route[j].id] + distMatrix[route[i].id][route[next_j].id];

        if (newEdges - currentEdges < 0)
        {
            std::reverse(route.begin() + i, route.begin() + j + 1);
        }
    }
    return route;
}

//trzeba bedzie zaktualizowac fenotyp
void Mutate(std::vector<Specimen>& population, const std::vector<std::vector<int>>& distMatrix)
{
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(0.0, 1.0);

    for (size_t i = 0; i < population.size(); i++)
    {
        if (dis(gen) < MUTATION_PROB)
        {      
            population[i].genotype = MemeticMutation(population[i].genotype, distMatrix);
            population[i].fenotype = calculateTotalDistance(population[i].genotype);
        }
    }
}

void GeneticAlgorithm(std::vector<City> &originalCities, const std::vector<std::vector<int>> &distMatrix, Specimen& globalBestSpecimen)
{
    std::vector<std::vector<Specimen>> islands = InitializeIsland(originalCities);
    
    int maxGenerations = 100;
    for (int generation = 0; generation < maxGenerations; generation++)
    {
        Selection(islands);

        for (size_t i = 0; i < islands.size(); i++)
        {
            islands[i] = Crossover(islands[i]);
            Mutate(islands[i], distMatrix);
        }

        for (size_t i = 0; i < islands.size(); i++) 
        {
            for (size_t j = 0; j < islands[i].size(); j++) 
            {
                if (islands[i][j].fenotype < globalBestSpecimen.fenotype) 
                {
                    globalBestSpecimen = islands[i][j];
                }
            }
        }
        
        if ((generation + 1) % 20 == 0) 
        {
            std::cout << "Pokolenie: " << (generation + 1) << "/" << maxGenerations << " | Aktualny najlepszy dystans: " << globalBestSpecimen.fenotype << "\n";
        }
    }
}

// --------------DICTIONARY -------------------
// Populacja    Zbiór wielu różnych tras (osobników) w danej iteracji algorytmu.  
// Genotyp:     Jeden lub więcej chromosomów. (Permutacja miast) 
// Chromosom:   Ciąg kodowy genów reprezentujący rozwiązanie.
// Gen:         Pojedynczy element chromosomu. (Konkretne miasto/wierzchołek w trasie) 
// Locus:       Pozycja genu w chromosomie, czyli po prostu indeks miasta w Twojej tablicy/wektorze (np. trzecie miasto w kolejności).  
// Fenotyp:     Wartość libczowa. (Długość trasy) W algorytmach genetycznych oceniamy tzw. funkcję przystosowania. W TSP jest to bezpośrednio długość trasy (im krótsza trasa, tym osobnik jest „silniejszy” i lepiej przystosowany).  
// Pokolenie:   Jedna pełna iteracja algorytmu populacyjnego. 

int main()
{
    std::ofstream resultsFile("results.txt", std::ios::app); 

    std::vector<std::string> filenames = {"wi29.tsp", "dj38.tsp", "qa194.tsp", "uy734.tsp", "zi929.tsp" "mu1979.tsp", "ca4663.tsp", "tz6117.tsp", "eg7146.tsp", "ei8246.tsp" }; // "wi29.tsp", "dj38.tsp", "qa194.tsp", "uy734.tsp", "zi929.tsp" "mu1979.tsp", "ca4663.tsp", "tz6117.tsp", "eg7146.tsp", "ei8246.tsp"
    std::string taskSuffix = "_genetic";

    for (size_t f = 0; f < filenames.size(); f++)
    {
        std::string currentFile = filenames[f];
        std::vector<City> originalCities = loadCitiesFile(currentFile);
        if (originalCities.empty())
        {
            std::cout << "Blad wczytywania pliku: " << currentFile << "\n";
            continue;
        }

        int n = originalCities.size();
        for (int i = 0; i < n; i++)
        {
            originalCities[i].id = i;
        }

        std::vector<std::vector<int>> distMatrix(n, std::vector<int>(n, 0));
        for (int i = 0; i < n; i++)
        {
            for (int j = 0; j < n; j++)
            {
                distMatrix[i][j] = calculateDistance(originalCities[i], originalCities[j]);
            }
        }

        Specimen globalBestSpecimen;
        globalBestSpecimen.fenotype = std::numeric_limits<long long>::max();

        auto start = std::chrono::high_resolution_clock::now();

        GeneticAlgorithm(originalCities, distMatrix, globalBestSpecimen);

        auto end = std::chrono::high_resolution_clock::now();
        double elapsed = std::chrono::duration<double>(end - start).count();
        
        resultsFile << "Wyniki dla: " << currentFile << taskSuffix << "\n";
        resultsFile << "Czas wykonania: " << elapsed << " s\n";
        resultsFile << "Najlepsze rozwiazanie: " << globalBestSpecimen.fenotype << "\n\n";

        std::string baseFileName = currentFile.substr(0, currentFile.find('.'));
        std::string outputFileName = "bestRoute_" + baseFileName + taskSuffix + ".txt";
        saveRouteToFile(globalBestSpecimen.genotype, outputFileName);
    }
    return 0;
}