#ifndef DATALOADER_H
#define DATALOADER_H

#include <vector>
#include <string>
#include "ABCMethod.h" // Para la definición de SKUData

SKUData loadSKUData(const std::string& filename);
std::vector<double> loadNormalizedFeatures(const std::string& filename);

#endif // DATALOADER_H