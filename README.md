**ABC_SALES_OBJECTIVE_APPROXIMAT**

This is the v0.3 repository of the application of the ABC method in a pricing case study.

Below are the statements for the build of the project.

---

## Build

This is the v0.3 repository of the application of the ABC (Approximate Bayesian Computation) method in a pricing case study.

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

- numberOfIterations=4
- numberOfRefinements=5
- tolerance=13
- daysToSimulate=5
- dayForSimulate=2024-08-03
- skuForSimulate=Z285320

ó

- numberOfIterations=4
- numberOfRefinements=5
- tolerance=13
- daysToSimulate=5
- dayForSimulate=2024-08-03
- skuForSimulate=All