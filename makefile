all:
	g++ -Wall -o main main.cpp DataOSAP.cpp
clean: 
	$(RM) main