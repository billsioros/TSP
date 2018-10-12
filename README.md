# The Travelling Salesman Problem in C++

## Algorithms:
* Nearest Neighbour
* Opt 2
* Simulated Annealing
* Compressed Annealing

## Testing:
* average.sh: Given an executable, a string and a number of iterations, it runs the specified executable the specified amount of times and then calculates the average of the values denoted by the specified string.
* results.sh: A complimentary script that runs the average.sh script multiple times for different search strings.
Note:
``` bash
./average.sh ../bin/TSP "NN: " 10 results.txt 1
```
1 is passed as an arguement only in the first run of average.sh, resulting in the creation of a file named results.txt which contains the necessary info for the average.sh script to produce an output. Consecutive runs of average.sh process the same results.txt file.

## Papers
* [Initialization methods for the TSP with Time Windows using Variable Neighborhood Search](https://www.researchgate.net/publication/296060410_Initialization_methods_for_the_TSP_with_Time_Windows_using_Variable_Neighborhood_Search)
* [A Compressed-Annealing Heuristic for the Traveling Salesman Problem with Time Windows](https://www.researchgate.net/publication/220669433_A_Compressed-Annealing_Heuristic_for_the_Traveling_Salesman_Problem_with_Time_Windows)
* [Improved K-Means Algorithm for Capacitated Clustering Problem](https://www.semanticscholar.org/paper/Improved-K-Means-Algorithm-for-Capacitated-Problem-EETHA/1b26ebd9f5ccbc51362c80e3339cdb79ea6a6669?tab=abstract)
* [An Effective Heuristic Algorithm for the Travelling-Salesman Problem](https://pubsonline.informs.org/doi/10.1287/opre.21.2.498)