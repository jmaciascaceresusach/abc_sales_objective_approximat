/* Funcionalidad general 27032024
Representa un parámetro individual en la simulación. Cada parámetro
tiene un nombre y una probabilidad (o valor) que puede ajustarse.
*/

#ifndef PARAMETER_H
#define PARAMETER_H

#include <string>
#include <map>

class Parameter {
public:
    std::string name;
    double probability;

    // Declaración del constructor
    Parameter(std::string name, double probability);

    // Método para ajustar la probabilidad basado en simulaciones.
    void adjustProbability(double adjustment);
};

#endif // PARAMETER_H
