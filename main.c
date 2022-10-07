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
    char *fpIn, *nb;
    FILE *fp;
    Graph *graph=NULL;
    Node *auxT=NULL;
    Node *auxH=NULL;
    ForwardTable *auxFT=NULL;
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
    
    //Simulation mode 
    simulations(graph->nextNode, eventsHead);
    printf("\n\nWidest-Shortest:\n");
    printFT(graph);

    flag_sim = 'w'; //We need to see this, graphs-line 297, conditions for shortest-widest
    simulations(graph->nextNode, eventsHead);
    printf("\n\nShortest-Widest:\n");
    printFT(graph);

    //Iteractive simulation mode
    /*tail = strtol(argv[2], &nb, 10); //souce
    head = strtol(argv[3], &nb, 10); ; //dest

    auxH = searchGraph(graph, head);
    simulationsIteractive(auxH, eventsHead);
    auxT = searchGraph(graph, tail);
    auxFT = searchDestiny(auxT->nextDest, head);

    printf("\nWidest-Shortest:\n");
    printf("From node %d to node %d -> width=%d and length=%d\n", tail, head, auxFT->cost_w, auxFT->cost_l);

    printf("\nShortest-Widest:\n");
    flag_sim = 'w';
    simulationsIteractive(auxH, eventsHead);
    printf("From node %d to node %d -> width=%d and length=%d\n", tail, head, auxFT->cost_w, auxFT->cost_l);*/

    freeGraph(graph->nextNode);
    free(graph);

}