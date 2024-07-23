#ifndef SIMULATIONENGINE_H
#define SIMULATIONENGINE_H

#include <vector>
#include <map>
#include "ABCMethod.h"
#include "Parameter.h"

class SimulationEngine {
public:
    SimulationEngine();

    void addParameter(const Parameter& parameter);

    void setProductData(const SKUData& data);

    void setNormalizedFeatures(const std::map<std::string, double>& features);

    void setNoNormalizedFeatures(const std::map<std::string, double>& features);

    void runSimulations(int numberOfIterations, int daysToSimulate, double tolerance);

private:
    std::vector<Parameter> parameters;

    ABCMethod abcMethod;

    SKUData skuData;
    
    std::map<std::string, double> normalizedFeatures;

    std::map<std::string, double> noNormalizedFeatures;
};

#endif // SIMULATIONENGINE_H