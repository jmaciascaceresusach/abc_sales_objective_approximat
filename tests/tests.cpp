#define CATCH_CONFIG_MAIN
#include "../include/catch.hpp"
#include "../include/Parameter.h"
#include "../include/ABCMethod.h"
#include "../include/SimulationEngine.h"

double calculateSaleForTesting(const std::vector<Parameter>& parameters) {
    double totalSaleValue = 0.0;
    for (const auto& param : parameters) {
        if (param.name == "customerType") {
            totalSaleValue += param.probability * 80;
        } else if (param.name == "typeOfSeller") {
            totalSaleValue += param.probability * 50;
        } else if (param.name == "numberOfProductsSold") {
            totalSaleValue += param.probability * 90;
        } else if (param.name == "saleDate") {
            totalSaleValue += param.probability * 20;
        } else if (param.name == "products") {
            totalSaleValue += param.probability * 10;
        } else if (param.name == "totalSaleValue") {
            totalSaleValue += param.probability * 11;
        } else if (param.name == "priceDiscounts") {
            totalSaleValue += param.probability * 21;
        } else if (param.name == "deliveryTime") {
            totalSaleValue += param.probability * 34;
        } else if (param.name == "productType") {
            totalSaleValue += param.probability * 24;
        } else if (param.name == "productList") {
            totalSaleValue += param.probability * 65;
        } else if (param.name == "inventoryLevel") {
            totalSaleValue += param.probability * 79;
        } else if (param.name == "perceptionOfRelationshipValue") {
            totalSaleValue += param.probability * 12;
        } else if (param.name == "marketParticipation") {
            totalSaleValue += param.probability * 45;
        } else if (param.name == "otherFactors") {
            totalSaleValue += param.probability * 72;
        }
    }
    return totalSaleValue;
}

TEST_CASE("Test ABCMethod adjustment methods") {
    ABCMethod abcMethod;
    std::vector<Parameter> parameters = {
        {"customerType", 0.1},
        {"typeOfSeller", 0.2},
        {"numberOfProductsSold", 0.3},
        {"saleDate", 0.4},
        {"products", 0.5}
    };

    double salesObjective = 100.0;

    SECTION("Test dynamicAdjustParameters") {
        double saleValue = calculateSaleForTesting(parameters);
        abcMethod.dynamicAdjustParameters(parameters, saleValue, salesObjective);
        REQUIRE(parameters[0].probability <= 1.0);
        REQUIRE(parameters[0].probability >= 0.0);
    }

    SECTION("Test dynamicAdjustParametersGradient") {
        double saleValue = calculateSaleForTesting(parameters);
        abcMethod.dynamicAdjustParametersGradient(parameters, saleValue, salesObjective);
        REQUIRE(parameters[0].probability <= 1.0);
        REQUIRE(parameters[0].probability >= 0.0);
    }

    SECTION("Test dynamicAdjustParametersSlidingAverage") {
        double saleValue = calculateSaleForTesting(parameters);
        abcMethod.dynamicAdjustParametersSlidingAverage(parameters, saleValue, salesObjective);
        REQUIRE(parameters[0].probability <= 1.0);
        REQUIRE(parameters[0].probability >= 0.0);
    }

    SECTION("Test dynamicAdjustParametersGenetic") {
        double saleValue = calculateSaleForTesting(parameters);
        abcMethod.dynamicAdjustParametersGenetic(parameters, calculateSaleForTesting, saleValue, salesObjective);
        REQUIRE(parameters[0].probability <= 1.0);
        REQUIRE(parameters[0].probability >= 0.0);
    }

    SECTION("Test dynamicAdjustParametersSimulatedAnnealing") {
        double saleValue = calculateSaleForTesting(parameters);
        abcMethod.dynamicAdjustParametersSimulatedAnnealing(parameters, calculateSaleForTesting, saleValue, salesObjective);
        REQUIRE(parameters[0].probability <= 1.0);
        REQUIRE(parameters[0].probability >= 0.0);
    }

    SECTION("Test dynamicAdjustParametersLM") {
        double saleValue = calculateSaleForTesting(parameters);
        abcMethod.dynamicAdjustParametersLM(parameters, saleValue, salesObjective);
        REQUIRE(parameters[0].probability <= 1.0);
        REQUIRE(parameters[0].probability >= 0.0);
    }
}

TEST_CASE("Test SimulationEngine") {
    SimulationEngine engine;
    engine.addParameter(Parameter("customerType", 0.1));
    engine.addParameter(Parameter("typeOfSeller", 0.2));
    engine.addParameter(Parameter("numberOfProductsSold", 0.3));

    int numberOfIterations = 5;
    double salesObjective = 200.0;
    double tolerance = 10.0;

    SECTION("Run simulations with correct configuration") {
        engine.runSimulations(numberOfIterations, calculateSaleForTesting, salesObjective, tolerance);
        REQUIRE(engine.parameters.size() == 3);
    }
}
