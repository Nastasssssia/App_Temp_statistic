all: temperature_stats.exe

temperature_stats.exe: main.o temp_functions.o
	gcc -o temperature_stats main.o temp_functions.o

main.o: main.c temp_functions.h
	gcc -c -o main.o main.c

temp_functions.o: temp_functions.c
	gcc -c -o temp_functions.o temp_functions.c	


clean:
	del *.o
	del *.exe
