
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

    tsp<Vector2> path(depot, points, [](const Vector2& v) { return 0.0; }, cost);
    
    path = path.nneighbour();

    #ifndef __TEST__
    std::cout << "NN:\n" << path << std::endl;
    #else
    std::cout << "NN: " << path.cost() << std::endl;
    #endif

    path = path.opt2();

    #ifndef __TEST__
    std::cout << "OPT21:\n" << path << std::endl;
    #else
    std::cout << "OPT21: " << path.cost() << std::endl;
    #endif

    path = path.sannealing();

    #ifndef __TEST__
    std::cout << "SA:\n" << path << std::endl;
    #else
    std::cout << "SA: " << path.cost() << std::endl;
    #endif

    path = path.opt2();

    #ifndef __TEST__
    std::cout << "OPT22:\n" << path << std::endl;
    #else
    std::cout << "OPT22: " << path.cost() << std::endl;
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
