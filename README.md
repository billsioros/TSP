# The Travelling Salesman Problem in C++

## Issues
* TSP creates duplicate elements

## Compiling & Running:
```bash
make; ./bin/TSP
```

## Implemented Algorithms:
* Nearest Neighbour
* Multi Fragment
* Opt 2
* Simulated Annealing

## For testing purposes:
* average.sh: Given an executable, a string and a number of iterations, it runs the specified executable the specified amount of times and then calculates the average of the values denoted by the specified string.
* results.sh: A complimentary script that runs the average.sh script multiple times for different search strings.
Note:
``` bash
./average.sh ../bin/TSP "NN: " 10 results.txt 1
```
1 is passed as an arguement only in the first run of average.sh, resulting in the creation of a file named results.txt which contains the necessary info for the average.sh script to produce an output. Consecutive runs of average.sh process the same results.txt file.
