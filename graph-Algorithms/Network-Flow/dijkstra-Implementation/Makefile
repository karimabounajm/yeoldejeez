VacRouter.out : VaccineRouter.o VaccineRunner.o StackAr.o 
	g++ -ansi -Wall -g -o VacRouter.out VaccineRouter.o VaccineRunner.o  

StackAr.o : StackAr.cpp
	g++ -ansi -Wall -g -c StackAr.h	

VaccineRouter.o : VaccineRouter.cpp VaccineRouter.h StackAr.h 
	g++ -ansi -Wall -g -c VaccineRouter.cpp

VaccineRunner.o : VaccineRunner.cpp CPUTimer.h VaccineRunner.h VaccineRouter.h StackAr.h
	g++ -ansi -Wall -g -c VaccineRunner.cpp

clean : 
	rm -f VacRouter.out VaccineRouter.o  VaccineRunner.o StackAr.o