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


// typedef struct {
    
//     int id;
//     int cost_l;
//     int cost_w;
//     struct Node *NextHop;
//     struct DistTable* next;
// }  DistTable;

int min(int a, int b){

    return (a > b) ? b : a;
}



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

    PriorityQueue *auxH, *auxT, *newElement, *afterHead = NULL;
    Node * aux = edge->destNode;
    
    newElement = createElementPriorityQueue(aux->distTable->cost_l, edge->destNode);
    // printf("\n\n\n\n\n\n QUEUE A ANALISAR AGORA \n");
    // printPriorityQueue(Head);

    if (Head == NULL){
        return newElement;
    }
    else if(Head->next == NULL){
        Head->next = newElement;
        return Head;
    }
    
    afterHead = Head->next;

    if(afterHead->dist > newElement->dist){
        newElement->next = afterHead;
        Head->next = newElement;
        return Head;
    }
    else{
        auxH = afterHead;
        auxT = afterHead->next;
        if (auxT == NULL){
            afterHead->next = newElement;
        }
        else{
            while( (auxT!=NULL) && (auxT->dist < newElement->dist)){
                auxH = auxT;
                auxT = auxT->next;
            }
            newElement->next = auxT;
            auxH->next = newElement;
        }
    }

   return Head;
}


void printPriorityQueue(PriorityQueue* QueueHead){

    PriorityQueue* aux;
    Node *auxN;
    
    printf("=================================================\n QUEUE: \n");

    while(QueueHead!= NULL){
        aux = QueueHead;
        QueueHead = QueueHead->next;
        auxN = aux->destNode;
        printf("\nID: %d with distance: %d\n", auxN->id, aux->dist);
    }

    printf("=================================================\n");
}


void freePriorityQueue(PriorityQueue* QueueHead){

    PriorityQueue* aux;

    while(QueueHead!= NULL){
        aux = QueueHead;
        QueueHead = QueueHead->next;
        free(aux);
    }
}

DistTable* createDistTableEntry(Edge* edge, Node* gotoNode, Node* nodeitself){
    
    DistTable *newElement;
    DistTable* destiny = gotoNode->distTable;


    //Create a new element for Priority Queue
    if((newElement = (DistTable*) calloc(1, sizeof(DistTable))) == NULL){ 
		printf("It was not possible to allocate memory\n");
	    exit(1);
    }

    printf("\n createDistTableEntry nó vizinho: %d \n\n\n", nodeitself->id);

    newElement->id = edge->dest;
    newElement->cost_l = edge->length + destiny->cost_l;
    newElement->cost_w = min(edge->width, destiny->cost_w);
    newElement->NextHop = gotoNode;
    newElement->next = NULL;
    nodeitself->distTable = newElement;

    //printf("\n createDistTableEntry nó vizinho: %d \n\n\n", nodeitself->id);
    printf("DENTrO DISTO id:%d l:%d  w:%d \n", newElement->id, newElement->cost_l, newElement->cost_w);

    return newElement;

}
    
DistTable* createSource (Node* node){
    
    DistTable *newElement;

    //Create a new element for Priority Queue
    if((newElement = (DistTable*) calloc(1, sizeof(DistTable))) == NULL){ 
		printf("It was not possible to allocate memory\n");
	    exit(1);
    }

    newElement->id = node->id;
    newElement->cost_l = 0;
    newElement->cost_w = 99999;
    newElement->NextHop = 0;
    newElement->next = NULL;
    node->distTable = newElement;

    return newElement;

}

DistTable* searchEntry(DistTable* DistTableHead, int id){

    while(DistTableHead->next != NULL){
        if (DistTableHead->id == id){
            return DistTableHead;
        }
        DistTableHead = DistTableHead->next;
    }

    return DistTableHead;
}

//DistTable* insertEntryintheEnd(DistTable* DistTableHead)

DistTable* updateDistTable(DistTable* DistTableHead, Edge *auxE, Node* gotoNode, Node* nodeitself){

    DistTable * newElement = NULL, *aux = NULL;

    // if(DistTableHead->next == NULL){
    //     DistTableHead = createDistTableEntry(DistTableHead, auxE, gotoNode);
    //     return DistTableHead;
    // }else{
        
    // }

    aux = searchEntry(DistTableHead, auxE->dest);
    if (aux->next == NULL){
        printf("\nNAO ENCONTREI O 6\n");
        aux->next = createDistTableEntry(auxE, gotoNode, nodeitself);
        return DistTableHead;
    }



    return DistTableHead;
}

void freeDistTable(DistTable* DistTableHead){

    DistTable* aux;

    while(DistTableHead!= NULL){
        aux = DistTableHead;
        DistTableHead = DistTableHead->next;
        free(aux);
    }
}

void printDistTable(DistTable* DistTableHead){

    DistTable* aux;
    Node *auxN;

    printf("------------------------------------------------\nIn DIST TABLE: \n");

    while(DistTableHead != NULL){
        aux = DistTableHead;
        DistTableHead = DistTableHead->next;
        auxN = aux->NextHop;
        if (auxN == NULL){
            printf("\nid: %d\t length: %d  width: %d\n", aux->id, aux->cost_l, aux->cost_w );
        }
        else{
            printf("\nid: %d\t length: %d  width: %d Next HOP: %d\n", aux->id, aux->cost_l, aux->cost_w, auxN->id );
        }
    }

    printf("------------------------------------------------\n\n\n\n");
}



void algorithm(Node *nodeHead){

    PriorityQueue* QueueHead = NULL;
    PriorityQueue* auxQ = NULL;
    DistTable* DistTableHead = NULL, *auxD;
    Node* auxN = NULL;
    Edge* auxE = NULL;
    int i = 0;

    if(nodeHead == NULL){
        return;
    }
    else{
        
        printf("\nID:%d\n", nodeHead->id);
        DistTableHead = createSource(nodeHead);
        QueueHead = createElementPriorityQueue(0, nodeHead);
        //nodeHead->distTable = DistTableHead;
        // auxD = nodeHead->distTable;
        // printf("IS TABLE WORKING? %d\n\n\n ", auxD->id );
        printf("\n\nQUEUE HEADID:%d\n", nodeHead->id);
        //updatePriorityQueue(0, nodeHead);
        printf("\n\nHEY ALGORITHM\n");

        //for(auxQ=QueueHead; auxQ!=NULL; auxQ=auxQ->next){
        //while(QueueHead != NULL){  
        printPriorityQueue(QueueHead);
        printDistTable(DistTableHead);

        //while(i != 6){
        while(QueueHead != NULL){ 
            auxQ = QueueHead;
            auxN = auxQ->destNode;
            printf("ID IS %d\n\n\n ", auxN->id);
            for(auxE=auxN->nextEdgeIn; auxE!=NULL; auxE=auxE->nextEdge){
                printf("%d\t%d\t%d\t%d\n", auxE->dest, auxE->width, auxE->length, auxE->destNode->id);
                
                printf("\nNó a analisar %d \n", auxN->id);
                if (auxE->dest !=nodeHead->id){
                    DistTableHead = updateDistTable(DistTableHead, auxE, auxN, auxE->destNode);
                    QueueHead = updatePriorityQueue(QueueHead, auxE);
                    
                    printf("\nTerceiro\n");
                    printPriorityQueue(QueueHead);
                    printDistTable(DistTableHead);
                }

            }
            QueueHead = QueueHead->next;
            free(auxQ);
            i++;
        }
        
        printf("\nENDDDDDDDDDDDDDDD\n");

        printPriorityQueue(QueueHead);



        freeDistTable(DistTableHead);
        
        freePriorityQueue(QueueHead);


    }
}



