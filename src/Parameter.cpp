#include "../include/Parameter.h"

/**
 * Implementación del constructor del parámetro.
 * @param name Nombre del parámetro.
 * @param probability Probabilidad inicial asociada al parámetro.
 */
Parameter::Parameter(std::string name, double probability) : name(name), probability(probability) {}

/**
 * Método para ajustar la probabilidad.
 * @param adjustment Cantidad de ajuste a realizar en la probabilidad.
 */
void Parameter::adjustProbability(double adjustment) {
    this->probability += adjustment;
    // Asegúrese de que la probabilidad se mantenga dentro de los límites de 0 y 1.
    if (this->probability < 0) this->probability = 0;
    if (this->probability > 1) this->probability = 1;
}
