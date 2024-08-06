#ifndef SIMULATIONENGINE_H
#define SIMULATIONENGINE_H

// La protección del archivo de cabecera evita que el contenido se incluya más de una vez en una única compilación, lo cual puede causar errores de redefinición.

#include <vector>
#include <map>
#include "ABCMethod.h"
#include "Parameter.h"
#include "DataLoader.h" // 04-08-2024 1820

/*
vector: Utilizado para manejar listas de elementos.
map: Utilizado para manejar asociaciones clave-valor.
ABCMethod.h: Incluye la definición de la clase ABCMethod, necesaria para la simulación.
Parameter.h: Incluye la definición de la clase Parameter, que representa los parámetros de la simulación.
DataLoader.h: Incluye funciones y clases auxiliares para cargar datos desde archivos.
*/

/*
Comentarios generales:
- El archivo SimulationEngine.h define la clase SimulationEngine, que gestiona la ejecución de las simulaciones de precios. Esta clase es responsable de manejar 
los datos de entrada, normalizar y desnormalizar características, y ejecutar las simulaciones utilizando el método ABCMethod. A continuación se detallan las 
funcionalidades principales:

Atributos Públicos y Métodos
- Atributos: Almacenan parámetros, características, datos históricos y configuraciones necesarias para la simulación.
- Métodos Públicos: Incluyen funciones para añadir parámetros, configurar datos de productos y características, cargar valores medios y desviaciones estándar, y ejecutar 
simulaciones.

Métodos Privados
- Normalización y Desnormalización: Métodos para normalizar y desnormalizar características.
- Carga de Datos Históricos: Método para cargar datos históricos desde un archivo.

La clase SimulationEngine proporciona una interfaz estructurada para configurar y ejecutar simulaciones complejas, manejando internamente todos los detalles necesarios 
para preparar los datos y ejecutar los cálculos necesarios.
*/

class SimulationEngine {
public:

    // Constructor: Inicializa un objeto SimulationEngine.
    SimulationEngine();

    // Añade un parámetro a la lista de parámetros de la simulación.
    void addParameter(const Parameter& parameter);

    // Establece los datos del producto (SKUData) para la simulación.
    void setProductData(const SKUData& data);

    // Establece las características normalizadas en el motor de simulación.
    void setNormalizedFeatures(const std::map<std::string, double>& features);

    // Establece las características no normalizadas en el motor de simulación.
    void setNoNormalizedFeatures(const std::map<std::string, double>& features);

    // Ejecuta las simulaciones con el número de iteraciones, días a simular y tolerancia especificados.
    void runSimulations(int numberOfIterations, int daysToSimulate, double tolerance);

    // Carga los valores medios y las desviaciones estándar desde archivos.
    void loadMeanAndStdValues(const std::string& meanFilename, const std::string& stdFilename);

    // 04-08-2024 2012
    // Establece el día para simular en el motor de simulación.
    void setDayForSimulate(const std::string& day) { 
        dayForSimulate = day; 
    }

    int numberOfRefinements; // 05-08-2024 1006

private:

    // Lista de parámetros utilizados en la simulación.
    std::vector<Parameter> parameters;

    // Instancia de ABCMethod utilizada para la simulación.
    ABCMethod abcMethod;
    
    // Mapa de características normalizadas.
    std::map<std::string, double> normalizedFeatures;

    // Mapa de características no normalizadas.
    std::map<std::string, double> noNormalizedFeatures;

    // Mapa de valores medios para la normalización.
    std::map<std::string, double> meanValues;
    
    // Mapa de desviaciones estándar para la normalización.
    std::map<std::string, double> stdValues;

    // Mapa de pesos de atributos utilizados en la simulación
    std::map<std::string, double> attributeWeights; // 04-08-2024 1714

    // Mapa de intervalos de SKU utilizados en la simulación.
    std::map<std::string, std::vector<std::pair<double, double>>> skuIntervals; // 04-08-2024 1714

    // Normaliza un valor utilizando los valores medios y desviaciones estándar.
    double normalize(double value, const std::string& feature); // 04-08-2024 1714

    // Desnormaliza un valor utilizando los valores medios y desviaciones estándar.
    double denormalize(double normalizedValue, const std::string& feature); // 04-08-2024 1714

    // Instancia de HistoricalData utilizada para almacenar datos históricos.
    HistoricalData historicalData; // 04-08-2024 1714

    // Carga datos históricos desde un archivo.
    void loadHistoricalData(const std::string& filename); // 04-08-2024 1714

    // Cadena que representa el día para el cual se realizará la simulación.
    std::string dayForSimulate; // 04-08-2024 2013

    // Datos del SKU utilizados en la simulación.
    SKUData skuData; // 04-08-2024 2013
};

// Esta línea cierra la protección del archivo de cabecera.
#endif // SIMULATIONENGINE_H