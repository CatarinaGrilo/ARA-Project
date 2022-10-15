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
    int width;
    struct Node* destNode;
    struct Node* neighbour;
    struct PriorityQueue* next;

} PriorityQueue ;


int min(int a, int b){

    return (a > b) ? b : a;
}

PriorityQueue* createElementPriorityQueue(int dist, int width, Node *node, Node *neighbour){ 

    PriorityQueue *newElement;

    //Create a new element for Priority Queue
    if((newElement = (PriorityQueue*) calloc(1, sizeof(PriorityQueue))) == NULL){ 
		printf("It was not possible to allocate memory\n");
	    exit(1);
    }

    newElement->dist = dist;
    newElement->width = width;
    newElement->destNode = node;
    newElement->neighbour = neighbour;
    newElement->next = NULL;

    return newElement;
}

PriorityQueue* updatePriorityQueue_l(PriorityQueue *Head, Edge *edge, Node *neighbour){ 

    PriorityQueue *auxH, *auxT, *newElement, *afterHead = NULL;
    Node * aux = edge->destNode;
    int length, width;

    length = edge->length + neighbour->nextDest->cost_l;
    width = min(edge->width, neighbour->nextDest->cost_w);
    
    newElement = createElementPriorityQueue(length, width , edge->destNode, neighbour);
    // printf("\n\n\n\n\n\n QUEUE A ANALISAR AGORA \n");
    // printPriorityQueue(Head);


    /* Inserir nó ordenadamente*/

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
        //printf("\n\n\n 1\n\n\n");
        return Head;
    }
    else{
        auxH = afterHead;
        auxT = afterHead->next;
        if (auxT == NULL){
            if ((auxH->destNode == newElement->destNode) && (auxH->dist <= newElement->dist)){
                //printf("\n\n\n 2\n\n\n");
                free(newElement);
                return Head;
            }
            afterHead->next = newElement;
        }
        else{
            while( (auxT!=NULL) && (auxT->dist < newElement->dist)){
                auxH = auxT;
                auxT = auxT->next;
            }
            //printf("\n\n\n 3\n\n\n");
            newElement->next = auxT;
            auxH->next = newElement;
        }
    }

   return Head;
}

PriorityQueue* updatePriorityQueue_w(PriorityQueue *Head, Edge *edge, Node *neighbour){ 

    PriorityQueue *auxH, *auxT, *newElement, *afterHead = NULL;
    Node * aux = edge->destNode;
    int length, width;

    length = edge->length + neighbour->nextDest->cost_l;
    width = min(edge->width, neighbour->nextDest->cost_w);
    
    newElement = createElementPriorityQueue(length, width , edge->destNode, neighbour);
    // printf("\n\n\n\n\n\n QUEUE A ANALISAR AGORA \n");
    // printPriorityQueue(Head);


    /* Inserir nó ordenadamente*/

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
        printf("\n\n\n 1\n\n\n");
        return Head;
    }
    else{
        auxH = afterHead;
        auxT = afterHead->next;
        if (auxT == NULL){
            if ((auxH->destNode == newElement->destNode) && (auxH->dist <= newElement->dist)){
                printf("\n\n\n 2\n\n\n");
                free(newElement);
                return Head;
            }
            afterHead->next = newElement;
        }
        else{
            while( (auxT!=NULL) && (auxT->dist < newElement->dist)){
                auxH = auxT;
                auxT = auxT->next;
            }
            printf("\n\n\n 3\n\n\n");
            newElement->next = auxT;
            auxH->next = newElement;
        }
    }

   return Head;
}

void printPriorityQueue(PriorityQueue* QueueHead){

    PriorityQueue* aux;
    Node *auxN, *viz;
    
    printf("=================================================\n QUEUE: \n");

    while(QueueHead!= NULL){
        aux = QueueHead;
        QueueHead = QueueHead->next;
        auxN = aux->destNode;
        if (aux->neighbour == NULL){
            printf("\nID: %d    distance: %d    width %d  \n", auxN->id, aux->dist, aux->width );
        }
        else{
            viz = aux->neighbour;
            printf("\nID: %d    distance: %d    width %d  neighbour:%d \n", auxN->id, aux->dist, aux->width , viz->id);
        }
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
    
void createDestinysFT(Node* nodeHead){

    ForwardTable *newElement;

    while (nodeHead != NULL){

        if((newElement = (ForwardTable*) calloc(1, sizeof(ForwardTable))) == NULL){ 
            printf("It was not possible to allocate memory\n");
            exit(1);
        }

        newElement->dest = nodeHead->id;
        newElement->cost_l = 0;
        newElement->cost_w = 1000000;
        newElement->nextHop = 0;
        newElement->nextDest = NULL;

        nodeHead->nextDest = newElement;
        nodeHead->visited = 0;

        nodeHead = nodeHead->nextNode;
    }

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

void resetNodeVisited(Node *Head){

    while (Head != NULL){
        Head->visited = 0;
        Head = Head->nextNode;
    }
}

void printForwardTable(Node *Head){

    Node *auxN, *auxHOP; 
    ForwardTable *auxFT;

    for(auxN=Head; auxN!=NULL; auxN=auxN->nextNode){
        printf("Forwarding Table Node %d:\n", auxN->id);
        printf("Dest\tWidth\tLength\tNextHop\n");
        for(auxFT=auxN->nextDest; auxFT!=NULL; auxFT=auxFT->nextDest){
            auxHOP = auxFT->nextHop;
            if (auxHOP == NULL){
                printf("%d\t%d\t%d\t%d\n", auxFT->dest, auxFT->cost_w, auxFT->cost_l, auxHOP);
            }else{
                printf("%d\t%d\t%d\t%d\n", auxFT->dest, auxFT->cost_w, auxFT->cost_l, auxHOP->id);
            }
        }
    }
}

void updateForwardTable_a(Node *node, PriorityQueue *element, Node *nextHop, Node *destiny){

    ForwardTable *newElement;

    if((newElement = (ForwardTable*) calloc(1, sizeof(ForwardTable))) == NULL){ 
        printf("It was not possible to allocate memory\n");
        exit(1);
    }

    newElement->dest = destiny->id;
    newElement->cost_l = element->dist;
    newElement->cost_w = element->width;
    newElement->nextHop = nextHop;
    newElement->nextDest = NULL;

    if(node->nextDest == NULL){
        node->nextDest = newElement;
    }
    else{
        newElement->nextDest = node->nextDest;
        node->nextDest = newElement;
    }
}

void algorithm(Node *nodeHead, char mode){

    PriorityQueue* QueueHead = NULL;
    PriorityQueue* auxQ = NULL;
    Node* auxN = NULL, *node = nodeHead;
    Edge* auxE = NULL;
    int i = 0;

    if(nodeHead == NULL){
        return;
    }
    else{
        
        
        for(node = nodeHead; node != NULL; node = node->nextNode){
      
            resetNodeVisited(nodeHead);
            QueueHead = createElementPriorityQueue(0, 100000, node, NULL);
            
            // printf("\n\n\n\n VELHOOOOOOOOOOOo \n\n\n\n\n");
            // printf("\nID:%d\n", node->id);
            // printPriorityQueue(QueueHead);


            while(QueueHead != NULL){ 
                auxQ = QueueHead;
                auxN = auxQ->destNode;

                if (auxN->visited != 1){
                    
                    auxN->visited = 1; 
                    updateForwardTable_a(auxN, QueueHead, QueueHead->neighbour, node);
                    
                    // printForwardTable(nodeHead);
                    // printf("ID IS %d\n\n\n ", auxN->id);

                    for(auxE=auxN->nextEdgeIn; auxE!=NULL; auxE=auxE->nextEdge){

                        // printf("%d\t%d\t%d\t%d\n", auxE->dest, auxE->width, auxE->length, auxE->destNode->id);          
                        // printf("\nNó a analisar %d \n", auxN->id);
                        // printf("\nANTES\n");
                        // printPriorityQueue(QueueHead);

                        if (mode == 'l'){
                            QueueHead = updatePriorityQueue_l(QueueHead, auxE, auxN);
                        }
                        else if(mode == 'w'){
                            QueueHead = updatePriorityQueue_w(QueueHead, auxE, auxN);
                        }


                        // printf("\n DEPOIS \n");
                        // printPriorityQueue(QueueHead);
                        // printf("\n DEPOIS DEPOIS \n");
                        // printForwardTable(nodeHead);

                    }
                }
                
                QueueHead = QueueHead->next;
                free(auxQ);
                
            }
            
            // printf("\nENDDDDDDDDDDDDDDD\n");
            // printPriorityQueue(QueueHead);
            // printForwardTable(nodeHead);
        
            freePriorityQueue(QueueHead);


        }

        printForwardTable(nodeHead);
    }
}



