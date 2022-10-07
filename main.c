/******************************************************************************
 *
 * File Name: main.h
 * Authors: Ana Catarina Grilo & Margarida Fernandes
 *
 *****************************************************************************/
#include "graph.h"
#include "calendar.h"
#include "simulator.h"


float time_simul = 0; //

char flag_sim = 'l'; //flag to type of simulation, l->widests-shortest, w->shortest-widest 

int main(int argc, char *argv[]){

    int tail, head, width, length;
    char *fpIn;
    FILE *fp;
    Graph *graph=NULL;
    Event *eventsHead = NULL;

    fpIn = argv[1];

    fp = fopen(fpIn, "r");
    if (fp == (FILE*) NULL) {
        printf("Error: File not found\n");
        exit(0);
    }

    if((graph = (Graph*) calloc(1, sizeof(Graph))) == NULL){ 
        printf("It was not possible to allocate memory\n");
	    exit(1);
    }

    while(fscanf(fp, "%d%*c%d%*c%d%*c%d\n", &tail, &head, &width, &length) != EOF ){

        printf("%d %d %d %d:\n", tail, head, width, length);
        graph = createGraph(graph, tail, head, width, length);
    }
    fclose(fp);
    //printGraph(graph);
    
    simulations(graph->nextNode, eventsHead);
    printf("\n\nWidest-Shortest:\n");
    printFT(graph);

    flag_sim = 'w'; //We need to see this, graphs-line 297, conditions for shortest-widest
    simulations(graph->nextNode, eventsHead);
    printf("\n\nShortest-Widest:\n");
    printFT(graph);

    freeGraph(graph->nextNode);
    free(graph);

}