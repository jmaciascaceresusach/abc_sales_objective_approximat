#ifndef DATALOADER_H
#define DATALOADER_H

#include <vector>
#include <string>
#include <map>
#include "ABCMethod.h" // Para la definición de SKUData

/*
Comentarios generales:
- Este archivo contiene funciones auxiliares para cargar datos desde archivos.

Comentario específicos:
- loadSKUData, loadNormalizedFeatures, loadNoNormalizedFeatures, loadSimulationConfig: Funciones para cargar datos de SKU, características normalizadas y no normalizadas, y la configuración de simulación.
- getCurrentDate, inverse_z_score, calculate_z_score, loadValues: Funciones para manejar fechas y normalización de datos.
*/

class SimulationEngine; // 04-08-2024 1810

class HistoricalData { // 04-08-2024 1820
public:
    std::vector<std::string> features;

    std::vector<std::map<std::string, double>> records;
    
    void loadFromCSV(const std::string& filename);
};

SKUData loadSKUData(const std::string& filename);

std::map<std::string, double> loadNormalizedFeatures(const std::string& filename);

std::map<std::string, double> loadNoNormalizedFeatures(const std::string& filename);

void loadSimulationConfig(const std::string& filename, int& numberOfIterations, int& numberOfRefinements, int& tolerance, int& daysToSimulate, std::string& dayForSimulate, std::string& skuForSimulate); // 05-08-2024 1004

std::map<std::string, double> loadValues(const std::string& filename); // 04-08-2024 1714

std::map<std::string, double> loadAttributeWeights(const std::string& filename); // 04-08-2024 1714

std::map<std::string, std::vector<std::pair<double, double>>> loadSKUIntervals(const std::string& filename); // 04-08-2024 1714

std::vector<std::string> getAllSKUs(); // 04-08-2024 1714

//void runSimulationForSKU(SimulationEngine& simulationEngine, const std::string& sku, const std::string& dayForSimulate, int numberOfIterations, int daysToSimulate, double tolerance); // 04-08-2024 1810

std::string getCurrentDate();

std::map<std::string, double> inverse_z_score(const std::map<std::string, double>& z_score_normalized,
                                              const std::map<std::string, double>& mean_values,
                                              const std::map<std::string, double>& std_values);

std::map<std::string, double> calculate_z_score(const std::map<std::string, double>& registro,
                                                const std::map<std::string, double>& mean_values,
                                                const std::map<std::string, double>& std_values);

std::map<std::string, double> loadValues(const std::string& filename);

#endif // DATALOADER_H