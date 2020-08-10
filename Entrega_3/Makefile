flags=-Wno-deprecated -g -ggdb
comp=g++
prog=tsp_ts_greedy

maxIterations = 1000000
maxTime = 7200
intervalMin = 2.0
intervalMax = 2.0
intervalLength = 15
SEEDS = 22

debug=0

exe:$(prog)
	#valgrind --leak-check=full --show-reachable=yes 
	./$(prog) $(maxIterations) $(maxTime) $(intervalMin) $(intervalMax) $(intervalLength) $(SEEDS)

$(prog): tsp_ts_greedy.cpp
	$(comp) $(prog).cpp -o $(prog)



clean:
	rm -f *~
	rm -f *.o
	rm -f $(prog)
	rm -f core*

