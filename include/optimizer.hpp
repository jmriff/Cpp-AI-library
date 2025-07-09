#ifndef __OPTIMIZER_HPP__
#define __OPTIMIZER_HPP__

#include <vector>
#include <cmath>

class RosenbrockOptimizer
{
public:
    RosenbrockOptimizer() {}
    
    double optimize(std::vector<double>& x);
};

class RosenbrockGradientOptimizer
{
public:
    RosenbrockGradientOptimizer() {}
    
    std::vector<double> optimize(std::vector<double>& x);
};

#endif // __OPTIMIZER_HPP__