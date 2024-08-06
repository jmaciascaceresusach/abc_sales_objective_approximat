#define _USE_MATH_DEFINES

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
- Este archivo implementa métodos avanzados para refinar parámetros, simular precios futuros y calcular diversas métricas probabilísticas y estadísticas. 
El código utiliza técnicas estadísticas y probabilísticas para ajustar y evaluar parámetros, asegurando que los modelos sean precisos y útiles para la predicción de precios futuros.
*/

// Constructor por defecto para la clase ABCMethod.
ABCMethod::ABCMethod() {}

// Inicializa los parámetros ajustando su probabilidad a 0.5.
void ABCMethod::initializeParameters(std::vector<Parameter>& parameters) {
    for (auto& param : parameters) {
        param.adjustProbability(0.5);
    }
}

// 05-08-2024 1011
// Refinar los parámetros utilizando un proceso iterativo.
// Se definen varios componentes como random_device, mt19937, y uniform_real_distribution para generar números aleatorios.
void ABCMethod::refineParameters(std::vector<Parameter>& parameters, 
                                 const SKUData& skuData,
                                 const std::map<std::string, double>& normalizedFeatures,
                                 int daysToSimulate,
                                 double tolerance,
                                 int numberOfRefinements) {

    std::cout << "Entering refineParameters function" << std::endl;

    // La función ajusta los parámetros dinámicos utilizando una distribución normal.
    // Genera precios simulados y calcula la distancia entre los precios simulados y los datos reales.
    // Si la distancia está por debajo de un umbral de tolerancia, se aceptan los parámetros.
    // Normaliza los parámetros aceptados y ajusta la tolerancia si no se aceptaron parámetros.
    std::vector<std::vector<Parameter>> acceptedParameters;
    
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(0.0, 1.0);

    for (int i = 0; i < numberOfRefinements; ++i) {

        /*if (i % 10 == 0) { // Imprimir solo cada 10 iteraciones para reducir el output
            std::cout << "Refinement iteration " << i + 1 << " of " << numberOfRefinements << std::endl;
        }*/
        std::cout << "Refinement iteration " << i + 1 << " of " << numberOfRefinements << std::endl;

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

    std::cout << "Exiting refineParameters function" << std::endl;

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
// Establece los datos históricos para el objeto ABCMethod.
void ABCMethod::setHistoricalData(const std::vector<std::map<std::string, double>>& data) {
    this->historicalData = data;
}

// 04-08-2024 1714
// Simula los precios futuros de los productos.
// Utiliza la distribución discreta para seleccionar intervalos de precios y una distribución uniforme para generar precios dentro de esos intervalos.
std::vector<double> ABCMethod::simulateFuturePrices(const SKUData& skuData, 
                                                    const std::map<std::string, double>& normalizedFeatures,
                                                    int daysToSimulate,
                                                    double initialPrice) {
    std::stringstream log;

    std::cout << "Entering simulateFuturePrices function" << std::endl;                                                 

    // La función genera una serie de precios futuros basados en los parámetros del SKU y las características normalizadas.
    std::vector<double> futurePrices;
    futurePrices.push_back(initialPrice);
    previousPrices.clear();
    previousPrices.push_back(initialPrice);

    std::random_device rd;
    std::mt19937 gen(rd());

    for (int i = 1; i < daysToSimulate; ++i) {

        std::cout << "\n** Simulating day " << i << " of " << daysToSimulate << " **";

        std::vector<double> probabilities;
        double prob = 0.0;

        for (const auto& interval : skuData.listProducts) {
            double prob = calculateProbabilityNoLog((interval.first + interval.second) / 2, skuData, i);
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

    std::cout << "\nExiting simulateFuturePrices function" << std::endl;
    return futurePrices;
}

// Calcula la distancia entre los precios simulados y los datos reales utilizando la diferencia absoluta de las probabilidades.
double ABCMethod::calculateDistance(const std::vector<double>& simulatedPrices, 
                                    const SKUData& skuData,
                                    double initialPrice,
                                    int daysToSimulate) {

    std::cout << "\n-> Entering calculateDistance function" << std::endl;

    double distance = 0.0;
    for (int i = 0; i < daysToSimulate; ++i) {
        double expectedProbability = calculateProbability(simulatedPrices[i], skuData, i); // Se mostrará en el log de consola
        double actualProbability = 1.0 / skuData.listProducts.size(); // Asumiendo distribución uniforme
        distance += std::abs(expectedProbability - actualProbability);
    }

    std::cout << "\n-> Exiting calculateDistance function\n" << std::endl;
    return distance / daysToSimulate;
}

// Normaliza las probabilidades de los parámetros para que su suma sea igual a 1.
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

// 05-08-2024 1057. Análisis de sensibilidad: función de prueba que varíe los parámetros de entrada.
// Realiza un análisis de sensibilidad variando los parámetros de entrada y calculando las probabilidades para diferentes precios y días.
void ABCMethod::sensitivityAnalysis(const SKUData& skuData) {
    std::vector<double> testPrices = {1000, 2000, 3000, 4000, 5000};
    std::vector<int> testDays = {1, 30, 90, 180, 365};

    for (double price : testPrices) {
        for (int day : testDays) {
            double prob = calculateProbabilityNoLog(price, skuData, day);
            std::cout << "Price: " << price << ", Day: " << day << ", Probability: " << prob << std::endl;
        }
    }
}

// 05-08-2024 1057. 
// Verifica los datos de entrada, asegurándose de que no estén vacíos y que los precios históricos estén dentro de un rango razonable.
bool ABCMethod::verifyInputData(const SKUData& skuData) {
    if (historicalData.empty()) {
        std::cerr << "Error: Historical data is empty" << std::endl;
        return false;
    }

    if (skuData.listProducts.empty()) {
        std::cerr << "Error: SKU product list is empty" << std::endl;
        return false;
    }

    // Verifica que los precios históricos estén dentro de un rango razonable
    double minPrice = std::numeric_limits<double>::max();
    double maxPrice = std::numeric_limits<double>::lowest();
    for (const auto& data : historicalData) {
        if (data.count("total_price_products") > 0) {
            double price = data.at("total_price_products");
            minPrice = std::min(minPrice, price);
            maxPrice = std::max(maxPrice, price);
        }
    }

    if (minPrice <= 0 || maxPrice > 1e6) {  // Ajusta estos límites según tus datos
        std::cerr << "Warning: Suspicious price range in historical data: [" << minPrice << ", " << maxPrice << "]" << std::endl;
    }

    return true;
}

// 05-08-2024 1555
// Calcula la probabilidad de un precio dado para un SKU en un día específico. Ajusta la probabilidad basada en la posición en el intervalo, el día, la tendencia histórica, 
// la estacionalidad, factores externos, autocorrelación y volatilidad.
double ABCMethod::calculateProbability(double price, const SKUData& skuData, int day) {
    double probability = 0.0;
    std::stringstream log;

    std::cout << "\n** Log calculateProbability **" << std::endl;

    // Verificar si el precio está en algún intervalo
    for (size_t i = 0; i < skuData.listProducts.size(); ++i) {
        const auto& interval = skuData.listProducts[i];
        if (price >= interval.first && price <= interval.second) {
            // Probabilidad base
            probability = 1.0 / skuData.listProducts.size();

            // Ajustar por la posición en el intervalo
            double positionInInterval = (price - interval.first) / (interval.second - interval.first);
            double intervalAdjustment = std::exp(-std::pow(positionInInterval - 0.5, 2) / 0.25);
            probability *= intervalAdjustment;
            
            log << "-> Day: " << day++ << ", Price: " << price << std::endl;            
            log << "-> Base probability: " << probability << std::endl;

            log << "-> Interval adjustment: " << intervalAdjustment << std::endl;

            // Ajustar por el día (decae con el tiempo)
            double dayAdjustment = std::exp(-0.05 * day);
            probability *= dayAdjustment;
            log << "-> Day adjustment: " << dayAdjustment << std::endl;

            // Tendencia histórica: Multiplicamos la probabilidad por (1 + historicalTrend). Esto aumenta la probabilidad si hay una tendencia positiva y la disminuye si es negativa. 
            double historicalTrend = calculateHistoricalTrend(price, day, historicalData);
            probability *= (1 + historicalTrend);
            log << "-> Historical trend adjustment: " << (1 + historicalTrend) << std::endl;

            // Estacionalidad: Similar a la tendencia, multiplicamos por (1 + seasonality). Esto ajusta la probabilidad basándose en patrones estacionales.
            double seasonality = calculateSeasonality(day);
            probability *= (1 + seasonality);
            log << "-> Seasonality adjustment: " << (1 + seasonality) << std::endl;

            // Factores externos: Multiplicamos por (1 + externalFactor) para incorporar eventos externos que puedan afectar el precio.
            double externalFactor = getExternalFactor(day);
            probability *= (1 + externalFactor);
            log << "-> externalFactor adjustment: " << (1 + externalFactor) << std::endl;

            // Autocorrelación: Multiplicamos por (1 + autocorrelation). Una autocorrelación positiva aumentará la probabilidad de que el precio se mueva en la misma dirección que los precios recientes.
            double autocorrelation = calculateAutocorrelation(price, previousPrices);
            probability *= (1 + autocorrelation);
            log << "-> autocorrelation adjustment: " << (1 + autocorrelation) << std::endl;

            // Volatilidad: Usamos una función exponencial para ajustar la probabilidad basada en la volatilidad y la diferencia entre el precio actual y el anterior. Una alta volatilidad y una gran diferencia de precio reducirán la probabilidad.
            double volatility = calculateVolatility(historicalData);
            double previousPrice = previousPrices.empty() ? price : previousPrices.back();
            probability *= std::exp(-volatility * std::abs(price - previousPrice));

            // 05-08-2024 1033
            log << "-> volatility adjustment: " << volatility << " and previousPrice adjustment: " << previousPrice << std::endl;

            break;
        }
    }

    // Normalización: Al final, aseguramos que la probabilidad esté en el rango [0, 1].
    probability = std::max(0.0, std::min(1.0, probability));

    // Agregar el log al archivo de salida y a la consola
    std::cout << log.str();

    return probability;
}

// 05-08-2024 1533
// Similar al método anterior, pero sin logs adicionales.
double ABCMethod::calculateProbabilityNoLog(double price, const SKUData& skuData, int day) {
    double probability = 0.0;
    std::stringstream log;

    // Verificar si el precio está en algún intervalo
    for (size_t i = 0; i < skuData.listProducts.size(); ++i) {
        const auto& interval = skuData.listProducts[i];
        if (price >= interval.first && price <= interval.second) {
            // Probabilidad base
            probability = 1.0 / skuData.listProducts.size();

            // Ajustar por la posición en el intervalo
            double positionInInterval = (price - interval.first) / (interval.second - interval.first);
            double intervalAdjustment = std::exp(-std::pow(positionInInterval - 0.5, 2) / 0.25);
            probability *= intervalAdjustment;

            // Ajustar por el día (decae con el tiempo)
            double dayAdjustment = std::exp(-0.05 * day);
            probability *= dayAdjustment;

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

    // Agregar el log al archivo de salida y a la consola
    std::cout << log.str();

    return probability;
}

// 05-08-2024 1050. La tendencia histórica se calcula usando una regresión lineal simple sobre los últimos 30 días de datos.
// Calcula la tendencia histórica utilizando una regresión lineal simple sobre los últimos 30 días de datos.
double ABCMethod::calculateHistoricalTrend(double price, int day, const std::vector<std::map<std::string, double>>& historicalData) {
    if (historicalData.empty()) return 0.0;

    int daysToAnalyze = std::min(30, static_cast<int>(historicalData.size()));
    std::vector<double> recentPrices;

    for (int i = 0; i < daysToAnalyze; ++i) {
        if (historicalData[historicalData.size() - 1 - i].count("total_price_products") > 0) {
            recentPrices.push_back(historicalData[historicalData.size() - 1 - i].at("total_price_products"));
        }
    }

    if (recentPrices.size() < 2) return 0.0;

    // Calcular la pendiente de la línea de tendencia
    double sumX = 0, sumY = 0, sumXY = 0, sumX2 = 0;
    for (size_t i = 0; i < recentPrices.size(); ++i) {
        sumX += i;
        sumY += recentPrices[i];
        sumXY += i * recentPrices[i];
        sumX2 += i * i;
    }

    double n = recentPrices.size();
    double slope = (n * sumXY - sumX * sumY) / (n * sumX2 - sumX * sumX);
    
    // Normalizar la tendencia
    double maxPrice = *std::max_element(recentPrices.begin(), recentPrices.end());
    double minPrice = *std::min_element(recentPrices.begin(), recentPrices.end());
    double normalizedTrend = (slope - minPrice) / (maxPrice - minPrice);

    return normalizedTrend;
}

// 05-08-2024 1051. La estacionalidad usa un patrón anual simplificado.
// Calcula la estacionalidad utilizando un patrón anual simplificado.
double ABCMethod::calculateSeasonality(int day) {
    // Asumimos un ciclo anual de 365 días
    const int annualCycle = 365;
    int dayOfYear = day % annualCycle;
    
    // Definimos 4 estaciones
    if (dayOfYear < 90) { // Primavera
        return 0.05 * std::sin((M_PI * dayOfYear) / 180);
    } else if (dayOfYear < 180) { // Verano
        return 0.1 * std::sin((M_PI * (dayOfYear - 90)) / 180);
    } else if (dayOfYear < 270) { // Otoño
        return -0.05 * std::sin((M_PI * (dayOfYear - 180)) / 180);
    } else { // Invierno
        return -0.1 * std::sin((M_PI * (dayOfYear - 270)) / 180);
    }
}

// 05-08-2024 1053. Los factores externos son eventos discretos en días específicos.
// Calcula factores externos basados en eventos específicos en días particulares.
double ABCMethod::getExternalFactor(int day) {
    // Simulamos eventos aleatorios
    std::vector<std::pair<int, double>> events = {
        {30, 0.05},   // Evento positivo en el día 30
        {90, -0.03},  // Evento negativo en el día 90
        {180, 0.07},  // Evento muy positivo en el día 180
        {270, -0.05}  // Evento muy negativo en el día 270
    };

    for (const auto& event : events) {
        if (day % 365 == event.first) {
            return event.second;
        }
    }

    // Pequeña variación aleatoria para otros días
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(-0.01, 0.01);
    return dis(gen);
}

// 05-08-2024 1053. La autocorrelación se calcula utilizando el coeficiente de correlación de Pearson con un retraso de 1 día.
// Calcula la autocorrelación utilizando el coeficiente de correlación de Pearson con un retraso de 1 día.
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

// 05-08-2024 1053. La volatilidad se calcula como la desviación estándar de los retornos diarios, anualizada.
// Calcula la volatilidad como la desviación estándar de los retornos diarios, anualizada.
double ABCMethod::calculateVolatility(const std::vector<std::map<std::string, double>>& historicalData) {
    if (historicalData.size() < 2) return 0.0;

    std::vector<double> returns;
    for (size_t i = 1; i < historicalData.size(); ++i) {
        if (historicalData[i-1].count("total_price_products") > 0 && historicalData[i].count("total_price_products") > 0) {
            double prevPrice = historicalData[i-1].at("total_price_products");
            double currPrice = historicalData[i].at("total_price_products");
            double dailyReturn = (currPrice - prevPrice) / prevPrice;
            returns.push_back(dailyReturn);
        }
    }

    if (returns.empty()) return 0.0;

    double mean = std::accumulate(returns.begin(), returns.end(), 0.0) / returns.size();
    double sqSum = std::inner_product(returns.begin(), returns.end(), returns.begin(), 0.0);
    double stdev = std::sqrt(sqSum / returns.size() - mean * mean);

    // Anualizar la volatilidad (asumiendo 252 días de trading)
    double annualizedVolatility = stdev * std::sqrt(252);

    return annualizedVolatility;
}