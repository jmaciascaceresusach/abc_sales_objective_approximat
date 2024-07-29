#ifndef PARAMETER_H
#define PARAMETER_H

#include <string>

/*
Comentarios generales:
- Este archivo define la clase Parameter, que representa un parámetro del modelo.

Comentario específicos:
- Parameter: Clase que encapsula un nombre y una probabilidad asociada a un parámetro.
- adjustProbability: Método para ajustar la probabilidad del parámetro.
*/

class Parameter {
public:
    std::string name;
    double probability;

    Parameter(std::string name, double probability);
    void adjustProbability(double adjustment);
};

#endif // PARAMETER_H