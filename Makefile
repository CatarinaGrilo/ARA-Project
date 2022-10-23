CC = gcc
CFLAGS = -Wall

# Sources
SOURCES =  main.c graph.c calendar.c simulator.c algorithms.c shortWidth.c

VALG = valgrind --leak-check=full

graphs: $(SOURCES)
	$(CC) $(CFLAGS) $(SOURCES) -o graphs

