
#include "vector2.hpp"
#include "tsp.hpp"
#include "annealing.hpp"

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
    double MIN = -100.0, MAX = 100.0; std::size_t SIZE = 24UL;

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

    #ifndef __TEST__
    std::cout << "NN:\n" << path << std::endl;
    #else
    std::cout << "NN: " << path.first << std::endl;
    #endif

    path = TSP::opt2<Vector2>(path.second.front(), path.second, cost);

    #ifndef __TEST__
    std::cout << "OPT21:\n" << path << std::endl;
    #else
    std::cout << "OPT21: " << path.first << std::endl;
    #endif

    path = Annealing::simulated<TSP::path<Vector2>>(
        path,
        [&cost](const TSP::path<Vector2>& current)
        {
            TSP::path<Vector2> next(0.0, current.second);

            const std::size_t i = 1UL + std::rand() % (next.second.size() - 2UL);
            const std::size_t j = 1UL + std::rand() % (next.second.size() - 2UL);

            const Vector2 temp(next.second[i]);
            next.second[i] = next.second[j];
            next.second[j] = temp;

            next.first = TSP::totalCost<Vector2>(next.second, cost);

            return next;
        },
        [](const TSP::path<Vector2>& path)
        {
            return path.first;
        },
        1000000000.0,
        0.0015,
        1000000UL
    );

    #ifndef __TEST__
    std::cout << "SA:\n" << path << std::endl;
    #else
    std::cout << "SA: " << path.first << std::endl;
    #endif

    path = TSP::opt2<Vector2>(path.second.front(), path.second, cost);

    #ifndef __TEST__
    std::cout << "OPT22:\n" << path << std::endl;
    #else
    std::cout << "OPT22: " << path.first << std::endl;
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
