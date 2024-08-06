#include "../include/Parameter.h"
#include <algorithm>

/*
algorithm: Biblioteca estándar de C++ que proporciona funciones para realizar operaciones algorítmicas, como std::max y std::min.
*/

/*
Comentarios generales:
- El archivo implementa la clase Parameter, que incluye un constructor y un método para ajustar la probabilidad del parámetro. La clase es sencilla y está 
diseñada para gestionar parámetros con una probabilidad que debe mantenerse dentro del rango [0, 1].
- Constructor: Inicializa el nombre y la probabilidad del parámetro.
- adjustProbability: Ajusta la probabilidad y garantiza que se mantenga dentro del rango válido.
Esta clase es útil en situaciones donde se necesita ajustar probabilidades de manera controlada, como en simulaciones o modelos probabilísticos. 
La función adjustProbability asegura que los valores de probabilidad no se salgan de los límites lógicos, evitando así posibles errores en cálculos posteriores.
*/

// name: Un std::string que representa el nombre del parámetro.
// probability: Un double que representa la probabilidad inicial del parámetro.
Parameter::Parameter(std::string name, double probability) : name(name), probability(probability) {}

// adjustment: Un double que representa el ajuste a aplicar a la probabilidad.
// this->probability += adjustment: Suma el ajuste a la probabilidad actual del parámetro.
// this->probability = std::max(0.0, std::min(1.0, this->probability)): Asegura que la probabilidad esté en el rango de 0 a 1. 
// Usa std::min para limitar la probabilidad máxima a 1 y std::max para limitar la probabilidad mínima a 0.
void Parameter::adjustProbability(double adjustment) {
    this->probability += adjustment;
    this->probability = std::max(0.0, std::min(1.0, this->probability));
}