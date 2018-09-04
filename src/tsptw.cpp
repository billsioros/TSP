
#include "annealing.hpp"
#include "tsp.hpp"
#include "vector2.hpp"
#include <iostream>
#include <map>

int main()
{
    std::srand((unsigned)std::time(nullptr));

    auto frand = [](double min, double max)
    {
        return min + (max - min) * (static_cast<double>(std::rand()) / static_cast<double>(RAND_MAX));
    };
    
    double MIN = -100.0, MAX = 100.0; std::size_t SIZE = 24UL;

    std::vector<Vector2> points; std::map<Vector2, Vector2> timewindows;

    for (std::size_t i = 0; i < SIZE; i++)
    {
        points.emplace_back(frand(MIN, MAX), frand(MIN, MAX));
        timewindows[points.back()] = Vector2(frand(7.15, 7.30), frand(7.45, 8.00));
    }

    auto euclidean2 =
    [](const Vector2& A, const Vector2& B)
    {
        double xdiff = A.x() - B.x();
        double ydiff = A.y() - B.y();
        
        return xdiff * xdiff + ydiff * ydiff;
    };

    auto service = []()
    {
        return 30.0;
    };

    auto penalty = [&euclidean2, &timewindows](const TSP::path<Vector2>& path)
    {
        double penalty = 0.0, partial = 0.0;
        for (std::size_t j = 0; j < path.second.size() - 1UL; j++)
        {
            const Vector2& previous = path.second[j];
            const Vector2& current  = path.second[j + 1UL];

            partial += euclidean2(previous, current);

            const double departure = std::max<double>(partial, timewindows[current].x());

            penalty += std::max<double>(0.0, departure - timewindows[current].y());
        }

        return penalty;
    };

    Vector2 depot(0.0, 0.0);

    TSP::path<Vector2> path;
    
    path = TSP::nearestNeighbor<Vector2>(depot, points, euclidean2);

    std::cout << "NN:\n" << path << " Penalty: " << penalty(path) << std::endl;

    auto shift1 = [&euclidean2](const TSP::path<Vector2>& current)
    {
        TSP::path<Vector2> next(0.0, current.second);

        const std::size_t i = 1UL + std::rand() % (next.second.size() - 2UL);
        const std::size_t j = 1UL + std::rand() % (next.second.size() - 2UL);

        const Vector2 v(next.second[i]);
        next.second.erase(next.second.begin() + i);
        next.second.insert(next.second.begin() + j, v);

        next.first = TSP::totalCost<Vector2>(next.second, euclidean2);

        return next;
    };

    auto cost = [](const TSP::path<Vector2>& path)
    {
        return path.first;
    };

     // Parameter Initialization (Robust Set provided by the authors):
    const double COOLING     = 0.95,    // (1)  Cooling Coefficient
                 ACCEPTANCE  = 0.94,    // (2)  Initial Acceptance Ratio
                 PRESSURE0   = 0.0,     // (3)  Initial Pressure
                 COMPRESSION = 0.06,    // (4)  Compression Coefficient
                 PCR         = 0.9999;  // (5)  Pressure Cap Ratio

    const std::size_t IPT = 30000UL,    // (6)  Iterations per temperature
                      MTC = 100UL,      // (7)  Minimum number of temperature changes
                      ITC = 75UL,       // (8)  Maximum idle temperature changes
                      TLI = IPT,        // (9)  Trial loop of iterations
                      TNP = 5000UL;     // (10) Trial neighbour pairs

    path = Annealing::compressed<TSP::path<Vector2>>
    (
        path,
        shift1,
        cost,
        penalty,
        COOLING,
        ACCEPTANCE,
        PRESSURE0,
        COMPRESSION,
        PCR,
        IPT,
        MTC,
        ITC,
        TLI,
        TNP
    );

    std::cout << "CA:\n" << path << " Penalty: " << penalty(path) << std::endl;

    return 0;
}
