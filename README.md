**ABC_SALES_OBJECTIVE_APPROXIMAT**

This is the v0.5 repository of the application of the ABC method in a pricing case study.

Below are the statements for the build of the project.

Summary:
- Project Structure: The project has an organized structure with separate directories for source code (src), header files (include), input data (data/input), and output data (data/output).
- ABC Method Implementation: The project implements the Approximate Bayesian Computation (ABC) method to simulate and predict product prices. The ABCMethod class in ABCMethod.cpp contains the main logic for refining parameters, simulating future prices, and calculating probabilities.
- Data Handling: The project uses various data structures such as SKUData to store information about products and their price ranges. The DataLoader class is responsible for loading data from CSV and text files.
- Simulation: The SimulationEngine class in SimulationEngine.cpp is responsible for running the simulations. It handles data preparation, running multiple simulation iterations, and logging results.
- Flexible configuration: The project allows configuring simulation parameters such as the number of iterations, tolerance, and days to simulate through a configuration file (simulation_config_initial.txt).
- Detailed outputs: The program generates detailed log files (simulation_log) and statistics (statistics_simulations) for each simulation, allowing for later analysis of the results.
- Handling multiple SKUs: The project can simulate for a specific SKU or for all available SKUs, making it flexible for different usage scenarios.
- Use of advanced statistical techniques: The code implements calculations of historical trends, seasonality, autocorrelation, and volatility to adjust future price probabilities.
- Error handling: The code includes input data checks and error handling to ensure program robustness.
- Build tools: The project uses CMake and has a Makefile, making it easy to compile in different environments.

---

## Build

This is the v0.4 repository of the application of the ABC (Approximate Bayesian Computation) method in a pricing case study.

Below are the statements for the build of the project.

From your virtual machine, run the following:
1. git clone **https://github.com/jmaciascaceresusach/abc_sales_objective_approximat.git**
2. mkdir abc_sales_objective_approximat/build
3. cd abc_sales_objective_approximat/build
4. cmake .. (Important: there is a blank space between the cmake and the two points)
5. make

Now, if you only need to download the latest version, having already initialized the project, you only have to do the following:

1. git pull **https://github.com/jmaciascaceresusach/abc_sales_objective_approximat.git**
2. cd abc_sales_objective_approximat/build
3. cmake .. (Important: there is a blank space between the cmake and the two points)
4. make

After this, verify the compilation that was carried out correctly and was also executed.

Now, to test the project, you would have to use the following statements:

1. cd abc_sales_objective_approximat/build
2. ./ABC_SALES_OBJECTIVE_APPROXIMAT

The simulation_config_initial.txt file, can be modified according to:

- numberOfIterations=100
- numberOfRefinements=100
- tolerance=13
- daysToSimulate=30
- dayForSimulate=2024-08-17
- skuForSimulate=U391805

Additionally, upon completion of the execution, it will generate output files in the data/output/ folder.