
#include "vector2.hpp"
#include "tsp.hpp"
#include "sa.hpp"

#include <vector>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <algorithm>
#include <sstream>

template <typename T>
T str2num(const char *);

int main(int argc, char * argv[])
{
    double MIN = -100.0, MAX = 100.0; std::size_t SIZE = 8UL;

    if (argc > 3)
    {
        MIN  = str2num<double>(argv[1]);
        MAX  = str2num<double>(argv[2]);
        SIZE = str2num<std::size_t>(argv[3]);
    }

    std::srand((unsigned)std::time(nullptr));

    auto frand = [](double min, double max)
    {
        return min + (max - min) * (static_cast<double>(std::rand()) / static_cast<double>(RAND_MAX));
    };

    std::vector<Vector2> points;

    for (std::size_t i = 0; i < SIZE; i++)
        points.emplace_back(frand(-MAX, MAX), frand(-MAX, MAX));

    auto cost =
    [](const Vector2& A, const Vector2& B)
    {
        double xdiff = A.x() - B.x();
        double ydiff = A.y() - B.y();
        
        return xdiff * xdiff + ydiff * ydiff;
    };

    Vector2 depot(0.0, 0.0);

    TSP::path<Vector2> path;
    
    path = TSP::nearestNeighbor<Vector2>(depot, points, cost);
    path = TSP::opt2<Vector2>(path.second.front(), path.second, cost);

    #ifndef __TEST__
    std::cout << "\n" << path << std::endl;
    #else
    std::cout << "NN: " << path.first << std::endl;
    #endif

    path.second = SimulatedAnnealing<std::vector<Vector2>>(
        path.second,
        [](const std::vector<Vector2>& current)
        {
            std::vector<Vector2> next(current);

            const std::size_t i = 1UL + std::rand() % (next.size() - 2UL);
            const std::size_t j = 1UL + std::rand() % (next.size() - 2UL);

            std::reverse(next.begin() + i, next.begin() + j);

            return next;
        },
        [&cost](const std::vector<Vector2>& current)
        {
            double total = 0.0;
            for (std::size_t j = 0; j < current.size() - 1UL; j++)
                total += cost(current[j], current[j + 1UL]);

            return total;
        },
        1000000.0,
        0.00003,
        1000000UL
    );

    path = TSP::opt2<Vector2>(path.second.front(), path.second, cost);

    #ifndef __TEST__
    std::cout << "\n" << path << std::endl;
    #else
    std::cout << "SA: " << path.first << std::endl;
    #endif
}

template <typename T>
T str2num(const char * str)
{
    std::stringstream ss(str);

    T num;
    if (ss >> num)
    {
        return num;
    }
    else
    {
        std::cerr << "<ERR>: Malformed arguement (" << str << ")" << std::endl;
        std::exit(EXIT_FAILURE);                                
    }
}
