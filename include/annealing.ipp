
#pragma once

#include <functional>   // std::function
#include <cmath>        // std::exp
#include <cstdlib>      // std::rand
#include <ctime>        // std::time

template <typename T>
T Annealing::simulated(
    const T& initial,
    const std::function<T(const T&)>& neighbour,
    const std::function<double(const T&)>& cost,
    double temperature,
    double cooling,
    std::size_t iterations
)
{
    const double _temperature = temperature;

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

template <typename T>
T Annealing::compressed(
    const T& initial,
    const std::function<T(const T&)>& neighbour,
    const std::function<double(const T&)>& cost,
    const std::function<double(const T&)>& penalty,
    double temperature,
    double pressure,
    double pmax
)
{
    const double cooling = 0.95, compression = 0.06;

    const std::size_t iterations = 30000UL;

    auto rand01 = []()
    {
        return static_cast<double>(std::rand()) / static_cast<double>(RAND_MAX);
    };
    
    auto probability = [&temperature](double ccost, double ncost)
    {
        return ncost < ccost ? 1.0 : std::exp((ccost - ncost) / temperature);
    };

    T current = initial, best = current;
    
    double ccost = cost(current),    bcost = ccost;
    double cpnlt = penalty(current), bpnlt = cpnlt;

    const double pressure0 = pressure;
    for (std::size_t k = 0UL, idle = 0UL; !(k >= 100UL && idle >= 75UL) ; k++, idle++)
    {
        for (std::size_t i = 0; i < iterations; i++)
        {
            T next = neighbour(current);

            const double ncost = cost(next);
            const double npnlt = penalty(next);

            const double ceval = ccost + pressure * cpnlt;
            const double neval = ncost + pressure * npnlt;

            if (probability(ceval, neval) > rand01())
            {
                current = next; ccost = ncost; cpnlt = npnlt;
            }

            if ((cpnlt <= bpnlt) && (ccost < bcost))
            {
                best = current; bcost = ccost; bpnlt = cpnlt;

                idle = 0UL;
            }
        }
        
        temperature *= cooling;

        pressure = pmax * (1.0 - ((pmax - pressure0) / pmax) * std::exp(-1.0 * compression * k));
    }

    return best;
}
