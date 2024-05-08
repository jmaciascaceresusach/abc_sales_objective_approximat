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
#include <cstdlib> // para std::atoi and std::atof
#include <fstream>  
#include <sstream>
#include <chrono>
#include <thread>

// Implementación del constructor
SimulationEngine::SimulationEngine() {}

// Agrega un parámetro a la simulación.
void SimulationEngine::addParameter(const Parameter& parameter) {
    this->parameters.push_back(parameter);
}

/* Versión antigua 05052024
struct SimulationOutcome {
    double saleValue;
    std::vector<Parameter> parameters;
};
*/

// Implementación (08052024)
void SimulationEngine::runSimulations(int numberOfIterations, std::function<double(const std::vector<Parameter>&)> calculateSale, double salesObjective, double tolerance) {
    std::ofstream statsFile("statistics_simulations.txt");
    statsFile << "Iteration,SaleValue,Distance,Objective,Tolerance";
    for (const auto& param : parameters) {
        statsFile << "," << param.name;
    }
    statsFile << "\n";

    double closestDistance = std::numeric_limits<double>::max();
    std::vector<Parameter> bestParameters = parameters;
    double bestSaleValue = 0;
    int bestIteration = -1;

    std::cout << "\n**Start Simulation***\n";
    std::cout << "\n";

    for (int i = 0; i < numberOfIterations; ++i) {
        double saleValue = calculateSale(parameters);
        double distance = std::abs(saleValue - salesObjective);

        std::cout << "Iteration: " << i << " - saleValue: " << saleValue << " - distance: " << distance << std::endl;

        statsFile << i << "," << saleValue << "," << distance << "," << salesObjective << "," << tolerance;
        for (const auto& param : parameters) {
            statsFile << "," << param.probability;
        }
        statsFile << "\n";

        if (distance < closestDistance) {
            closestDistance = distance;
            bestParameters = parameters;
            bestSaleValue = saleValue;
            bestIteration = i;

            if (distance <= tolerance) break;
        }

        adjustParameters(saleValue, salesObjective);
    }

    statsFile.close();

    std::cout << "\n***End Simulation***\n";

    // Mostrar detalles de los mejores parámetros
    std::cout << "\n***Results***\n";
    std::cout << "Best parameters found at iteration " << bestIteration << " with sale value " << bestSaleValue << std::endl;
    std::cout << "\n***Best Parameters***\n";
    for (const auto& param : bestParameters) {
        std::cout << "Parameter: " << param.name << ", Probability: " << param.probability << std::endl;
    }
}

/*void SimulationEngine::runSimulations(int numberOfIterations, 
                                      std::function<double(const std::vector<Parameter>&)> calculateSale, 
                                      double salesObjective, 
                                      double tolerance) {
    std::vector<SimulationOutcome> outcomes;

    std::cout << "\n";
    std::cout << "***Start Simulation***\n";
    std::cout << "\n";

    double refinedSaleValue;
    for (int i = 0; i < numberOfIterations; ++i) {
        double saleValue = calculateSale(this->parameters);    
        std::cout << "Iteration: " << i << " - saleValue: " << saleValue << std::endl;
        
        // Almacenar el resultado antes de cualquier ajuste
        outcomes.push_back({saleValue, this->parameters});

        // Ajustar los parámetros según los resultados.
        this->adjustParameters(saleValue, salesObjective);
        std::cout << "\n";
    }

    std::cout << "***End Simulation***\n";

    // Analizar los resultados para encontrar el conjunto de parámetros más cercano al objetivo de ventas.
    auto bestOutcome = std::min_element(outcomes.begin(), outcomes.end(), 
        [salesObjective](const SimulationOutcome& a, const SimulationOutcome& b) {
            return std::abs(a.saleValue - salesObjective) < std::abs(b.saleValue - salesObjective);
        });

    if (bestOutcome != outcomes.end()) {
        std::cout << "\n";
        std::cout << "***Refined Parameters - Result***\n";
        if (std::abs(bestOutcome->saleValue - salesObjective) <= tolerance) {
            // Actualizar parámetros con el mejor resultado encontrado
            this->parameters = bestOutcome->parameters;
            std::cout << "Optimal parameters found within tolerance.\n";
            std::cout << "\n";
            std::cout << "Best saleValue: " << bestOutcome->saleValue << std::endl;  // Mostrar el mejor saleValue correctamente
            for (const auto& param : this->parameters) {
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
}*/

// Implementación (08052024)
void SimulationEngine::adjustParameters(double saleValue, double salesObjective) {
    double error = saleValue - salesObjective;
    double learningRate = 0.01;

    for (auto& param : parameters) {
        double adjustment = (error > 0 ? -1 : 1) * learningRate * std::abs(param.probability);
        param.adjustProbability(adjustment);

        param.probability = std::max(0.0, std::min(param.probability, 1.0));
        std::cout << "Parameter: " << param.name << ", Probability: " << param.probability << std::endl;
    }
}


/* Implementación antigua 05052024
void SimulationEngine::adjustParameters(double saleValue, double salesObjective) {
    double error = saleValue - salesObjective;
    double learningRate = 0.01; // Tasa de aprendizaje para ajustes

    for (auto& param : this->parameters) {  // Aquí 'parameters' se usa directamente.
        // Ajuste proporcional al error y a la contribución del parámetro
        double adjustment = (error > 0 ? -1 : 1) * learningRate * std::abs(param.probability);
        param.probability += adjustment;

        // Asegurarse de que la probabilidad permanezca dentro de los límites [0, 1]
        param.probability = std::max(0.0, std::min(param.probability, 1.0));

        std::cout << "Parameter: " << param.name << ", Probability: " << param.probability << std::endl;
    }
}*/

/*void SimulationEngine::adjustParameters(double saleValue, double salesObjective) {
    // Ajuste simple: se aumentaron los parámetros si "saleValue" está por debajo del objetivo, disminuirá si está por encima
    double adjustmentFactor = (saleValue < salesObjective) ? 1.01 : 0.99; // Ajustar en un 1%
    for (auto& param : this->parameters) {
        param.probability *= adjustmentFactor; // Se asegura que la "probabilidad" sea el campo a ajustar
        std::cout << "Parameter: " << param.name << ", Probability: " << param.probability << std::endl;
        // Determinado por el valor de probabilidad en el rango [0, 1]
        param.probability = std::max(0.0, std::min(param.probability, 1.0));
    }
}*/
