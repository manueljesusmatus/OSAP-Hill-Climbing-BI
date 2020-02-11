flags=-Wno-deprecated -g -ggdb
comp=g++
prog=HC+Restart

#El problema
config=Instances/burma14.tsp
resultados=Out.txt
semilla=123
restarts=1000
debug=0

comp:$(prog)

exe:$(prog)
	#valgrind --leak-check=full --show-reachable=yes 
	./$(prog) $(config) $(resultados) $(semilla) $(restarts) $(debug)

$(prog): main.o
	$(comp) main.o -o $(prog) $(flags)

main.o: main.cpp includes.h globales.h
	$(comp) -c main.cpp $(flags)

clean:
	rm -f *~
	rm -f *.o
	rm -f $(prog)
	rm -f core*

