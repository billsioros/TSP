
PATH_BIN = ./bin

.PHONY: all
all:
	@echo "\n*** Compiling TSP ***"
	@echo "***"
	mkdir -p ./bin
	g++ $(DEFLAGS) -g3 -W -I include/ -std=c++14 src/main.cpp src/vector2.cpp -o $(PATH_BIN)/TSP
	@echo "***"

.PHONY: test
test:
	make DEFLAGS="-D __TEST__"

.PHONY: clean
clean:
	@echo "\n*** Purging TSP ***"
	@echo "***"
	rm -rv $(PATH_BIN)
	@echo "***"
