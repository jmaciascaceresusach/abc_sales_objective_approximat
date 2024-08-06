#ifndef SIMULATION_H
#define SIMULATION_H

// La protección del archivo de cabecera evita que el contenido se incluya más de una vez en una única compilación, lo cual puede causar errores de redefinición.

#include "SimulationEngine.h"
#include <string>

/*
SimulationEngine.h: Incluye la definición de la clase SimulationEngine, necesaria para la función runSimulationForSKU.
string: Biblioteca estándar de C++ utilizada para manejar cadenas de texto.
*/

/*
- Comentarios generales:
El archivo Simulation.h declara la función runSimulationForSKU, que es responsable de ejecutar una simulación para un SKU específico utilizando un objeto SimulationEngine. 
A continuación, se describen los componentes principales del archivo:

Inclusiones
- SimulationEngine.h: Incluye la definición de la clase SimulationEngine, que es crucial para la ejecución de la simulación.
- string: Biblioteca estándar de C++ utilizada para manejar cadenas de texto, lo cual es necesario para representar el SKU y el día para simular.

Declaración de la Función
- runSimulationForSKU: Esta función toma varios parámetros necesarios para configurar y ejecutar la simulación:
- simulationEngine: Referencia a un objeto de SimulationEngine que se encargará de realizar la simulación.
- sku: Cadena que identifica el SKU del producto a simular.
- dayForSimulate: Cadena que especifica el día para el cual se desea realizar la simulación.
- numberOfIterations: Número de iteraciones que se realizarán durante la simulación.
- daysToSimulate: Número de días que se simularán.
- tolerance: Tolerancia utilizada para determinar la precisión de la simulación.
*/

// 04-08-2024 1810
// simulationEngine: Referencia a un objeto de SimulationEngine que se usará para ejecutar la simulación.
// sku: Cadena que representa el SKU (Stock Keeping Unit) del producto a simular.
// dayForSimulate: Cadena que representa el día para el cual se simularán los datos.
// numberOfIterations: Número de iteraciones a realizar en la simulación.
// daysToSimulate: Número de días a simular.
// tolerance: Tolerancia para la simulación.
void runSimulationForSKU(SimulationEngine& simulationEngine, const std::string& sku, const std::string& dayForSimulate, int numberOfIterations, int daysToSimulate, double tolerance);

// Esta línea cierra la protección del archivo de cabecera.
#endif // SIMULATION_H