#include "../include/Parameter.h"
#include "../include/SimulationEngine.h"
#include <iostream>
#include <vector>
#include <cstdlib> // para std::atoi and std::atof
#include <fstream>  
#include <sstream>
#include <chrono>

/**
 * Función para calcular ventas simuladas en función de parámetros.
 * @param parameters Lista de parámetros utilizados para calcular las ventas.
 * @return Valor total de las ventas calculadas.
 */
double calculateSale(const std::vector<Parameter>& parameters) {
    double totalSaleValue = 0.0;
    for (const auto& param : parameters) {
        if (param.name == "customerParam") {
            totalSaleValue += param.probability * 27;
        } else if (param.name == "dateParam") {
            totalSaleValue += param.probability * 16;
        } else if (param.name == "sellerParam") {
            totalSaleValue += param.probability * 22;
        } else if (param.name == "productParam") {
            totalSaleValue += param.probability * 5;
        } else if (param.name == "saleParam") {
            totalSaleValue += param.probability * 30;
        }
    }
    return totalSaleValue;
}

/**
 * Lee una configuración completa desde un archivo para 8 parámetros.
 * @param configFilePath Ruta al archivo de configuración.
 * @param numberOfIterations Variable donde se almacenará el número de iteraciones.
 * @param salesObjectiveInitial Variable donde se almacenará el objetivo de ventas inicial.
 * @param salesObjectiveFinal Variable donde se almacenará el objetivo de ventas final.
 * @param tolerance Variable donde se almacenará la tolerancia.
 * @param customerParam Variable donde se almacenará el valor inicial de customerParam.
 * @param sellerParam Variable donde se almacenará el valor inicial de sellerParam.
 * @param saleParam Variable donde se almacenará el valor inicial de saleParam.
 * @param dateParam Variable donde se almacenará el valor inicial de dateParam.
 * @param productParam Variable donde se almacenará el valor inicial de productParam.
 */
void readConfigFor8(const std::string& configFilePath, 
                    int& numberOfIterations, 
                    double& salesObjectiveInitial, 
                    double& salesObjectiveFinal, 
                    double& tolerance,
                    double& customerParam,
                    double& sellerParam,
                    double& saleParam,
                    double& dateParam,
                    double& productParam/*,
                    double& totalSaleValue*/) {
    std::ifstream configFile(configFilePath);
    std::string line;

    if (configFile.is_open()) {
        while (getline(configFile, line)) {
            std::istringstream iss(line);
            std::string key;
            if (getline(iss, key, '=')) {
                std::string value;
                if (getline(iss, value)) {
                    if (key == "numberOfIterations") numberOfIterations = std::stoi(value);
                    else if (key == "salesObjectiveInitial") salesObjectiveInitial = std::stod(value);
                    else if (key == "salesObjectiveFinal") salesObjectiveFinal = std::stod(value);
                    else if (key == "tolerance") tolerance = std::stod(value);
                    else if (key == "customerParam") customerParam = std::stod(value);
                    else if (key == "sellerParam") sellerParam = std::stod(value);
                    else if (key == "saleParam") saleParam = std::stod(value);
                    else if (key == "dateParam") dateParam = std::stod(value);
                    else if (key == "productParam") productParam = std::stod(value);
                    /*else if (key == "totalSaleValue") totalSaleValue = std::stod(value);*/
                }
            }
        }
    } else {
        std::cerr << "Failed to open config file: " << configFilePath << std::endl;
    }
}

int main(int argc, char* argv[]) {
    // Obtiene el tiempo de inicio
    auto start = std::chrono::high_resolution_clock::now();

    int numberOfIterations;
    double salesObjectiveInitial, salesObjectiveFinal, tolerance;
    double customerParam, sellerParam, saleParam, dateParam, productParam/*, totalSaleValue*/;

    readConfigFor8("../simulation_config.txt", 
                    numberOfIterations, 
                    salesObjectiveInitial,
                    salesObjectiveFinal, 
                    tolerance,
                    customerParam,
                    sellerParam,
                    saleParam,
                    dateParam,
                    productParam/*,
                    totalSaleValue*/);

    // Inicializar el motor de simulación
    SimulationEngine simulationEngine;

    // Definir y agregar parámetros
    Parameter customerParamParameter("customerParam", customerParam);
    Parameter sellerParamParameter("sellerParam", sellerParam);
    Parameter saleParamParameter("saleParam", saleParam);
    Parameter dateParamParameter("dateParam", dateParam);
    Parameter productParamParameter("productParam", productParam);
    /*Parameter totalSaleValueParameter("totalSaleValue", totalSaleValue);*/

    // Agregar parámetros al motor de simulación
    simulationEngine.addParameter(customerParamParameter);
    simulationEngine.addParameter(sellerParamParameter);
    simulationEngine.addParameter(saleParamParameter);
    simulationEngine.addParameter(dateParamParameter);
    simulationEngine.addParameter(productParamParameter);
    /*simulationEngine.addParameter(totalSaleValueParameter);*/

    // Ejecutar simulaciones
    simulationEngine.runSimulations(numberOfIterations, calculateSale, salesObjectiveFinal, tolerance);

    std::cout << "\n";
    std::cout << "***Configurations***" << std::endl;
    std::cout << "numberOfIterations: " << numberOfIterations << std::endl;
    std::cout << "salesObjectiveInitial: " << salesObjectiveInitial << std::endl;
    std::cout << "salesObjectiveFinal: " << salesObjectiveFinal << std::endl;
    std::cout << "tolerance: " << tolerance << std::endl;
    std::cout << "\n";

    std::cout << "***Startup Parameters***" << std::endl;
    std::cout << "customerParam: " << customerParam << std::endl;
    std::cout << "sellerParam: " << sellerParam << std::endl;
    std::cout << "saleParam: " << saleParam << std::endl;
    std::cout << "dateParam: " << dateParam << std::endl;
    std::cout << "productParam: " << productParam << std::endl;
    /*std::cout << "totalSaleValue: " << totalSaleValue << std::endl;*/
    std::cout << "\n";

    // Obtiene el tiempo de finalización
    auto end = std::chrono::high_resolution_clock::now();

    // Calcula la duración en milisegundos
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

    std::cout << "***Duration***" << std::endl;
    std::cout << "Time: " << duration.count() << " milliseconds" << std::endl;
    std::cout << "\n";

    return 0;
}
