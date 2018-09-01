
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
    const std::function<double(const T&)>& penalty
)
{
    auto rand01 = []()
    {
        return static_cast<double>(std::rand()) / static_cast<double>(RAND_MAX);
    };
    
    auto probability = [](double temperature, double ceval, double neval)
    {
        return neval < ceval ? 1.0 : std::exp((ceval - neval) / temperature);
    };

    // Parameter Initialization:
    const double COOLING     = 0.95,    // (1) Cooling Coefficient
                 ACCEPTANCE  = 0.94,    // (2) Initial Acceptance Ratio
                 PRESSURE    = 0.0,     // (3) Initial Pressure
                 COMPRESSION = 0.06,    // (3) Compression Coefficient
                 PCR         = 0.9999;  // (4) Pressure Cap Ratio

    const std::size_t IPT = 30000UL,    // (5) Iterations per temperature
                      MTC = 100UL,      // (6) Minimum number of temperature changes
                      TLI = IPT;        // (7) Trial loop of iterations

    // Determine Initial Temperature & Maximum Pressure:
    double dv = 0.0, MPRESSURE = 0.0;
    for (std::size_t r = 0; r < 5000UL; r++)
    {
        T n1 = neighbour(initial), n2 = neighbour(n1);

        const double c1 = cost(n1), p1 = penalty(n1), e1 = c1 + PRESSURE * p1;
        const double c2 = cost(n2), p2 = penalty(n2), e2 = c2 + PRESSURE * p2;

        dv += std::abs(e2 - e1);

        const double c1pressure = (c1 * PCR) / (p1 * (1.0 - PCR));
        const double c2pressure = (c2 * PCR) / (p2 * (1.0 - PCR));

        if (c1pressure > MPRESSURE)
            MPRESSURE = c1pressure;

        if (c2pressure > MPRESSURE)
            MPRESSURE = c2pressure;
    }

    double temperature = dv / std::log(1.0 / ACCEPTANCE);

    for
    (
        std::size_t accepted = 0UL;
        (static_cast<double>(accepted) / static_cast<double>(TLI)) < ACCEPTANCE;
        accepted = 0UL, temperature *= 1.5
    )
    {
        T current = initial;
    
        double ccost = cost(current);
        double cpnlt = penalty(current);

        for (std::size_t it = 0UL; it < TLI; it++)
        {
            T next = neighbour(current);

            const double ncost = cost(next);
            const double npnlt = penalty(next);

            const double ceval = ccost + PRESSURE * cpnlt;
            const double neval = ncost + PRESSURE * npnlt;

            if (probability(temperature, ceval, neval) > rand01())
            {
                current = next; ccost = ncost; cpnlt = npnlt;

                accepted++;
            }
        }
    }
    
    // Algorithm:
    T current = initial, best = current;
    
    double ccost = cost(current),    bcost = ccost;
    double cpnlt = penalty(current), bpnlt = cpnlt;

    double pressure = PRESSURE;
    for (std::size_t k = 0UL, idle = 0UL; !(k >= MTC && idle >= 75UL) ; k++, idle++)
    {
        for (std::size_t i = 0; i < IPT; i++)
        {
            T next = neighbour(current);

            const double ncost = cost(next);
            const double npnlt = penalty(next);

            const double ceval = ccost + pressure * cpnlt;
            const double neval = ncost + pressure * npnlt;

            if (probability(temperature, ceval, neval) > rand01())
            {
                current = next; ccost = ncost; cpnlt = npnlt;
            }

            if ((cpnlt <= bpnlt) && (ccost < bcost))
            {
                best = current; bcost = ccost; bpnlt = cpnlt;

                idle = 0UL;
            }
        }
        
        temperature *= COOLING;

        pressure = MPRESSURE * (1.0 - ((MPRESSURE - pressure) / MPRESSURE) * std::exp(-1.0 * COMPRESSION * k));
    }

    return best;
}
