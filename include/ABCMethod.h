#ifndef ABCMETHOD_H
#define ABCMETHOD_H

// La protección del archivo de cabecera evita que el contenido se incluya más de una vez en una única compilación, lo cual puede causar errores de redefinición.

#include <vector>
#include <string>
#include <functional>
#include <map>
#include "Parameter.h"

/*
vector: Utilizado para manejar listas de elementos.
string: Utilizado para manejar cadenas de texto.
functional: No se usa en el código proporcionado, pero podría estar ahí para permitir el uso de funciones y lambdas.
map: Utilizado para manejar asociaciones clave-valor.
Parameter.h: Define la clase Parameter, que probablemente representa parámetros del modelo ABC.
*/

/*
Comentarios generales:
- El archivo ABCMETHOD_H define la clase ABCMethod, que implementa el método Approximate Bayesian Computation (ABC) para la simulación de precios futuros. 
La clase maneja varias tareas relacionadas con la simulación, como la inicialización y refinamiento de parámetros, la simulación de precios futuros y el cálculo 
de diversas métricas estadísticas y probabilísticas.
- Estructuras: Define PriceInterval y SKUData para manejar datos específicos del SKU y los intervalos de precios.
- Métodos Públicos: Permiten inicializar parámetros, refinar parámetros, simular precios futuros, calcular distancias, establecer datos históricos y realizar análisis 
de sensibilidad.
- Métodos Privados: Incluyen cálculos específicos como la normalización de parámetros, probabilidades, tendencias históricas, estacionalidad, factores externos, 
autocorrelación y volatilidad.

La clase ABCMethod es una parte crucial del sistema de simulación, proporcionando las herramientas necesarias para predecir precios futuros basados en datos históricos 
y características específicas del producto.
*/

// minPrice: Precio mínimo del intervalo.
// maxPrice: Precio máximo del intervalo.
// count: Número de elementos en el intervalo.
struct PriceInterval {
    double minPrice;
    double maxPrice;
    int count;
};

// sku: Identificador del SKU.
// intervals: Lista de intervalos de precios.
// globalMinPrice: Precio mínimo global del SKU.
// globalMaxPrice: Precio máximo global del SKU.
// listProducts: Lista de productos con pares de precios mínimos y máximos.
struct SKUData {
    std::string sku;
    std::vector<PriceInterval> intervals;
    double globalMinPrice;
    double globalMaxPrice;
    std::vector<std::pair<double, double>> listProducts;
};

class ABCMethod {
public:

    // Constructor de la clase.
    ABCMethod();

    // Inicializa los parámetros del modelo.
    void initializeParameters(std::vector<Parameter>& parameters);

    // 05-08-2024 1025
    // Refina los parámetros del modelo en base a los datos del SKU y características normalizadas.
    void refineParameters(std::vector<Parameter>& parameters, 
                        const SKUData& skuData,
                        const std::map<std::string, double>& normalizedFeatures,
                        int daysToSimulate,
                        double tolerance,
                        int numberOfRefinements);
                          
    // Simula los precios futuros del SKU.
    std::vector<double> simulateFuturePrices(const SKUData& skuData, 
                                             const std::map<std::string, double>& normalizedFeatures,
                                             int daysToSimulate,
                                             double initialPrice);

    // Calcula la distancia entre los precios simulados y los datos históricos.
    double calculateDistance(const std::vector<double>& simulatedPrices, 
                             const SKUData& skuData,
                             double initialPrice,
                             int daysToSimulate);

    // Establece los datos históricos para el modelo.
    void setHistoricalData(const std::vector<std::map<std::string, double>>& data);

    // Realiza un análisis de sensibilidad en los datos del SKU.
    void sensitivityAnalysis(const SKUData& skuData); // 05-08-2024 1105

    // Verifica la validez de los datos de entrada del SKU.
    bool verifyInputData(const SKUData& skuData); // 05-08-2024 1105

private:

    // Normaliza los parámetros del modelo.
    void normalizeParameters(std::vector<Parameter>& parameters);
    
    // Calcula la probabilidad de un precio dado en un día específico.
    double calculateProbability(double price, const SKUData& skuData, int day);

    // Calcula la probabilidad de un precio dado en un día específico sin logs adicionales.
    double calculateProbabilityNoLog(double price, const SKUData& skuData, int day); // 05-08-2024 1532

    // Calcula la tendencia histórica de los precios.
    double calculateHistoricalTrend(double price, int day, const std::vector<std::map<std::string, double>>& historicalData); // 04-08-2024 1714

    // Calcula el efecto de la estacionalidad en los precios.
    double calculateSeasonality(int day); // 04-08-2024 1714

    // Obtiene factores externos que puedan afectar los precios.
    double getExternalFactor(int day); // 04-08-2024 1714

    // Calcula la autocorrelación de los precios.
    double calculateAutocorrelation(double price, const std::vector<double>& previousPrices); // 04-08-2024 1714

    // Calcula la volatilidad de los precios basándose en los datos históricos.
    double calculateVolatility(const std::vector<std::map<std::string, double>>& historicalData); // 04-08-2024 1714

    // Almacena los precios previos simulados.
    std::vector<double> previousPrices; // 04-08-2024 1714

    // Almacena los datos históricos utilizados para las simulaciones.
    std::vector<std::map<std::string, double>> historicalData; // 04-08-2024 1714
};

#endif // ABCMETHOD_H