/******************************************************************************
 *
 * File Name: main.h
 * Authors: Ana Catarina Grilo & Margarida Fernandes
 *
 *****************************************************************************/

#include "graph.h"

int main(int argc, char *argv[]){

    int tail, head, width, length;
    char *fpIn;
    FILE *fp;
    Graph *graph=NULL;

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
    graph->nextNode = NULL;

    while(fscanf(fp, "%d %d %d %d\n", &tail, &head, &width, &length) != EOF ){

        printf("%d %d %d %d:\n", tail, head, width, length);
        graph = createGraph(graph, tail, head, width, length);
    }
    fclose(fp);

    printGraph(graph);

    freeGraph(graph->nextNode);
    free(graph);

}