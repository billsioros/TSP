# The Travelling Salesman Problem in C++

## Algorithms:
* Nearest Neighbour
* Opt 2
* Simulated Annealing
* Compressed Annealing

## Papers:
* [A Compressed-Annealing Heuristic for the Traveling Salesman Problem with Time Windows](https://www.researchgate.net/publication/220669433_A_Compressed-Annealing_Heuristic_for_the_Traveling_Salesman_Problem_with_Time_Windows)
* [An Effective Heuristic Algorithm for the Travelling-Salesman Problem](https://pubsonline.informs.org/doi/10.1287/opre.21.2.498)

## Examples:
### Data initialization - TSP
```C++
// Seed the random number generator
std::srand(static_cast<unsigned>(std::time(nullptr)));

// A lambda function returning a random double in the given range
auto frand = [](double min, double max)
{
    return min + (max - min) * (static_cast<double>(std::rand()) / static_cast<double>(RAND_MAX));
};

Vector2 depot(0.0, 0.0);

// Populate the vector with random points
std::vector<Vector2> points;
for (std::size_t i = 0; i < SIZE; i++)
    points.emplace_back(frand(-MAX, MAX), frand(-MAX, MAX));
```

### Data initialization - TSPTW
```C++
// Map each previously created point to a timewindow
for (const auto& point : points)
{
    // Maybe check tstamp.cpp
    // for implementation details on the TStamp struct ("Timestamp")
    TStamp l(7, static_cast<uint8_t>(frand(15.0, 30.0)));
    TStamp r(8, static_cast<uint8_t>(frand(0.0,  15.0)));

    timewindows[point] = tsptw<Vector2>::Timewindow
    (
        static_cast<double>(l.seconds()),
        static_cast<double>(r.seconds())
    );
}
```

### TSP
```C++
try
{
    // Initialize the tsp<Vector2> class with 4 parameters
    // (1) The "depot" object
    // (2) The "city" objects
    // (3) A function returning the service time demanded by each "city" object
    // (4) A function returning the distance between two "city" objects
    tsp<Vector2> path
    (
        depot,
        points,
        [](const Vector2& v)
        {
            return 0.0;
        },
        [](const Vector2& A, const Vector2& B)
        {
            double xdiff = A.x() - B.x();
            double ydiff = A.y() - B.y();
            
            return xdiff * xdiff + ydiff * ydiff;
        }
    );

    // Used to find a quick and greedy solution
    path = path.nneighbour();

    // Used to optimize an already existing route
    path = path.opt2();

    // Greedy and local optimization methods rarely result in finding
    // the exact solution, but give good enough approximations of it.
    // sannealing is used in order to escape this local minima
    // and if possible reach optimality
    path = path.sannealing();

    // Used in conjuction with simulated annealing in order to optimize
    // the solution it returned with regards to its local neighbourhood
    path = path.opt2();
}
catch (std::exception& e)
{
    std::cerr << e.what() << std::endl;
}
```

### TSPTW
```C++
try
{
    // Initialize the tsp<Vector2> class with 6 parameters
    // (1) The "depot" object
    // (2) The "city" objects
    // (3) A function returning the service time demanded by each "city" object
    // (4) A function returning the distance between two "city" objects
    // (5) The departure time from the "depot" object
    // (6) A function returning the timewindow corresponding to
    //     the specified "city" object
    tsptw<Vector2> path
    (
        depot,
        points,
        [&depot](const Vector2& v)
        {
            return v == depot ? 0.0 : 30.0;
        },
        [](const Vector2& A, const Vector2& B)
        {
            double xdiff = A.x() - B.x();
            double ydiff = A.y() - B.y();
            
            return xdiff * xdiff + ydiff * ydiff;
        },
        TStamp(7, 30).seconds(),
        [&timewindows](const Vector2& point)
        {
            return timewindows[point];
        }
    );

    path = path.nneighbour();

    // A different flavor of simulated annealing that takes into consideration
    // the penalty i.e. the sum of all timewindow divergences as well as the cost
    // of the path
    // NOTICE:
    // if the original path is a feasible solution,
    // the compressed annealing algorithm guarantees a feasible solution
    path = path.cannealing();
}
catch (std::exception& e)
{
    std::cerr << e.what() << std::endl;
}
```

## Testing:
* average.sh: Given an executable, a string and a number of iterations, it runs the specified executable the specified amount of times and then calculates the average of the values denoted by the specified string.
* results.sh: A complimentary script that runs the average.sh script multiple times for different search strings.
Note:
``` bash
./average.sh ../bin/TSP "NN: " 10 results.txt 1
```
1 is passed as an arguement only in the first run of average.sh, resulting in the creation of a file named results.txt which contains the necessary info for the average.sh script to produce an output. Consecutive runs of average.sh process the same results.txt file