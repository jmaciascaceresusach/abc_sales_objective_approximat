/* Funcionalidad general 27032024
Define la clase "SimulationEngine", que es el motor de la simulación. Puede
agregar parámetros, ejecutar simulaciones usando estos parámetros, y ajustar
los parámetros utilizando un objeto "ABCMethod".
*/

#ifndef SIMULATIONENGINE_H
#define SIMULATIONENGINE_H

#include "Parameter.h"
#include "ABCMethod.h"
#include <vector>
#include <functional>

class SimulationEngine {
public:
    // Vector para almacenar los parámetros involucrados en la simulación.
    std::vector<Parameter> parameters;

    // Constructor
    SimulationEngine();

    // Método para agregar un parámetro a la simulación.
    void addParameter(const Parameter& parameter);

    // Ejecuta simulaciones para aproximarse al objetivo de ventas.
    void runSimulations(int numberOfIterations, std::function<double(const std::vector<Parameter>&)> calculateSale, double salesObjective, double tolerance);

    // Método para ajustar parámetros en función de los resultados de la simulación.
    void adjustParameters(double saleValue, double salesObjective);
    
private:
    ABCMethod abcMethod;
};

#endif // SIMULATIONENGINE_H
