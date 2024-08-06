#ifndef DATALOADER_H
#define DATALOADER_H

// La protección del archivo de cabecera evita la inclusión múltiple del mismo archivo, lo que puede causar errores de redefinición.

#include <vector>
#include <string>
#include <map>
#include "ABCMethod.h" // Para la definición de SKUData

/*
vector: Utilizado para manejar listas de elementos.
string: Utilizado para manejar cadenas de texto.
map: Utilizado para manejar asociaciones clave-valor.
ABCMethod.h: Incluye la definición de SKUData, necesario para algunas de las funciones declaradas en este archivo.
*/

/*
Comentarios generales:
- El archivo DataLoader.h declara una serie de funciones y clases auxiliares para cargar y manejar datos desde archivos. Estas funciones son cruciales para preparar los datos necesarios para las simulaciones, manejar fechas y realizar normalización de datos. Aquí tienes un resumen de las funcionalidades principales:
- HistoricalData: Clase para manejar datos históricos, incluyendo la carga desde un archivo CSV.
- Funciones para Cargar Datos: Incluyen funciones para cargar datos de SKU, características normalizadas y no normalizadas, configuraciones de simulación, valores, pesos de atributos e intervalos de SKU.
- Funciones para Manejar Fechas y Normalización: Incluyen funciones para obtener la fecha y hora actuales, y para convertir entre valores de z-score y valores originales.

Estas herramientas son esenciales para preparar y gestionar los datos utilizados en simulaciones de precios y otros análisis relacionados.
*/

// SimulationEngine: Declaración anticipada de la clase SimulationEngine, utilizada para evitar dependencias circulares.
class SimulationEngine; // 04-08-2024 1810

// features: Vector de nombres de características.
// records: Vector de mapas que almacenan registros históricos.
// loadFromCSV: Método para cargar datos desde un archivo CSV.
class HistoricalData { // 04-08-2024 1820
public:
    std::vector<std::string> features;

    std::vector<std::map<std::string, double>> records;
    
    void loadFromCSV(const std::string& filename);
};

// loadSKUData: Carga los datos del SKU desde un archivo.
SKUData loadSKUData(const std::string& filename);

// loadNormalizedFeatures: Carga características normalizadas desde un archivo.
std::map<std::string, double> loadNormalizedFeatures(const std::string& filename);

// loadNoNormalizedFeatures: Carga características no normalizadas desde un archivo.
std::map<std::string, double> loadNoNormalizedFeatures(const std::string& filename);

// loadSimulationConfig: Carga la configuración de la simulación desde un archivo.
void loadSimulationConfig(const std::string& filename, int& numberOfIterations, int& numberOfRefinements, int& tolerance, int& daysToSimulate, std::string& dayForSimulate, std::string& skuForSimulate); // 05-08-2024 1004

// loadValues: Carga valores desde un archivo.
std::map<std::string, double> loadValues(const std::string& filename); // 04-08-2024 1714

// loadAttributeWeights: Carga los pesos de los atributos desde un archivo.
std::map<std::string, double> loadAttributeWeights(const std::string& filename); // 04-08-2024 1714

// loadSKUIntervals: Carga intervalos de SKU desde un archivo.
std::map<std::string, std::vector<std::pair<double, double>>> loadSKUIntervals(const std::string& filename); // 04-08-2024 1714

// getAllSKUs: Obtiene una lista de todos los SKU disponibles.
std::vector<std::string> getAllSKUs();

// getCurrentDate: Obtiene la fecha actual en un formato específico.
std::string getCurrentDate();

// getCurrentDateTime: Obtiene la fecha y la hora actuales en un formato específico.
std::string getCurrentDateTime(); // 05-08-2024 1434

// inverse_z_score: Convierte valores de z-score a valores originales utilizando medias y desviaciones estándar.
std::map<std::string, double> inverse_z_score(const std::map<std::string, double>& z_score_normalized,
                                              const std::map<std::string, double>& mean_values,
                                              const std::map<std::string, double>& std_values);

// calculate_z_score: Calcula los z-scores de un registro utilizando medias y desviaciones estándar.
std::map<std::string, double> calculate_z_score(const std::map<std::string, double>& registro,
                                                const std::map<std::string, double>& mean_values,
                                                const std::map<std::string, double>& std_values);

std::map<std::string, double> loadValues(const std::string& filename);

// Esta línea cierra la protección del archivo de cabecera.
#endif // DATALOADER_H