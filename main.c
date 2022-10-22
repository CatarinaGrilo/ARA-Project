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


Graph *readFile(char *argv){
    
    int tail, head, width, length, nodes;
    char *fpIn;
    FILE *fp = NULL; 
    Graph *graph = NULL;

    fpIn = argv;
    fp = fopen(fpIn, "r");
    if (fp == (FILE*) NULL) {
        printf("Error: File not found\n");
        return NULL;
    }

    if((graph = (Graph*) calloc(1, sizeof(Graph))) == NULL){ 
        printf("It was not possible to allocate memory\n");
	    return NULL;
    }
    if(fscanf(fp, "%d\n", &nodes) == EOF )
        return NULL;
    while(fscanf(fp, "%d%*c%d%*c%d%*c%d\n", &tail, &head, &width, &length) != EOF ){

        //printf("%d %d %d %d:\n", tail, head, width, length);
        graph = createGraph(graph, tail, head, width, length);
    }

    graph = createGraph(graph, tail, head, width, length);
    fclose(fp);
    //printGraph(graph);

    return graph;
}


int main(int argc, char *argv[]){

    int tail, head;
    char *nb;
    Graph *graph = NULL;
    Node *auxT = NULL, *auxH = NULL;
    ForwardTable *auxFT = NULL;
    Event *eventsHead = NULL;
  
    if( argc < 3){
        printf("\n\nError: Arguments are not valid\n");
        printf("To run the program, use the following arguments:\n");
        printf("\n ./[Executable] [File.txt] -[simulation/algorithm][mode]\n");
        printf("\n Simulation mode (widest-shortest): ./graphs example.txt -sl");
        printf("\n Simulation mode (shortest-widest): ./graphs example.txt -sw");
        printf("\n Interactive Simulation mode (widest-shortest): ./graphs example.txt -sil [Source] [Destiny] ");
        printf("\n Interactive Simulation mode (shortest-widest): ./graphs example.txt -siw [Source] [Destiny] ");
        printf("\n Algorithm mode (widest-shortest):  ./graphs example.txt -al");
        printf("\n Algorithm mode (shortest-widest):  ./graphs example.txt -aw");
        printf("\n Interactive Simulation mode (widest-shortest): ./graphs example.txt -ail [Source] [Destiny] ");
        printf("\n Interactive Simulation mode (shortest-widest): ./graphs example.txt -aiw [Source] [Destiny] ");
        printf("\n Optimal Algorithm mode for shortest-widest:  ./graphs example.txt -ao\n\n");
        exit(0);
    }
    

    graph = readFile(argv[1]);
    if(graph == NULL){
        printf("\n Error on creating the graph.\n Exiting...");
        exit(0);
    }


    /* Different Modes*/

    /* Simulation of a QoS vectoring protocol */ 
    if( strlen(argv[2]) == 3 && argv[2][0] == '-' && argv[2][1] == 's'){      
        //Widest Shortest
        if(argv[2][2] == 'l'){
            printf("\n\nWidest-Shortest:\n\n");
            simulations(graph->nextNode, eventsHead);     
            printFT(graph);
            stats(graph);  
        }
        //Shortest Widest 
        else if (argv[2][2] == 'w'){
            flag_sim = 'w';
            printf("\n\nShortest-Widest:\n");
            simulations(graph->nextNode, eventsHead);
            printFT(graph);
            stats(graph);  
        }
        else{
            printf("\nInvalid arguments...\n Try using -sl or -sw\n");
        }              
    }
    /* Interactive simulation mode */
    else if(argc == 5 && strlen(argv[2]) == 4  && argv[2][0] == '-' && argv[2][1] == 's' && argv[2][2] == 'i'){
        
        tail = strtol(argv[3], &nb, 10); // source
        head = strtol(argv[4], &nb, 10); ; // destiny
        auxH = searchGraph(graph, head);

        //Widest Shortest
        if(argv[2][3] == 'l'){
            printf("\nWidest-Shortest:\n\n");
            simulationsInteractive(auxH, eventsHead);
            auxT = searchGraph(graph, tail);
            auxFT = searchDestiny(auxT->nextDest, head);
            printf("From node %d to node %d: width=%d and length=%d\nPath:%d->", tail, head, auxFT->cost_w, auxFT->cost_l, tail);
            printPath(auxT, head);
        }
        //Shortest Widest 
        else if (argv[2][3] == 'w'){
            printf("\nShortest-Widest:\n");
            flag_sim = 'w';
            simulationsInteractive(auxH, eventsHead);
            auxT = searchGraph(graph, tail);
            auxFT = searchDestiny(auxT->nextDest, head);
            printf("From node %d to node %d: width=%d and length=%d\nPath:%d->", tail, head, auxFT->cost_w, auxFT->cost_l, tail);
            printPath(auxT, head);
        }
        else{
            printf("\nInvalid arguments...\n Try using -sil or -siw\n");
        }
    }
    /* Stable State Algorithm */
    else if( strlen(argv[2]) == 3  && argv[2][0] == '-' && argv[2][1] == 'a'){
        // Widest Shortest
        if (argv[2][2] == 'l'){  
            printf("\nWidest-Shortest:\n\n");
            algorithm(graph->nextNode, 'l');
            printFT_SW(graph);
            stats(graph);
        }
        //Shortest Widest
        else if (argv[2][2] == 'w'){
            algorithm(graph->nextNode, 'w');
            printFT_SW(graph);
            stats(graph);
        }
        //Shortest Widest optimal
        else if (argv[2][2] == 'o'){
            shortWidth(graph->nextNode);
            printFT_SW(graph);
            stats(graph);
        }
        else{
            printf("\nInvalid arguments...\n Try using -al or -aw or -ao\n");
        }
    }
    /* Algorithm interactive mode */
    else if(argc == 5 && strlen(argv[2]) == 4  && argv[2][0] == '-' && argv[2][1] == 'a' && argv[2][2] == 'i'){
        
        tail = strtol(argv[3], &nb, 10); // source
        head = strtol(argv[4], &nb, 10); ; // destiny
        auxH = searchGraph(graph, head);

        // Widest Shortest
        if(argv[2][3] == 'l'){
            printf("\nWidest-Shortest:\n\n");
            algorithmInteractive(auxH, 'l');
            auxT = searchGraph(graph, tail);
            auxFT = searchDestiny(auxT->nextDest, head);
            printf("From node %d to node %d: width=%d and length=%d\nPath: %d -> ", tail, head, auxFT->cost_w, auxFT->cost_l, tail);
            printPath(auxT, head);
        }
        //Shortest Widest
        else if (argv[2][3] == 'w'){
            printf("\nShortest-Widest:\n");
            algorithmInteractive(auxH, 'w');
            auxT = searchGraph(graph, tail);
            auxFT = searchDestiny(auxT->nextDest, head);
            printf("From node %d to node %d: width=%d and length=%d\nPath: %d -> ", tail, head, auxFT->cost_w, auxFT->cost_l, tail);
            printPath(auxT, head);
        }        
        else{
            printf("\nInvalid arguments...\n Try using -ail or -aiw\n");
        }
    }
    else{
        printf("\n\nError: Arguments are not valid\n");
        printf("To run the program, use the following arguments:\n");
        printf("\n ./[Executable] [File.txt] -[simulation/algorithm][mode]\n");
        printf("\n Simulation mode (widest-shortest): ./graphs example.txt -sl");
        printf("\n Simulation mode (shortest-widest): ./graphs example.txt -sw");
        printf("\n Interactive Simulation mode (widest-shortest): ./graphs example.txt -sil [Source] [Destiny] ");
        printf("\n Interactive Simulation mode (shortest-widest): ./graphs example.txt -siw [Source] [Destiny] ");
        printf("\n Algorithm mode (widest-shortest):  ./graphs example.txt -al");
        printf("\n Algorithm mode (shortest-widest):  ./graphs example.txt -aw");
        printf("\n Interactive Simulation mode (widest-shortest): ./graphs example.txt -ail [Source] [Destiny] ");
        printf("\n Interactive Simulation mode (shortest-widest): ./graphs example.txt -aiw [Source] [Destiny] ");
        printf("\n Optimal Algorithm mode for shortest-widest:  ./graphs example.txt -ao\n\n");
        exit(0);
    }
        

    freeGraph(graph->nextNode);
    free(graph);

    return 0;

}