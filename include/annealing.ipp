
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

// A compressed annealing approach to the traveling salesman problem with time windows
// by
// Jeffrey W. Ohlmann
// Barrett W. Thomas
// @ https://www.researchgate.net/publication/228633085_A_compressed_annealing_approach_to_the_traveling_salesman_problem_with_time_windows
template <typename T>
T Annealing::compressed(
    const T& initial,
    const std::function<T(const T&)>& neighbour,
    const std::function<double(const T&)>& cost,
    const std::function<double(const T&)>& penalty,
    double COOLING,                                 // (1)  Cooling Coefficient
    double ACCEPTANCE,                              // (2)  Initial Acceptance Ratio
    double PRESSURE0,                               // (3)  Initial Pressure
    double COMPRESSION,                             // (4)  Compression Coefficient
    double PCR,                                     // (5)  Pressure Cap Ratio
    std::size_t IPT,                                // (6)  Iterations per temperature
    std::size_t MTC,                                // (7)  Minimum number of temperature changes
    std::size_t ITC,                                // (8)  Maximum idle temperature changes
    std::size_t TLI,                                // (9)  Trial loop of iterations
    std::size_t TNP                                 // (10) Trial neighbour pairs
)
{
    // Step 1: Parameter Calibration
    // Determine Initial Temperature & Maximum Pressure:
    double dv = 0.0, MAXPRESSURE = 0.0;
    for (std::size_t r = 0; r < 2UL * TNP; r++)
    {
        T n1 = neighbour(initial), n2 = neighbour(n1);

        const double c1 = cost(n1), p1 = penalty(n1), e1 = c1 + PRESSURE0 * p1;
        const double c2 = cost(n2), p2 = penalty(n2), e2 = c2 + PRESSURE0 * p2;

        dv += std::abs(e2 - e1);

        const double c1pressure = (c1 * PCR) / (p1 * (1.0 - PCR));
        const double c2pressure = (c2 * PCR) / (p2 * (1.0 - PCR));

        if (c1pressure > MAXPRESSURE)
            MAXPRESSURE = c1pressure;

        if (c2pressure > MAXPRESSURE)
            MAXPRESSURE = c2pressure;
    }

    double temperature = dv / std::log(1.0 / ACCEPTANCE);

    auto rand01 = []()
    {
        return static_cast<double>(std::rand()) / static_cast<double>(RAND_MAX);
    };
    
    auto probability = [&temperature](double ceval, double neval)
    {
        return neval < ceval ? 1.0 : std::exp((ceval - neval) / temperature);
    };

    for (std::size_t accepted = 0UL; ; accepted = 0UL, temperature *= 1.5)
    {
        T current = initial;
    
        double ccost = cost(current);
        double cpnlt = penalty(current);

        for (std::size_t it = 0UL; it < TLI; it++)
        {
            T next = neighbour(current);

            const double ncost = cost(next);
            const double npnlt = penalty(next);

            const double ceval = ccost + PRESSURE0 * cpnlt;
            const double neval = ncost + PRESSURE0 * npnlt;

            if (probability(ceval, neval) > rand01())
            {
                current = next; ccost = ncost; cpnlt = npnlt;

                accepted++;
            }
        }

        if ((static_cast<double>(accepted) / static_cast<double>(TLI)) >= ACCEPTANCE)
            break;
    }
    
    // Step 2: Actual Algorithm
    T current = initial, best = current;
    
    double ccost = cost(current),    bcost = ccost;
    double cpnlt = penalty(current), bpnlt = cpnlt;

    double pressure = PRESSURE0;
    for (std::size_t k = 0UL, idle = 0UL; ; k++, idle++)
    {
        for (std::size_t i = 0; i < IPT; i++)
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

        if (k >= MTC && idle >= ITC)
            break;
        
        temperature *= COOLING;

        pressure = MAXPRESSURE * (1.0 - ((MAXPRESSURE - PRESSURE0) / MAXPRESSURE) * std::exp(-1.0 * COMPRESSION * k));
    }

    return best;
}
