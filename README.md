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

Now, if you only need to download the latest version, having already initialized the project, you only have to do the following:

1. git pull **https://github.com/jmaciascaceresusach/abc_sales_objective_approximat.git**
2. cd abc_sales_objective_approximat/build
3. cmake .. (Important: there is a blank space between the cmake and the two points)
4. make

After this, verify the compilation that was carried out correctly and was also executed.

Now, to test the project, you would have to use the following statements:

1. cd abc_sales_objective_approximat/build
2. ./ABC_SALES_OBJECTIVE_APPROXIMAT ../simulation_config.txt

The simulation_config.txt file, can be modified according to:

- numberOfIterations=50
- salesObjective=46.005
- tolerance=2.120
- customerType=0.00314591338431157
- typeOfSeller=0.102829848201114
- numberOfProductsSold=0.122376749687935
- saleDate=0.0112380619822541
- products=0.0259187661818709
- totalSaleValue=0.0470569157186043
- priceDiscounts=0.0207999784257691
- deliveryTime=0.110643138673347
- productType=0.16256338556168
- productList=0.153456118173069
- inventoryLevel=0.00422676784987325
- perceptionOfRelationshipValue=0.0655840981931411
- marketParticipation=0.156170822929046
- otherFactors=0.0139894350379837