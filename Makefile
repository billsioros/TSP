
PATH_BIN = ./bin

.PHONY: TSP
TSP:
	@echo "\n*** Compiling TSP ***"
	@echo "***"
	mkdir -p $(PATH_BIN)
	g++ $(DEFLAGS) -g3 -W -I include/ -std=c++14 src/tsp.cpp src/vector2.cpp -o $(PATH_BIN)/TSP
	@echo "***"

.PHONY: TSPTW
TSPTW:
	@echo "\n*** Compiling TSPTW ***"
	@echo "***"
	mkdir -p $(PATH_BIN)
	g++ -g3 -W -I include/ -std=c++14 src/tsptw.cpp src/vector2.cpp src/tstamp.cpp -o $(PATH_BIN)/TSPTW
	@echo "***"

.PHONY: test
test:
	make DEFLAGS="-D __TEST__"

.PHONY: clean
clean:
	@echo "\n*** Purging $(PATH_BIN) ***"
	@echo "***"
	rm -rv $(PATH_BIN)
	@echo "***"
