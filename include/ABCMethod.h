#ifndef ABCMETHOD_H
#define ABCMETHOD_H

#include "Parameter.h"
#include <vector>
#include <functional>
#include <fstream>
#include <sstream>
#include <cstdlib> // para std::atoi and std::atof

/**
 * Clase que implementa el método ABC para refinar parámetros.
 */
class ABCMethod {
public:
    // Constructor por defecto.
    ABCMethod();

    /**
     * Método para refinar parámetros usando el método ABC.
     * @param parameters Lista de parámetros a refinar.
     */
    void initializeParameters(std::vector<Parameter>& parameters);

    /**
     * Lee una configuración simple desde un archivo.
     * @param configFilePath Ruta al archivo de configuración.
     * @param numberOfIterations Variable donde se almacenará el número de iteraciones.
     */
    void readConfigSimple(const std::string& configFilePath, int& numberOfIterations);

    /**
     * Método para refinar parámetros usando el método ABC.
     * @param parameters Lista de parámetros a refinar.
     * @param calculateSale Función para calcular las ventas.
     * @param salesObjectiveFinal Objetivo de ventas a alcanzar.
     * @param tolerance Tolerancia aceptable entre las ventas calculadas y el objetivo.
     */
    void refineParameters(std::vector<Parameter>& parameters, std::function<double(const std::vector<Parameter>&)> calculateSale, double salesObjectiveFinal, double tolerance);

    /**
     * Ajusta dinámicamente los parámetros en función de las ventas simuladas.
     * @param parameters Lista de parámetros a ajustar.
     * @param saleValue Valor de las ventas simuladas.
     * @param salesObjectiveFinal Objetivo de ventas a alcanzar.
     */
    void dynamicAdjustParameters(std::vector<Parameter>& parameters, double saleValue, double salesObjectiveFinal);

    /**
     * Ajuste basado en gradiente.
     * @param parameters Lista de parámetros a ajustar.
     * @param saleValue Valor de las ventas simuladas.
     * @param salesObjectiveFinal Objetivo de ventas a alcanzar.
     */
    void dynamicAdjustParametersGradient(std::vector<Parameter>& parameters, double saleValue, double salesObjectiveFinal);

    /**
     * Ajuste basado en la media deslizante.
     * @param parameters Lista de parámetros a ajustar.
     * @param saleValue Valor de las ventas simuladas.
     * @param salesObjectiveFinal Objetivo de ventas a alcanzar.
     */
    void dynamicAdjustParametersSlidingAverage(std::vector<Parameter>& parameters, double saleValue, double salesObjectiveFinal);

    /**
     * Optimización basada en algoritmos evolutivos (genéticos).
     * @param parameters Lista de parámetros a ajustar.
     * @param calculateSale Función para calcular las ventas.
     * @param saleValue Valor de las ventas simuladas.
     * @param salesObjectiveFinal Objetivo de ventas a alcanzar.
     */
    void dynamicAdjustParametersGenetic(std::vector<Parameter>& parameters, std::function<double(const std::vector<Parameter>&)> calculateSale, double saleValue, double salesObjectiveFinal);

    /**
     * Optimización por recocido simulado.
     * @param parameters Lista de parámetros a ajustar.
     * @param calculateSale Función para calcular las ventas.
     * @param saleValue Valor de las ventas simuladas.
     * @param salesObjectiveFinal Objetivo de ventas a alcanzar.
     */
    void dynamicAdjustParametersSimulatedAnnealing(std::vector<Parameter>& parameters, std::function<double(const std::vector<Parameter>&)> calculateSale, double saleValue, double salesObjectiveFinal);

    /**
     * Optimización basada en cuadrados mínimos (Levenberg-Marquardt).
     * @param parameters Lista de parámetros a ajustar.
     * @param saleValue Valor de las ventas simuladas.
     * @param salesObjectiveFinal Objetivo de ventas a alcanzar.
     */
    void dynamicAdjustParametersLM(std::vector<Parameter>& parameters, double saleValue, double salesObjectiveFinal);

private:
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

};

#endif // ABCMETHOD_H
