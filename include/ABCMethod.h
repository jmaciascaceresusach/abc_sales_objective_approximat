#ifndef ABCMETHOD_H
#define ABCMETHOD_H

#include <vector>
#include <string>
#include <functional>
#include <map>
#include "Parameter.h"

struct PriceInterval {
    double minPrice;
    double maxPrice;
    int count;
};

struct SKUData {
    std::string sku;
    std::vector<PriceInterval> intervals;
    double globalMinPrice;
    double globalMaxPrice;
    std::vector<std::pair<double, double>> listProducts;
};

class ABCMethod {
public:
    ABCMethod();

    void initializeParameters(std::vector<Parameter>& parameters);

    void refineParameters(std::vector<Parameter>& parameters, 
                          const SKUData& skuData,
                          const std::map<std::string, double>& normalizedFeatures,
                          int daysToSimulate,
                          double tolerance);
                          
    std::vector<double> simulateFuturePrices(const SKUData& skuData, 
                                             const std::map<std::string, double>& normalizedFeatures,
                                             int daysToSimulate);

    double calculateDistance(const std::vector<double>& simulatedPrices, 
                             const SKUData& skuData);

private:
    void normalizeParameters(std::vector<Parameter>& parameters);
};

#endif // ABCMETHOD_H