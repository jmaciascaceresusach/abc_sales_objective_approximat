#ifndef SIMULATION_H
#define SIMULATION_H

#include "SimulationEngine.h"
#include <string>

// 04-08-2024 1810
void runSimulationForSKU(SimulationEngine& simulationEngine, const std::string& sku, const std::string& dayForSimulate, int numberOfIterations, int daysToSimulate, double tolerance);

#endif // SIMULATION_H