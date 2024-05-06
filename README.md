**ABC_SALES_OBJECTIVE_APPROXIMAT**

This is the v0.1 repository of the application of the ABC method in a pricing case study.

Below are the statements for the build of the project.

---

## Build

This is the v0.1 repository of the application of the ABC (Approximate Bayesian Computation) method in a pricing case study.

Below are the statements for the build of the project.

From your virtual machine, run the following:
1. git clone **https://github.com/jmaciascaceresusach/abc_sales_objective_approximat.git**
2. mkdir abc_sales_objective_approximat/build
3. cd abc_sales_objective_approximat/build
4. cmake .. (Important: there is a blank space between the cmake and the two points)
5. make

After this, verify the compilation that was carried out correctly and was also executed.

Now, to test the project, you would have to use the following statements:

1. cd abc_sales_objective_approximat/build
2. ./ABC_SALES_OBJECTIVE_APPROXIMAT ../simulation_config.txt

The simulation_config.txt file, can be modified according to:

- numberOfIterations=10000
- salesObjective=1000.5
- tolerance=900.12
- customerType=0.4
- typeOfSeller=0.2
- numberOfProductsSold=0.3
- saleDate=0.1
- products=0.2
- totalSaleValue=0.2