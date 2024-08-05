#ifndef SIMULATIONENGINE_H
#define SIMULATIONENGINE_H

#include <vector>
#include <map>
#include "ABCMethod.h"
#include "Parameter.h"
#include "DataLoader.h" // 04-08-2024 1820

/*
Comentarios generales:
- Este archivo contiene la definición de la clase SimulationEngine, que gestiona la ejecución de las simulaciones.

Comentario específicos:
- SimulationEngine: Clase principal para manejar la simulación, incluyendo la configuración de parámetros y la ejecución de simulaciones.
*/

class SimulationEngine {
public:
    SimulationEngine();

    void addParameter(const Parameter& parameter);

    void setProductData(const SKUData& data);

    void setNormalizedFeatures(const std::map<std::string, double>& features);

    void setNoNormalizedFeatures(const std::map<std::string, double>& features);

    void runSimulations(int numberOfIterations, int daysToSimulate, double tolerance);

    void loadMeanAndStdValues(const std::string& meanFilename, const std::string& stdFilename);

    void setDayForSimulate(const std::string& day) { dayForSimulate = day; } // 04-08-2024 2012

private:
    std::vector<Parameter> parameters;

    ABCMethod abcMethod;
    
    std::map<std::string, double> normalizedFeatures;

    std::map<std::string, double> noNormalizedFeatures;

    std::map<std::string, double> meanValues;
    
    std::map<std::string, double> stdValues;

    std::map<std::string, double> attributeWeights; // 04-08-2024 1714

    std::map<std::string, std::vector<std::pair<double, double>>> skuIntervals; // 04-08-2024 1714

    double normalize(double value, const std::string& feature); // 04-08-2024 1714

    double denormalize(double normalizedValue, const std::string& feature); // 04-08-2024 1714

    HistoricalData historicalData; // 04-08-2024 1714

    void loadHistoricalData(const std::string& filename); // 04-08-2024 1714

    std::string dayForSimulate; // 04-08-2024 2013

    SKUData skuData; // 04-08-2024 2013
};

#endif // SIMULATIONENGINE_H