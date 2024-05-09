#ifndef SIMULATIONENGINE_H
#define SIMULATIONENGINE_H

#include "Parameter.h"
#include "ABCMethod.h"
#include <vector>
#include <functional>

/**
 * Clase que actúa como motor de simulación para ajustar parámetros usando el método ABC.
 */
class SimulationEngine {
public:
    std::vector<Parameter> parameters; ///< Vector para almacenar los parámetros involucrados en la simulación.

    // Constructor
    SimulationEngine();

    /**
     * Agrega un parámetro a la simulación.
     * @param parameter Parámetro a agregar.
     */
    void addParameter(const Parameter& parameter);

    /**
     * Ejecuta simulaciones para aproximarse al objetivo de ventas.
     * @param numberOfIterations Número de iteraciones a ejecutar.
     * @param calculateSale Función para calcular las ventas.
     * @param salesObjective Objetivo de ventas a alcanzar.
     * @param tolerance Tolerancia aceptable entre las ventas calculadas y el objetivo.
     */
    void runSimulations(int numberOfIterations, std::function<double(const std::vector<Parameter>&)> calculateSale, double salesObjective, double tolerance);

    /**
     * Ajusta parámetros en función de los resultados de la simulación.
     * @param saleValue Valor de las ventas simuladas.
     * @param salesObjective Objetivo de ventas a alcanzar.
     */
    void adjustParameters(double saleValue, double salesObjective);
    
private:
    ABCMethod abcMethod; ///< Instancia del método ABC.
};

#endif // SIMULATIONENGINE_H
