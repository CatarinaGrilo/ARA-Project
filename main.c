/******************************************************************************
 *
 * File Name: main.h
 * Authors: Ana Catarina Grilo & Margarida Fernandes
 *
 *****************************************************************************/
#include "graph.h"
#include "calendar.h"
#include "simulator.h"
#include "shortWidth.h"
#include "algorithms.h"


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

        //printf("%d %d %d %d:\n", tail, head, width, length);
        graph = createGraph(graph, tail, head, width, length);
    }
    fclose(fp);
    //printGraph(graph);



    /* Different Modes*/
    if( strlen(argv[1]) == 3  && argv[1][0] == '-'){

        if (argv[1][1] == 's'){
            if(argv[1][2] == 'l'){
                /* Simulation Mode*/
                simulations(graph->nextNode, eventsHead);
                printf("\n\nWidest-Shortest:\n\n");
                printFT(graph);
            }
            else if (argv[1][2] == 'w'){
                flag_sim = 'w';
                simulations(graph->nextNode, eventsHead);
                printf("\n\nShortest-Widest:\n");
                printFT(graph);
            }
            stats(graph);            
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

            if(argv[1][2] == 'l'){
                printf("\nWidest-Shortest:\n\n");
                simulationsInteractive(auxH, eventsHead);
                auxT = searchGraph(graph, tail);
                auxFT = searchDestiny(auxT->nextDest, head);
                printf("From node %d to node %d: width=%d and length=%d\nPath:%d->", tail, head, auxFT->cost_w, auxFT->cost_l, tail);
                printPath(auxT, head);
            }

            else if (argv[1][2] == 'w'){
                printf("\nShortest-Widest:\n");
                flag_sim = 'w';
                simulationsInteractive(auxH, eventsHead);
                auxT = searchGraph(graph, tail);
                auxFT = searchDestiny(auxT->nextDest, head);
                printf("From node %d to node %d: width=%d and length=%d\nPath:%d->", tail, head, auxFT->cost_w, auxFT->cost_l, tail);
                printPath(auxT, head);
            }
        }
    }  

    if( strlen(argv[1]) == 3  && argv[1][0] == '-' && argv[1][1] == 'a'){
         if (argv[1][2] == 'l'){  
            printf("\nWidest-Shortest:\n\n");
            algorithm(graph->nextNode, 'l');
            printFT_Algorithm(graph);
            stats(graph);
        }
        else if (argv[1][2] == 'w'){
            //Shortest Widest
            algorithm(graph->nextNode, 'w');
            printFT_Algorithm(graph);
            stats(graph);
        }
        else if (argv[1][2] == 'o'){
            //Shortest Widest optimal
            shortWidth(graph->nextNode);
            printFT_SW(graph);
            stats(graph);
        }
    }

    if( strlen(argv[1]) == 4  && argv[1][0] == '-' && argv[1][1] == 'a' && argv[1][2] == 'i'){
    
        if( argc != 5){
            printf("Error: Arguments are not valid\n");
            exit(0);
        }
        
        /* Interactive simulation mode */
        tail = strtol(argv[3], &nb, 10); //souce
        head = strtol(argv[4], &nb, 10); ; //dest

        auxH = searchGraph(graph, head);

        if(argv[1][3] == 'l'){
            printf("\nWidest-Shortest:\n\n");
            algorithmInteractive(auxH, 'l');
            auxT = searchGraph(graph, tail);
            auxFT = searchDestiny(auxT->nextDest, head);
            printf("From node %d to node %d: width=%d and length=%d\nPath:%d->", tail, head, auxFT->cost_w, auxFT->cost_l, tail);
            printPath(auxT, head);
        }

        else if (argv[1][3] == 'w'){
            printf("\nShortest-Widest:\n");
            algorithmInteractive(auxH, 'w');
            auxT = searchGraph(graph, tail);
            auxFT = searchDestiny(auxT->nextDest, head);
            printf("From node %d to node %d: width=%d and length=%d\nPath:%d->", tail, head, auxFT->cost_w, auxFT->cost_l, tail);
            printPath(auxT, head);
        }
    }
        
        


    freeGraph(graph->nextNode);
    free(graph);

}