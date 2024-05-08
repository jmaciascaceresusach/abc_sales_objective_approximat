/* Funcionalidad general 27032024
Implementa la lógica de refinamiento de parámetros en "ABCMethod". La función
"refineParameters" ajusta los parámetros basándose en un factor aleatorio pequeño
y luego selecciona el mejor conjunto de parámetros basándose en la distancia al
objetivo de ventas.

Problema encontrado (por solucionar): 
- El ajuste se realiza solo unas vez al principio,
basado en un factor aleatorio, sin considerar el resultado de las simulaciones posteriores
para realizar ajustes adicionales.
- Para el ABCMethod::refineParameters, ajustar los parámetros de manera más efectiva
en función de los resultados de cada simulación. Esto podría incluir técnicas de optimización
más avanzadas que simplemente ajustar los parámetros basándose en si están por encima o por
debajo del objetivo.
*/

#include "../include/ABCMethod.h"
#include <limits>
#include <random>
#include <cmath>

ABCMethod::ABCMethod() {}

void ABCMethod::refineParameters(std::vector<Parameter>& parameters, 
                                 std::function<double(const std::vector<Parameter>&)> calculateSale, 
                                 double salesObjective, 
                                 double tolerance) {
    std::default_random_engine generator(std::random_device{}());
    std::normal_distribution<double> distribution(0.0, 0.01); // Ajuste pequeño

    for (auto& param : parameters) {
        double adjustment = distribution(generator);
        param.adjustProbability(adjustment);
    }

    normalizeParameters(parameters);

    double closestDistance = std::numeric_limits<double>::max();
    std::vector<Parameter> bestParameters = parameters;

    int numberOfIterations;
    readConfigSimple("simulation_config.txt", numberOfIterations);

    for (int i = 0; i < numberOfIterations; ++i) {
        double saleValue = calculateSale(parameters);
        double distance = calculateDistance(saleValue, salesObjective);

        if (distance < closestDistance) {
            closestDistance = distance;
            bestParameters = parameters;
        }

        if (distance <= tolerance) {
            break;
        }

        dynamicAdjustParameters(parameters, saleValue, salesObjective);
        normalizeParameters(parameters);
    }

    parameters = bestParameters;
}

// Ajuste dinámicamente los parámetros según los resultados de la simulación.
void ABCMethod::dynamicAdjustParameters(std::vector<Parameter>& parameters, double saleValue, double salesObjective) {
    double errorMargin = calculateDistance(saleValue, salesObjective);
    for (auto& param : parameters) {
        double adjustment = (saleValue < salesObjective) ? 0.01 : -0.01;
        param.adjustProbability(adjustment * errorMargin);
    }
}

double ABCMethod::calculateDistance(double saleValue, double salesObjective) {
    return std::abs(saleValue - salesObjective);
}

void ABCMethod::normalizeParameters(std::vector<Parameter>& parameters) {
    double totalProbability = 0.0;
    for (const auto& param : parameters) {
        totalProbability += param.probability;
    }

    if (totalProbability > 1.0) {
        for (auto& param : parameters) {
            param.probability /= totalProbability;
        }
    }
}
