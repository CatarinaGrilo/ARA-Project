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
    struct Edge* nextEdgeOut;
    struct Edge* nextEdgeIn;
};

// Data structure to store a graph edge
struct Edge
{
    int dest;
    int width, length;
    struct Edge* nextEdge;
};

Graph *createGraph(Graph *listHead, int tail, int head, int width, int length){ 

    Node *newNodeT = NULL;
    Node *newNodeH = NULL;
    Edge *newEdgeT = NULL;
    Edge *newEdgeH = NULL;
    
    //For tail 
    newNodeT = searchGraph(listHead, tail);
    if (newNodeT == NULL){
        newNodeT = createNode(tail);
        listHead->nextNode = insertNode(listHead->nextNode, newNodeT);
    }

    newEdgeT = createEdge(head, width, length);
    newNodeT->nextEdgeOut= insertEdge(newNodeT->nextEdgeOut, newEdgeT);

    //for head
    newNodeH = searchGraph(listHead, head);
    if (newNodeH == NULL){
        newNodeH = createNode(head);
        listHead->nextNode = insertNode(listHead->nextNode, newNodeH);
    }

    newEdgeH = createEdge(tail, width, length);
    newNodeH->nextEdgeIn= insertEdge(newNodeH->nextEdgeIn, newEdgeH);

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
    newNode->nextEdgeOut = NULL;

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
        printf("Out\n");
        printf("Dest\tWidth\tLength\n");
        for(auxE=auxN->nextEdgeOut; auxE!=NULL; auxE=auxE->nextEdge){
            printf("%d\t%d\t%d\n", auxE->dest, auxE->width, auxE->length);
        }
        printf("In\n");
        printf("Dest\tWidth\tLength\n");
        for(auxE=auxN->nextEdgeIn; auxE!=NULL; auxE=auxE->nextEdge){
            printf("%d\t%d\t%d\n", auxE->dest, auxE->width, auxE->length);
        }
    }
}

void freeGraph(Node *listHead){

    Node *auxN;
    Edge *auxEHeadIn, *auxEIn;
    Edge *auxEHeadOut, *auxEOut;

    while(listHead!= NULL){
        auxN=listHead;
        auxEHeadIn = listHead->nextEdgeIn;
        while(auxEHeadIn!=NULL){
            auxEIn = auxEHeadIn;
            auxEHeadIn = auxEIn->nextEdge;
            free(auxEIn);
        }
        auxEHeadOut = listHead->nextEdgeOut;
        while(auxEHeadOut!=NULL){
            auxEOut = auxEHeadOut;
            auxEHeadOut = auxEOut->nextEdge;
            free(auxEOut);
        }

        listHead=listHead->nextNode;
        free(auxN); 
    }
}


