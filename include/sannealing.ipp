
#pragma once

#include <functional>   // std::function
#include <cmath>        // std::exp
#include <cstdlib>      // std::rand
#include <ctime>        // std::time

template <typename T>
T SimulatedAnnealing(
    const T& initial,
    const std::function<T(const T&)>& neighbour,
    const std::function<double(const T&)>& cost,
    double temperature,
    double cooling,
    std::size_t iterations
)
{
    const double _temperature = temperature;

    std::srand(static_cast<unsigned>(std::time(nullptr)));
    auto rand01 = []()
    {
        return static_cast<double>(std::rand()) / static_cast<double>(RAND_MAX);
    };
    
    auto probability = [&temperature](double ccost, double ncost)
    {
        return ncost < ccost ? 1.0 : std::exp((ccost - ncost) / temperature);
    };

    T current    = initial,       best  = current;
    double ccost = cost(current), bcost = ccost;

    std::size_t counter = 0UL;
    do
    {
        T next = neighbour(current);

        const double ncost = cost(next);

        if (probability(ccost, ncost) > rand01())
        {
            current = next; ccost = ncost;
        }

        if (ccost < bcost)
        {
            best = current; bcost = ccost;
            
            counter = 0UL; temperature = _temperature;
        }
    } while (counter++ < iterations && (temperature *= (1.0 - cooling)) > 1.0);

    return best;
}
