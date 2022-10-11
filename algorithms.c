/******************************************************************************
 *
 * File Name: algorithms.c
 * Authors: Ana Catarina Grilo & Margarida Fernandes
 *
 *****************************************************************************/

#include "algorithms.h"

#include "calendar.h"
#include "graph.h"



/**********************************     Algorithms     ***********************
 * 
 * 
 * 
 *****************************************************************************/

typedef struct{
    int dist;
    struct Node* destNode;
    struct PriorityQueue* next;

} PriorityQueue ;


typedef struct {
    
    int id;
    int cost_l;
    int cost_w;
    struct Node *hop;
    struct DistTable* next;
}  DistTable;


PriorityQueue* createElementPriorityQueue(int dist, Node *node){ 

    PriorityQueue *newElement;

    //Create a new element for Priority Queue
    if((newElement = (PriorityQueue*) calloc(1, sizeof(PriorityQueue))) == NULL){ 
		printf("It was not possible to allocate memory\n");
	    exit(1);
    }

    newElement->dist = dist;
    newElement->destNode = node;
    newElement->next = NULL;

    return newElement;
}

PriorityQueue* updatePriorityQueue(PriorityQueue *Head, Edge *edge){ 

    PriorityQueue *auxH, *auxT, *newElement;

    newElement = createElementPriorityQueue(edge->length, edge->destNode);

    if (Head == NULL){
        Head = newElement;
    }
    else{
        if(Head->dist > newElement->dist){
            newElement->next = Head;
            Head = newElement
        }
        else{
            auxH = Head;
            auxT = Head->next;
            if (auxT == NULL){
                Head->next = newElement;
            }
            else{
                while( (auxT!=NULL) && (auxT->dist > newElement->dist)){
                    auxH = auxT;
                    auxT = auxT->next;
                }
                newElement->next = auxT;
                auxH->next = newElement;
            }
        }
    }   

    printPriorityQueue(Head);

    return Head;
}


void printPriorityQueue(PriorityQueue* QueueHead){

    PriorityQueue* aux;
    Node *auxN;

    while(QueueHead!= NULL){
        aux = QueueHead;
        QueueHead = QueueHead->next;
        auxN = aux->destNode;
        printf("IN QUEUE: \n%d\n", auxN->id);
    }
}


void freePriorityQueue(PriorityQueue* QueueHead){

    PriorityQueue* aux;

    while(QueueHead!= NULL){
        aux = QueueHead;
        QueueHead = QueueHead->next;
        free(aux);
    }
}

DistTable* createDistTableEntry(Edge* edge){
    
    DistTable *newElement;

    //Create a new element for Priority Queue
    if((newElement = (DistTable*) calloc(1, sizeof(DistTable))) == NULL){ 
		printf("It was not possible to allocate memory\n");
	    exit(1);
    }

    newElement->id = edge->dest;
    newElement->cost_l = edge->length;
    newElement->cost_w = edge->width;
    newElement->hop = edge->destNode;
    newElement->next = NULL;

    return newElement;

}

DistTable* updateDistTable(DistTable* DistTableHead, Edge *auxE){

    if(DistTableHead == NULL){
        DistTableHead = createDistTableEntry(auxE);


        return DistTableHead;
    }

    return NULL;
}



void freeDistTable(DistTable* DistTableHead){

    DistTable* aux;

    while(DistTableHead!= NULL){
        aux = DistTableHead;
        DistTableHead = DistTableHead->next;
        free(aux);
    }
}



void algorithm(Node *nodeHead){

    PriorityQueue* QueueHead = NULL;
    PriorityQueue* auxQ = NULL;
    DistTable* DistTableHead = NULL;
    Node* auxN = NULL;
    Edge* auxE = NULL;
    int i = 0;

    if(nodeHead == NULL){
        return;
    }
    else{
        
        printf("\nID:%d\n", nodeHead->id);
        QueueHead = createElementPriorityQueue(0, nodeHead);
        printf("\n\nQUEUE HEADID:%d\n", nodeHead->id);
        //updatePriorityQueue(0, nodeHead);
        printf("\n\nHEY ALGORITHM\n");

        for(auxQ=QueueHead; auxQ!=NULL; auxQ=auxQ->next){
             
            auxN = auxQ->destNode;
            printf("ID IS %d\n\n\n ", auxN->id);
            for(auxE=auxN->nextEdgeIn; auxE!=NULL; auxE=auxE->nextEdge){
                printf("%d\t%d\t%d\t%d\n", auxE->dest, auxE->width, auxE->length, auxE->destNode->id);

                DistTableHead = updateDistTable(DistTableHead, auxE);
                QueueHead = updatePriorityQueue(QueueHead, auxE);
                
                
                printPriorityQueue(QueueHead);

            }

        
        }
        
        
        



        freeDistTable(DistTableHead);
        
        freePriorityQueue(QueueHead);


    }
}



