#ifndef SIMULATIONENGINE_H
#define SIMULATIONENGINE_H

#include <vector>
#include <map>
#include "ABCMethod.h"
#include "Parameter.h"

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

private:
    std::vector<Parameter> parameters;

    ABCMethod abcMethod;

    SKUData skuData;
    
    std::map<std::string, double> normalizedFeatures;

    std::map<std::string, double> noNormalizedFeatures;

    std::map<std::string, double> meanValues;
    
    std::map<std::string, double> stdValues;
};

#endif // SIMULATIONENGINE_H