#ifndef SIMULATIONENGINE_H
#define SIMULATIONENGINE_H

#include <vector>
#include "ABCMethod.h"
#include "Parameter.h"

class SimulationEngine {
public:
    SimulationEngine();

    void addParameter(const Parameter& parameter);
    void setProductData(const SKUData& data);
    void setNormalizedFeatures(const std::vector<double>& features);
    void runSimulations(int numberOfIterations, int daysToSimulate, double tolerance);

private:
    std::vector<Parameter> parameters;
    ABCMethod abcMethod;
    SKUData skuData;
    std::vector<double> normalizedFeatures;
};

#endif // SIMULATIONENGINE_H