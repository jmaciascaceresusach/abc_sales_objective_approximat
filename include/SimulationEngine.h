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
     * @param salesObjectiveFinal Objetivo de ventas a alcanzar.
     * @param tolerance Tolerancia aceptable entre las ventas calculadas y el objetivo.
     */
    void runSimulations(int numberOfIterations, std::function<double(const std::vector<Parameter>&)> calculateSale, double salesObjectiveFinal, double tolerance);

    /**
     * Ajusta parámetros en función de los resultados de la simulación.
     * @param saleValue Valor de las ventas simuladas.
     * @param salesObjectiveFinal Objetivo de ventas a alcanzar.
     */
    void adjustParameters(double saleValue, double salesObjectiveFinal);

    /**
     * Método para refinar parámetros usando el método ABC.
     * @param parameters Lista de parámetros a refinar.
     * @param calculateSale Función para calcular las ventas.
     * @param salesObjectiveFinal Objetivo de ventas a alcanzar.
     * @param tolerance Tolerancia aceptable entre las ventas calculadas y el objetivo.
     */
    void refineParameters(std::vector<Parameter>& parameters, std::function<double(const std::vector<Parameter>&)> calculateSale, double salesObjectiveFinal, double tolerance);

    
private:
    ABCMethod abcMethod; ///< Instancia del método ABC.

    /**
     * Método auxiliar para calcular la distancia entre las ventas simuladas y el objetivo de ventas.
     * @param saleValue Valor de las ventas simuladas.
     * @param salesObjectiveFinal Objetivo de ventas a alcanzar.
     * @return Distancia entre las ventas simuladas y el objetivo.
     */
    double calculateDistance(double saleValue, double salesObjectiveFinal);

    /**
     * Normaliza los parámetros para que sus probabilidades sumen 1 o menos.
     * @param parameters Lista de parámetros a normalizar.
     */
    void normalizeParameters(std::vector<Parameter>& parameters);

    /**
     * Monitorea y evalúa el rendimiento de la simulación.
     * @param parameters Lista de parámetros ajustados.
     * @param saleValue Valor de las ventas simuladas.
     * @param salesObjectiveFinal Objetivo de ventas a alcanzar.
     */
    void monitorAndEvaluate(const std::vector<Parameter>& parameters, double saleValue, double salesObjectiveFinal);
};

#endif // SIMULATIONENGINE_H
