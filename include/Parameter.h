#ifndef PARAMETER_H
#define PARAMETER_H

// La protección del archivo de cabecera evita que el contenido se incluya más de una vez en una única compilación, lo cual puede causar errores de redefinición.

#include <string>

/*
string: Utilizado para manejar cadenas de texto, necesario para el nombre del parámetro.
*/

/*
Comentarios generales:
- El archivo Parameter.h define la clase Parameter, que encapsula un nombre y una probabilidad asociada a un parámetro. La clase proporciona un constructor para 
inicializar estos valores y un método para ajustar la probabilidad asegurando que se mantenga en un rango válido (0 a 1).

Funcionalidades Principales
Atributos:  
- name: Nombre del parámetro
- probability: Probabilidad asociada al parámetro.

Métodos: 
- Constructor: Inicializa los atributos name y probability.
- adjustProbability: Ajusta la probabilidad y la restringe al rango [0, 1].

Esta clase es útil en contextos donde se necesita manejar parámetros con probabilidades asociadas, como en modelos de simulación o computación probabilística. 
La clase garantiza que las probabilidades se mantengan dentro de límites lógicos, evitando errores en cálculos posteriores.
*/

class Parameter {
public:

    // Almacena el nombre del parámetro.
    std::string name;

    // Almacena la probabilidad asociada al parámetro.
    double probability;

    // Constructor: Inicializa un objeto Parameter con un nombre y una probabilidad.
    Parameter(std::string name, double probability);

    // Método para ajustar la probabilidad: Ajusta la probabilidad del parámetro.
    void adjustProbability(double adjustment);
};

// Esta línea cierra la protección del archivo de cabecera.
#endif // PARAMETER_H