
#include "annealing.hpp"
#include "tsp.hpp"
#include "tstamp.hpp"
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

    std::vector<Vector2> points; std::map<Vector2, tsptw<Vector2>::Timewindow> timewindows;

    for (std::size_t i = 0; i < SIZE; i++)
    {
        points.emplace_back(frand(MIN, MAX), frand(MIN, MAX));

        TStamp l(7, static_cast<uint8_t>(frand(15.0, 30.0)));
        TStamp r(8, static_cast<uint8_t>(frand(0.0,  15.0)));

        timewindows[points.back()] = tsptw<Vector2>::Timewindow
        (
            static_cast<double>(l.seconds()),
            static_cast<double>(r.seconds())
        );
    }

    auto timewindow = [&timewindows](const Vector2& point)
    {
        return timewindows[point];
    };

    auto euclidean2 =
    [](const Vector2& A, const Vector2& B)
    {
        double xdiff = A.x() - B.x();
        double ydiff = A.y() - B.y();
        
        return xdiff * xdiff + ydiff * ydiff;
    };

    Vector2 depot(0.0, 0.0);

    tsptw<Vector2> path
    (
        depot,
        points,
        [&depot](const Vector2& v)
        {
            return v == depot ? 0.0 : 30.0;
        },
        euclidean2,
        TStamp(7, 30).seconds(),
        timewindow
    );

    path = path.nneighbour();

    std::cout << "NN:\n" << path << std::endl;

    path = path.cannealing();

    std::cout << "CA:\n" << path << std::endl;

    return 0;
}
