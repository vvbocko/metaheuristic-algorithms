#pragma once

#include "FileLoadSave.h"
#include <vector>

int calculateDistance(const City& a, const City& b);

void randomPermutation(std::vector<City>& cities);

int calculateTotalDistance(std::vector<City>& cities);