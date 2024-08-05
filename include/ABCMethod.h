#ifndef ABCMETHOD_H
#define ABCMETHOD_H

#include <vector>
#include <string>
#include <functional>
#include <map>
#include "Parameter.h"

/*
Comentarios generales:
- Este archivo contiene la definición de la clase ABCMethod, que implementa el método Approximate Bayesian Computation (ABC) para la simulación de precios futuros.

Comentario específicos:
- PriceInterval y SKUData: Estructuras que contienen información relevante sobre los intervalos de precios y datos del SKU, respectivamente.
- initializeParameters, refineParameters: Métodos para inicializar y refinar los parámetros del modelo.
- simulateFuturePrices, calculateDistance, calculateProbabilityNoLog, calculateProbability: Métodos principales para la simulación de precios, cálculo de distancias y probabilidades.
*/

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

    // 05-08-2024 1025
    void refineParameters(std::vector<Parameter>& parameters, 
                        const SKUData& skuData,
                        const std::map<std::string, double>& normalizedFeatures,
                        int daysToSimulate,
                        double tolerance,
                        int numberOfRefinements);
                          
    std::vector<double> simulateFuturePrices(const SKUData& skuData, 
                                             const std::map<std::string, double>& normalizedFeatures,
                                             int daysToSimulate,
                                             double initialPrice);

    double calculateDistance(const std::vector<double>& simulatedPrices, 
                             const SKUData& skuData,
                             double initialPrice,
                             int daysToSimulate);

    void setHistoricalData(const std::vector<std::map<std::string, double>>& data);

    void sensitivityAnalysis(const SKUData& skuData); // 05-08-2024 1105

    bool verifyInputData(const SKUData& skuData); // 05-08-2024 1105

private:
    void normalizeParameters(std::vector<Parameter>& parameters);
    
    double calculateProbability(double price, const SKUData& skuData, int day);

    double calculateProbabilityNoLog(double price, const SKUData& skuData, int day); // 05-08-2024 1532

    double calculateHistoricalTrend(double price, int day, const std::vector<std::map<std::string, double>>& historicalData); // 04-08-2024 1714

    double calculateSeasonality(int day); // 04-08-2024 1714

    double getExternalFactor(int day); // 04-08-2024 1714

    double calculateAutocorrelation(double price, const std::vector<double>& previousPrices); // 04-08-2024 1714

    double calculateVolatility(const std::vector<std::map<std::string, double>>& historicalData); // 04-08-2024 1714

    std::vector<double> previousPrices; // 04-08-2024 1714

    std::vector<std::map<std::string, double>> historicalData; // 04-08-2024 1714
};

#endif // ABCMETHOD_H