#ifndef PARAMETER_H
#define PARAMETER_H

#include <string>

class Parameter {
public:
    std::string name;
    double probability;

    Parameter(std::string name, double probability);
    void adjustProbability(double adjustment);
};

#endif // PARAMETER_H