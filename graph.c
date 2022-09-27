/******************************************************************************
 *
 * File Name: graph.c
 * Authors: Ana Catarina Grilo & Margarida Fernandes
 *
 *****************************************************************************/

#include "graph.h"

#define N 4096 // Maximum number of nodes in the network 0-4095

// Data structure to store the graph, list of all nodes that exist in the network
struct Graph
{
    struct Node* nextNode;
    
};

// Data structure to store Edgeacency list nodes of the graph
struct Node
{
    int id;
    struct Node* nextNode;
    struct Edge* nextEdge;
};

// Data structure to store a graph edge
struct Edge
{
    int dest;
    int width, length;
    struct Edge* nextEdge;
};

Graph *createGraph(Graph *listHead, int tail, int head, int width, int length){ 

    Node *newNode = NULL;
    Edge *newEdge = NULL;
    
    
    newNode = searchGraph(listHead, tail);
    if (newNode == NULL){
        newNode = createNode(tail);
        listHead->nextNode = insertNode(listHead->nextNode, newNode);
    }

    newEdge = createEdge(head, width, length);
    newNode->nextEdge= insertEdge(newNode->nextEdge, newEdge);

    return listHead;
}

Node *searchGraph(Graph *listHead, int id){

    Node *auxT = listHead->nextNode;

    if(auxT == NULL){
        return NULL;
    }else{
        if(auxT->id == id){
            return auxT;
        }
        while(auxT->nextNode !=NULL){
            auxT = auxT->nextNode;
            if( auxT->id == id ){
                return auxT;
            }
        }
    }
    return NULL;
}

Node *createNode(int tail){

    Node *newNode = NULL;
    
    if((newNode = (Node*) calloc(1, sizeof(Node))) == NULL){ 
        printf("It was not possible to allocate memory\n");
	    exit(1);
    } 
    
    newNode->id = tail;
    newNode->nextNode = NULL;
    newNode->nextEdge = NULL;

    return newNode;
}

Node *insertNode(Node *listHead, Node *newNode){ //Insert the new Node in the end of the Nodes List

    Node *auxH, *auxT;

    if(listHead == NULL){
        listHead = newNode;
    }else{
        auxH = listHead;
        auxT = listHead->nextNode;
        while(auxT != NULL){
            auxH = auxT;
            auxT = auxT->nextNode;
        }
        auxH->nextNode= newNode;
    }
    return listHead;
}

Edge *createEdge(int head, int width, int length){ 

    Edge *newEdge;

    if((newEdge = (Edge*) calloc(1, sizeof(Edge))) == NULL){ //Create a new Edgeacent
		printf("It was not possible to allocate memory\n");
	    exit(1);
    }

    newEdge->dest = head;
    newEdge->width = width;
    newEdge->length = length;
    newEdge->nextEdge = NULL;

    return newEdge;
}

Edge *insertEdge(Edge *listHead, Edge *newEdge){ 

    Edge *auxH, *auxT;

    if(listHead == NULL){
        listHead = newEdge;
    }else{
        auxH = listHead;
        auxT = listHead->nextEdge;
        while(auxT != NULL){
            auxH = auxT;
            auxT = auxT->nextEdge;
        }
        auxH->nextEdge = newEdge;
    }
    return listHead;
}

void printGraph(Graph *Head){

    Node *auxN; 
    Edge *auxE;

    for(auxN=Head->nextNode; auxN!=NULL; auxN=auxN->nextNode){
        printf("Edges of Node %d:\n", auxN->id);
        printf("Dest\tWidth\tLength\n");
        for(auxE=auxN->nextEdge; auxE!=NULL; auxE=auxE->nextEdge){
            printf("%d\t%d\t%d\n", auxE->dest, auxE->width, auxE->length);
        }
    }
}

void freeGraph(Node *listHead){

    Node *auxN;
    Edge *auxEHead, *auxE;

    while(listHead!= NULL){
        auxN=listHead;
        auxEHead = listHead->nextEdge;
        while(auxEHead!=NULL){
            auxE = auxEHead;
            auxEHead = auxE->nextEdge;
            free(auxE);
        }

        listHead=listHead->nextNode;
        free(auxN); 
    }
}

