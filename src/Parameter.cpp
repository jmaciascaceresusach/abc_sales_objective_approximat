/* Funcionalidad general 27032024
Implementa la clase "Parameter", proporcionando un constructor y un método
"adjustProbability" para ajustar la posibilidad del parámetro y asegurarse
de que permanezca en el rango [0,1].
*/

#include "../include/Parameter.h"

// Implementación del constructor
Parameter::Parameter(std::string name, double probability) : name(name), probability(probability) {}

// Método para ajustar la probabilidad.
void Parameter::adjustProbability(double adjustment) {
    this->probability += adjustment;
    // Asegúrese de que la probabilidad se mantenga dentro de los límites de 0 y 1
    if (this->probability < 0) this->probability = 0;
    if (this->probability > 1) this->probability = 1;
}
