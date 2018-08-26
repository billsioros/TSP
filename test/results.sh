#!/bin/bash

./average.sh ../bin/TSP "NN: "    100 results.txt 1
./average.sh ../bin/TSP "OPT21: " 100 results.txt
./average.sh ../bin/TSP "SA: "    100 results.txt
./average.sh ../bin/TSP "OPT22: " 100 results.txt
