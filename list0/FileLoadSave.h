#pragma once

#include "City.h"

#include <vector>
#include <string>

std::vector<City> loadCitiesFile(const std::string& filename);

void saveRouteToFile(const std::vector<City>& route, const std::string& filename);