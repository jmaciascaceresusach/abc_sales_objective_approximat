#include "../include/Parameter.h"
#include <algorithm>

/*
Comentarios generales:
- Este archivo implementa la clase Parameter.

Comentario específicos:
- Implementación del constructor y el método adjustProbability.
*/

Parameter::Parameter(std::string name, double probability) : name(name), probability(probability) {}

void Parameter::adjustProbability(double adjustment) {
    this->probability += adjustment;
    this->probability = std::max(0.0, std::min(1.0, this->probability));
}