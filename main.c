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
  
    if( argc < 3){
        printf("Error: Arguments are not valid\n");
        exit(0);
    }
    
    fpIn = argv[2];

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


    /* Different Modes*/
    if( strlen(argv[1]) == 2  && argv[1][0] == '-'){

        if (argv[1][1] == 's'){
            /* Simulation Mode*/
            simulations(graph->nextNode, eventsHead);
            printf("\n\nWidest-Shortest:\n");
            printFT(graph);

            flag_sim = 'w';
            simulations(graph->nextNode, eventsHead);
            printf("\n\nShortest-Widest:\n");
            printFT(graph);
        }

        else if(argv[1][1] == 'i'){

            if( argc != 5){
                printf("Error: Arguments are not valid\n");
                exit(0);
            }

            /* Interactive simulation mode */
            tail = strtol(argv[3], &nb, 10); //souce
            head = strtol(argv[4], &nb, 10); ; //dest

            auxH = searchGraph(graph, head);
            simulationsInteractive(auxH, eventsHead);
            auxT = searchGraph(graph, tail);
            auxFT = searchDestiny(auxT->nextDest, head);

            printf("\nWidest-Shortest:\n");
            printf("From node %d to node %d -> width=%d and length=%d\n", tail, head, auxFT->cost_w, auxFT->cost_l);

            printf("\nShortest-Widest:\n");
            flag_sim = 'w';
            simulationsInteractive(auxH, eventsHead);
            printf("From node %d to node %d -> width=%d and length=%d\n", tail, head, auxFT->cost_w, auxFT->cost_l);
        }   
    }

    freeGraph(graph->nextNode);
    free(graph);

}