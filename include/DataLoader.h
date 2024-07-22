#ifndef DATALOADER_H
#define DATALOADER_H

#include <vector>
#include <string>
#include <map>
#include "ABCMethod.h" // Para la definición de SKUData

SKUData loadSKUData(const std::string& filename);

std::map<std::string, double> loadNormalizedFeatures(const std::string& filename);

void loadSimulationConfig(const std::string& filename, int& numberOfIterations, double& tolerance, int& daysToSimulate);

#endif // DATALOADER_H