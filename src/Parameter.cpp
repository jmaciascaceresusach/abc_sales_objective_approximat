#include "../include/Parameter.h"
#include <algorithm>

Parameter::Parameter(std::string name, double probability) : name(name), probability(probability) {}

void Parameter::adjustProbability(double adjustment) {
    this->probability += adjustment;
    this->probability = std::max(0.0, std::min(1.0, this->probability));
}