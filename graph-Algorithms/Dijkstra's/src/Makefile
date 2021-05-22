spreader.out : minheap.o spreader.o spreaderRunner.o 
	g++ -ansi -Wall -g -o spreader.out spreader.o spreaderRunner.o  

minheap.o : minheap.cpp
	g++ -ansi -Wall -g -c minheap.h	

spreader.o : spreader.cpp spreader.h minheap.h
	g++ -ansi -Wall -g -c spreader.cpp

spreaderRunner.o : spreaderRunner.cpp CPUTimer.h spreaderRunner.h spreader.h minheap.h
	g++ -ansi -Wall -g -c spreaderRunner.cpp

clean : 
	rm -f spreader.out spreader.o spreaderRunner.o minheap.o

