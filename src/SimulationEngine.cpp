/* Funcionalidad general 27032024
Implementa el "SimulationEngine", incluyendo la lógica para añadir
parámetros, ejecutar simulaciones, y ajustar parámetros basándose en
los resultados de la simulación.

Problema encontrado (por solucionar): 
- Aunque se ajustan los parámetros después de cada simulación basándose 
en si el valor de venta está por encima o por debajo del objetivo, este 
ajuste es relativamente simple y puede no ser suficiente para alcanzar 
el objetivo de ventas dentro de la tolerancia específicada.
*/

#include "../include/SimulationEngine.h"
#include <iostream>
#include <algorithm> // para std::min_element

// Implementación del constructor
SimulationEngine::SimulationEngine() {}

// Agrega un parámetro a la simulación.
void SimulationEngine::addParameter(const Parameter& parameter) {
    this->parameters.push_back(parameter);
}

struct SimulationOutcome {
    double saleValue;
    std::vector<Parameter> parameters;
};

void SimulationEngine::runSimulations(int numberOfIterations, 
                                      std::function<double(const std::vector<Parameter>&)> calculateSale, 
                                      double salesObjective, 
                                      double tolerance) {
    std::vector<SimulationOutcome> outcomes;

    std::cout << "\n";
    std::cout << "***Start Simulation***\n";

    double saleValue;
    for (int i = 0; i < numberOfIterations; ++i) {
        saleValue = calculateSale(this->parameters);    
        std::cout << "Iteration: " << i << std::endl;    
        std::cout << "saleValue: " << saleValue << std::endl;
        outcomes.push_back({saleValue, this->parameters});

        // Ajustar los parámetros según los resultados.
        this->adjustParameters(saleValue, salesObjective); // Llamada recién agregada para ajustar parámetros
        std::cout << "saleValue (adjust): " << saleValue << std::endl; 
        std::cout << "\n";
    }

    std::cout << "Parameter: saleValue (before), Probability: " << saleValue << std::endl;

    std::cout << "***End Simulation***\n";

    // Analizar los resultados para encontrar el conjunto de parámetros más cercano al objetivo de ventas.
    auto bestOutcome = std::min_element(outcomes.begin(), outcomes.end(), 
        [salesObjective](const SimulationOutcome& a, const SimulationOutcome& b) {
            return std::abs(a.saleValue - salesObjective) < std::abs(b.saleValue - salesObjective);
        });

    if (bestOutcome != outcomes.end() && std::abs(bestOutcome->saleValue - salesObjective) <= tolerance) {
        // Actualizar parámetros con el mejor resultado encontrado
        this->parameters = bestOutcome->parameters;
        std::cout << "\n";
        std::cout << "***Refined Parameters - Result***\n";
        std::cout << "Optimal parameters found within tolerance.\n";
        std::cout << "Parameter: saleValue, Probability: " << bestOutcome->saleValue << std::endl;
        for (auto& param : this->parameters) {
            std::cout << "Parameter: " << param.name << ", Probability: " << param.probability << std::endl;
        }
        std::cout << "\n";
    } else {
        std::cout << "\n";
        std::cout << "***Result***\n";
        std::cout << "No optimal parameters were found within tolerance.\n";
        std::cout << "\n";
    }
}

void SimulationEngine::adjustParameters(double saleValue, double salesObjective) {
    // Ajuste simple: se aumentaron los parámetros si "saleValue" está por debajo del objetivo, disminuirá si está por encima
    double adjustmentFactor = (saleValue < salesObjective) ? 1.01 : 0.99; // Ajustar en un 1%
    for (auto& param : this->parameters) {
        param.probability *= adjustmentFactor; // Se asegura que la "probabilidad" sea el campo a ajustar
        std::cout << "Parameter: " << param.name << ", Probability: " << param.probability << std::endl;
        // Determinado por el valor de probabilidad en el rango [0, 1]
        param.probability = std::max(0.0, std::min(param.probability, 1.0));
    }
}
