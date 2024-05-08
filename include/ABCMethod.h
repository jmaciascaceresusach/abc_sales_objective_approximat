/* Funcionalidad general 27032024
Se encapsula la lógica de parámetros de simulación con el objetivo 
de alcanzar un objetivo de ventas específico. Se proporciona un método
"refineParameters" que ajusta los parámetros basándose en la distancia
entre los valores de ventas simulados y el objetivo de ventas.
*/

#ifndef ABCMETHOD_H
#define ABCMETHOD_H

#include "Parameter.h"
#include <vector>
#include <functional>
#include <fstream>
#include <sstream>
#include <cstdlib> // para std::atoi and std::atof

class ABCMethod {
public:
    // Constructor
    ABCMethod();

    // Método para refinar parámetros usando el método ABC.
    void refineParameters(std::vector<Parameter>& parameters, std::function<double(const std::vector<Parameter>&)> calculateSale, double salesObjective, double tolerance);
    void dynamicAdjustParameters(std::vector<Parameter>& parameters, double saleValue, double salesObjective);
    void dynamicAdjustParametersGradient(std::vector<Parameter>& parameters, double saleValue, double salesObjective);
    void dynamicAdjustParametersSlidingAverage(std::vector<Parameter>& parameters, double saleValue, double salesObjective);
    void dynamicAdjustParametersGenetic(std::vector<Parameter>& parameters, double saleValue, double salesObjective);
    void dynamicAdjustParametersSimulatedAnnealing(std::vector<Parameter>& parameters, double saleValue, double salesObjective);
    void dynamicAdjustParametersLM(std::vector<Parameter>& parameters, double saleValue, double salesObjective);

private:
    // Método auxiliar para calcular la distancia entre las ventas simuladas y el objetivo de ventas.
    double calculateDistance(double saleValue, double salesObjective);
    void normalizeParameters(std::vector<Parameter>& parameters); // Implementación 08052024
};

void readConfigSimple(const std::string& configFilePath, int& numberOfIterations) {
    std::ifstream configFile(configFilePath);
    std::string line;

    if (configFile.is_open()) {
        while (getline(configFile, line)) {
            std::istringstream iss(line);
            std::string key;
            if (getline(iss, key, '=')) {
                std::string value;
                if (getline(iss, value)) {
                    if (key == "numberOfIterations") numberOfIterations = std::stoi(value);
                }
            }
        }
    }
}

#endif // ABCMETHOD_H