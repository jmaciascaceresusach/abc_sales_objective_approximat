#ifndef PARAMETER_H
#define PARAMETER_H

#include <string>
#include <map>

/**
 * Clase que representa un parámetro utilizado en simulaciones.
 */
class Parameter {
public:
    std::string name; ///< Nombre del parámetro.
    double probability; ///< Probabilidad asociada al parámetro.

    /**
     * Constructor del parámetro.
     * @param name Nombre del parámetro.
     * @param probability Probabilidad inicial asociada al parámetro.
     */
    Parameter(std::string name, double probability);

    /**
     * Ajusta la probabilidad basada en simulaciones.
     * @param adjustment Cantidad de ajuste a realizar en la probabilidad.
     */
    void adjustProbability(double adjustment);
};

#endif // PARAMETER_H
