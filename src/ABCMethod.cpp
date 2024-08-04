#include "../include/ABCMethod.h"
#include <random>
#include <algorithm>
#include <cmath>
#include <iostream>
#include <fstream>
#include <sstream>
#include <numeric>
#include <vector>
#include <map>
#include <string>

/*
Comentarios generales:
- Este archivo implementa los métodos definidos en ABCMethod.h.

Comentario específicos:
- Implementación de métodos como initializeParameters, refineParameters, simulateFuturePrices, calculateDistance, normalizeParameters, calculateProbability.
- calculateProbability: Método que necesita mejoras para incluir factores como tendencia histórica, estacionalidad, factores externos, autocorrelación con precios anteriores y volatilidad del producto.
*/

ABCMethod::ABCMethod() {}

void ABCMethod::initializeParameters(std::vector<Parameter>& parameters) {
    for (auto& param : parameters) {
        param.adjustProbability(0.5);
    }
}

void ABCMethod::refineParameters(std::vector<Parameter>& parameters, 
                                 const SKUData& skuData,
                                 const std::map<std::string, double>& normalizedFeatures,
                                 int daysToSimulate,
                                 double tolerance) {
    std::vector<std::vector<Parameter>> acceptedParameters;
    int numberOfSimulations = 1000;
    
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(0.0, 1.0);

    for (int i = 0; i < numberOfSimulations; ++i) {
        std::vector<Parameter> proposedParameters = parameters;
        
        // Solo ajustamos los parámetros dinámicos
        for (auto& param : proposedParameters) {
            if (param.name == "total_num_count_products" || param.name == "total_price_products") {
                std::normal_distribution<> d(param.probability, 0.1);
                param.probability = std::max(0.0, std::min(1.0, d(gen)));
            }
        }

        double initialPrice = skuData.globalMinPrice + dis(gen) * (skuData.globalMaxPrice - skuData.globalMinPrice);
        std::vector<double> simulatedPrices = simulateFuturePrices(skuData, normalizedFeatures, daysToSimulate, initialPrice);
        double distance = calculateDistance(simulatedPrices, skuData, initialPrice, daysToSimulate);

        if (distance < tolerance) {
            acceptedParameters.push_back(proposedParameters);
        }
    }

    if (!acceptedParameters.empty()) {  
        for (size_t i = 0; i < parameters.size(); ++i) {
            if (parameters[i].name == "total_num_count_products" || parameters[i].name == "total_price_products") {
                double sum = 0.0;
                for (const auto& accepted : acceptedParameters) {
                    sum += accepted[i].probability;
                }
                parameters[i].probability = sum / acceptedParameters.size();
            }
        }
        
        normalizeParameters(parameters);
    } else {
        tolerance *= 1.1;
    }
}

// 04-08-2024 1714
void ABCMethod::setHistoricalData(const std::vector<std::map<std::string, double>>& data) {
    this->historicalData = data;
}

// 04-08-2024 1714
std::vector<double> ABCMethod::simulateFuturePrices(const SKUData& skuData, 
                                                    const std::map<std::string, double>& normalizedFeatures,
                                                    int daysToSimulate,
                                                    double initialPrice) {
    std::vector<double> futurePrices;
    futurePrices.push_back(initialPrice);
    previousPrices.clear();
    previousPrices.push_back(initialPrice);

    std::random_device rd;
    std::mt19937 gen(rd());

    for (int i = 1; i < daysToSimulate; ++i) {
        std::vector<double> probabilities;
        for (const auto& interval : skuData.listProducts) {
            double prob = calculateProbability((interval.first + interval.second) / 2, skuData, i);
            probabilities.push_back(prob);
        }

        std::discrete_distribution<> distribution(probabilities.begin(), probabilities.end());
        int selectedInterval = distribution(gen);

        std::uniform_real_distribution<> intervalDist(skuData.listProducts[selectedInterval].first,
                                                      skuData.listProducts[selectedInterval].second);
        double price = intervalDist(gen);

        futurePrices.push_back(price);
        previousPrices.push_back(price);
    }

    return futurePrices;
}

double ABCMethod::calculateDistance(const std::vector<double>& simulatedPrices, 
                                    const SKUData& skuData,
                                    double initialPrice,
                                    int daysToSimulate) {
    double distance = 0.0;
    for (int i = 0; i < daysToSimulate; ++i) {
        double expectedProbability = calculateProbability(simulatedPrices[i], skuData, i);
        double actualProbability = 1.0 / skuData.listProducts.size(); // Asumiendo distribución uniforme
        distance += std::abs(expectedProbability - actualProbability);
    }
    return distance / daysToSimulate;
}

void ABCMethod::normalizeParameters(std::vector<Parameter>& parameters) {
    double totalProbability = 0.0;
    for (const auto& param : parameters) {
        if (param.name == "total_num_count_products" || param.name == "total_price_products") {
            totalProbability += param.probability;
        }
    }
    for (auto& param : parameters) {
        if (param.name == "total_num_count_products" || param.name == "total_price_products") {
            param.probability /= totalProbability;
        }
    }
}

// 04-08-2024 1714 (v2)
double ABCMethod::calculateProbability(double price, const SKUData& skuData, int day) {
    double probability = 0.0;

    // Verificar si el precio está en algún intervalo
    for (size_t i = 0; i < skuData.listProducts.size(); ++i) {
        const auto& interval = skuData.listProducts[i];
        if (price >= interval.first && price <= interval.second) {
            // Probabilidad base
            probability = 1.0 / skuData.listProducts.size();

            // Ajustar por la posición en el intervalo
            double positionInInterval = (price - interval.first) / (interval.second - interval.first);
            probability *= std::exp(-std::pow(positionInInterval - 0.5, 2) / 0.25);

            // Ajustar por el día (decae con el tiempo)
            probability *= std::exp(-0.05 * day);

            // Tendencia histórica: Multiplicamos la probabilidad por (1 + historicalTrend). Esto aumenta la probabilidad si hay una tendencia positiva y la disminuye si es negativa. 
            double historicalTrend = calculateHistoricalTrend(price, day, historicalData);
            probability *= (1 + historicalTrend);

            // Estacionalidad: Similar a la tendencia, multiplicamos por (1 + seasonality). Esto ajusta la probabilidad basándose en patrones estacionales.
            double seasonality = calculateSeasonality(day);
            probability *= (1 + seasonality);

            // Factores externos: Multiplicamos por (1 + externalFactor) para incorporar eventos externos que puedan afectar el precio.
            double externalFactor = getExternalFactor(day);
            probability *= (1 + externalFactor);

            // Autocorrelación: Multiplicamos por (1 + autocorrelation). Una autocorrelación positiva aumentará la probabilidad de que el precio se mueva en la misma dirección que los precios recientes.
            double autocorrelation = calculateAutocorrelation(price, previousPrices);
            probability *= (1 + autocorrelation);

            // Volatilidad: Usamos una función exponencial para ajustar la probabilidad basada en la volatilidad y la diferencia entre el precio actual y el anterior. Una alta volatilidad y una gran diferencia de precio reducirán la probabilidad.
            double volatility = calculateVolatility(historicalData);
            double previousPrice = previousPrices.empty() ? price : previousPrices.back();
            probability *= std::exp(-volatility * std::abs(price - previousPrice));

            break;
        }
    }

    // Normalización: Al final, aseguramos que la probabilidad esté en el rango [0, 1].
    probability = std::max(0.0, std::min(1.0, probability));

    return probability;
}

// 04-08-2024 1714 (v2). La tendencia histórica se calcula usando una regresión lineal simple sobre los últimos 30 días de datos.
double ABCMethod::calculateHistoricalTrend(double price, int day, const std::vector<std::map<std::string, double>>& historicalData) {
    if (historicalData.empty()) return 0.0;

    // Calcular la tendencia de los últimos 30 días (o menos si no hay suficientes datos)
    int daysToAnalyze = std::min(30, static_cast<int>(historicalData.size()));
    std::vector<double> recentPrices;

    for (int i = 0; i < daysToAnalyze; ++i) {
        recentPrices.push_back(historicalData[historicalData.size() - 1 - i].at("total_price_products"));
    }

    // Calcular la pendiente de la línea de tendencia
    double sumX = 0, sumY = 0, sumXY = 0, sumX2 = 0;
    for (int i = 0; i < daysToAnalyze; ++i) {
        sumX += i;
        sumY += recentPrices[i];
        sumXY += i * recentPrices[i];
        sumX2 += i * i;
    }

    double slope = (daysToAnalyze * sumXY - sumX * sumY) / (daysToAnalyze * sumX2 - sumX * sumX);
    
    // Normalizar la tendencia
    double maxTrend = *std::max_element(recentPrices.begin(), recentPrices.end());
    double minTrend = *std::min_element(recentPrices.begin(), recentPrices.end());
    double normalizedTrend = (slope - minTrend) / (maxTrend - minTrend);

    return normalizedTrend;
}

// 04-08-2024 1714 (v2). La estacionalidad usa un patrón anual simplificado.
double ABCMethod::calculateSeasonality(int day) {
    // Asumimos un ciclo anual de 365 días
    const int annualCycle = 365;
    
    // Factores de estacionalidad (ejemplo simplificado)
    std::vector<double> seasonalFactors = {
        0.9, 1.0, 1.1, 1.2, 1.1, 1.0,  // Primeros 6 meses
        0.9, 0.8, 0.9, 1.0, 1.1, 1.2   // Últimos 6 meses
    };

    int month = (day % annualCycle) / 30;  // Aproximación del mes
    return seasonalFactors[month] - 1.0;  // Normalizar alrededor de 0
}

// 04-08-2024 1714 (v2). Los factores externos son eventos discretos en días específicos.
double ABCMethod::getExternalFactor(int day) {
    // Factores externos simplificados (ejemplo)
    std::vector<std::pair<int, double>> externalEvents = {
        {30, 0.05},   // Evento positivo en el día 30
        {90, -0.03},  // Evento negativo en el día 90
        {180, 0.07},  // Evento muy positivo en el día 180
    };

    for (const auto& event : externalEvents) {
        if (day == event.first) {
            return event.second;
        }
    }

    return 0.0;  // Sin efecto externo en otros días
}

// 04-08-2024 1714 (v2). La autocorrelación se calcula utilizando el coeficiente de correlación de Pearson con un retraso de 1 día.
double ABCMethod::calculateAutocorrelation(double price, const std::vector<double>& previousPrices) {
    if (previousPrices.size() < 2) return 0.0;

    int lag = 1;  // Autocorrelación de primer orden
    int n = previousPrices.size();

    double mean = std::accumulate(previousPrices.begin(), previousPrices.end(), 0.0) / n;
    double variance = 0.0;
    double covariance = 0.0;

    for (int i = 0; i < n - lag; ++i) {
        double diff1 = previousPrices[i] - mean;
        double diff2 = previousPrices[i + lag] - mean;
        variance += diff1 * diff1;
        covariance += diff1 * diff2;
    }

    variance /= (n - lag);
    covariance /= (n - lag);

    double autocorrelation = (variance != 0) ? (covariance / variance) : 0.0;
    return autocorrelation;
}

// 04-08-2024 1714 (v3). La volatilidad se calcula como la desviación estándar de los retornos diarios, anualizada.
double ABCMethod::calculateVolatility(const std::vector<std::map<std::string, double>>& historicalData) {
    if (historicalData.size() < 2) return 0.0;

    std::vector<double> returns;
    for (size_t i = 1; i < historicalData.size(); ++i) {
        double prevPrice = historicalData[i-1].at("total_price_products");
        double currPrice = historicalData[i].at("total_price_products");
        double dailyReturn = (currPrice - prevPrice) / prevPrice;
        returns.push_back(dailyReturn);
    }

    double mean = std::accumulate(returns.begin(), returns.end(), 0.0) / returns.size();
    double sqSum = std::inner_product(returns.begin(), returns.end(), returns.begin(), 0.0);
    double stdev = std::sqrt(sqSum / returns.size() - mean * mean);

    // Anualizar la volatilidad (asumiendo 252 días de trading)
    double annualizedVolatility = stdev * std::sqrt(252);

    return annualizedVolatility;
}