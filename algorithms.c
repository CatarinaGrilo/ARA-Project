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

PriorityQueue* updatePriorityQueue_l(PriorityQueue *Head, Edge *edge, Node *neighbour, Node *destiny){ 

    PriorityQueue *auxH, *auxT, *newElement, *afterHead = NULL;
    Node *node;
    int length, width;

    /* Values of new element of Queue*/
    if (neighbour == destiny){
        length = edge->length + 0;
        width = min(edge->width, 100000);
        node = edge->destNode;
    }
    else{
        length = edge->length + neighbour->nextDest->cost_l;
        width = min(edge->width, neighbour->nextDest->cost_w);
        node = edge->destNode;
    }
    


    /* Se o nó já foi visitado não interessa colocar na Queue */
    if ( edge->destNode->visited == 1){
        return Head;
    }

    /* Inserir nó ordenadamente*/

    if (Head == NULL){
        newElement = createElementPriorityQueue(length, width , edge->destNode, neighbour);
        return newElement;
    }
    else if(Head->next == NULL){
        newElement = createElementPriorityQueue(length, width , edge->destNode, neighbour);
        Head->next = newElement;
        return Head;
    }
    
    afterHead = Head->next;

    if(afterHead->dist > length){
        newElement = createElementPriorityQueue(length, width , edge->destNode, neighbour);
        newElement->next = afterHead;
        Head->next = newElement;
        return Head;
    }
    else{
        auxH = afterHead;
        auxT = afterHead->next;
        if (auxT == NULL){
            if(auxH->destNode == node) {
                if(auxH->dist < length){  //AfterHead better length than new node          
                    return Head;
                }else if (afterHead->dist == length && afterHead->width >= width){ //AfterHead same length better width than new node
                    return Head;
                }else{ // AfterHead is worse than incoming node => replace by new node
                    Head->next = createElementPriorityQueue(length, width , edge->destNode, neighbour);
                    free(auxH);
                    return Head;
                }
            }
            newElement = createElementPriorityQueue(length, width , edge->destNode, neighbour);
            afterHead->next = newElement;
        }
        else{

           while( (auxT!=NULL) && (auxT->dist <= length)){

                if(auxT->destNode == node) {
                    if(auxT->dist < length){ // AuxT better length than new node
                        return Head;
                    }else if (auxT->dist == length && auxT->width >= width){ // AuxT same length, better width than new node
                            return Head;
                    }
                    else{ // AuxT is worse than new node => replace by new node
                        newElement = createElementPriorityQueue(length, width , edge->destNode, neighbour);
                        auxH->next = newElement;
                        newElement->next = auxT->next;
                        free(auxT);
                        return Head;                    
                    }
                }
                auxH = auxT;
                auxT = auxT->next;
            }
           
            newElement = createElementPriorityQueue(length, width , edge->destNode, neighbour);
            newElement->next = auxT;
            auxH->next = newElement;
        }
    }

   return Head;
}

PriorityQueue* updatePriorityQueue_w(PriorityQueue *Head, Edge *edge, Node *neighbour, Node *destiny){ 

    PriorityQueue *auxH, *auxT, *newElement, *afterHead = NULL;
    Node *node;
    int length, width;

    /* Values of new element of Queue*/
    if (neighbour == destiny){
        length = edge->length + 0;
        width = min(edge->width, 100000);
        node = edge->destNode;
    }
    else{
        length = edge->length + neighbour->nextDest->cost_l;
        width = min(edge->width, neighbour->nextDest->cost_w);
        node = edge->destNode;
    }
    

    /* Se o nó já foi visitado não interessa colocar na Queue */
    if ( edge->destNode->visited == 1){
        return Head;
    }

    /* Inserir nó ordenadamente*/

    if (Head == NULL){
        newElement = createElementPriorityQueue(length, width , edge->destNode, neighbour);
        return newElement;
    }
    else if(Head->next == NULL){
        newElement = createElementPriorityQueue(length, width , edge->destNode, neighbour);
        Head->next = newElement;
        return Head;
    }
    
    afterHead = Head->next;

    if(afterHead->width < width){ // AfterHead worse width, so switch with new node
        newElement = createElementPriorityQueue(length, width , edge->destNode, neighbour);
        newElement->next = afterHead;
        Head->next = newElement;
        return Head;
    }
    else{
        auxH = afterHead;
        auxT = afterHead->next;
        if (auxT == NULL){
            if(auxH->destNode == node) {
                if(auxH->width > width){  //AfterHead better width than new node          
                    return Head;
                }else if (afterHead->width == width && afterHead->dist <= length){ //AfterHead same width better length than new node
                    return Head;
                }else{ // AfterHead is worse than incoming node => replace by new node
                    Head->next = createElementPriorityQueue(length, width , edge->destNode, neighbour);
                    free(auxH);
                    return Head;
                }
            }
            newElement = createElementPriorityQueue(length, width , edge->destNode, neighbour);
            afterHead->next = newElement;
        }
        else{
            // In this case auxH = afterHead
            if(auxH->destNode == node) {
                if(auxH->width > width){ // AuxH better width than new node
                    return Head;
                }else if (auxH->width == width && auxH->dist <= length){ // AuxH same width, better length than new node
                        return Head;
                }
                else{ // AuxH is worse than new node => replace by new node
                    newElement = createElementPriorityQueue(length, width , edge->destNode, neighbour);
                    Head->next = newElement;
                    newElement->next = auxH->next;
                    free(auxH);
                    return Head;                    
                }
            }


            while( (auxT!=NULL) && (auxT->width >= width)){

                if(auxT->destNode == node) {
                    if(auxT->width > width){ // AuxT better width than new node
                        return Head;
                    }else if (auxT->width == width && auxT->dist <= length){ // AuxT same width, better length than new node
                            return Head;
                    }
                    else{ // AuxT is worse than new node => replace by new node
                        newElement = createElementPriorityQueue(length, width , edge->destNode, neighbour);
                        auxH->next = newElement;
                        newElement->next = auxT->next;
                        free(auxT);
                        return Head;                    
                    }
                }
                auxH = auxT;
                auxT = auxT->next;
            }
            newElement = createElementPriorityQueue(length, width , edge->destNode, neighbour);
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
            auxHOP = auxFT->hop;
            if (auxHOP == NULL){
                printf("%d\t%d\t%d\t\n", auxFT->dest, auxFT->cost_w, auxFT->cost_l);
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
    if(nextHop != NULL){
        newElement->nextHop = nextHop->id;
        newElement->hop = nextHop;
    }
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


    for(node = nodeHead; node != NULL; node = node->nextNode){
    
        resetNodeVisited(nodeHead);
        QueueHead = createElementPriorityQueue(0, 100000, node, NULL);
        
        while(QueueHead != NULL){ 
            auxQ = QueueHead;
            auxN = auxQ->destNode;

            if (auxN->visited != 1){
                
                auxN->visited = 1; 
                
                if (auxN != node){
                    updateForwardTable_a(auxN, QueueHead, QueueHead->neighbour, node);
                }
                
                for(auxE=auxN->nextEdgeIn; auxE!=NULL; auxE=auxE->nextEdge){

                    if (mode == 'l'){
                        QueueHead = updatePriorityQueue_l(QueueHead, auxE, auxN, node);
                    }
                    else if(mode == 'w'){
                        QueueHead = updatePriorityQueue_w(QueueHead, auxE, auxN, node);
                    }
                }
            }
            
            QueueHead = QueueHead->next;
            free(auxQ);   
        }

        freePriorityQueue(QueueHead);


    }
}


void algorithmInteractive(Node *nodeHead, char mode){

    PriorityQueue* QueueHead = NULL;
    PriorityQueue* auxQ = NULL;
    Node* auxN = NULL, *node = nodeHead;
    Edge* auxE = NULL;


    if (node == NULL){
        return;
    }
 
    node = nodeHead;

    resetNodeVisited(nodeHead);
    QueueHead = createElementPriorityQueue(0, 100000, node, NULL);
    
    while(QueueHead != NULL){ 
        auxQ = QueueHead;
        auxN = auxQ->destNode;

        if (auxN->visited != 1){
            
            auxN->visited = 1; 
            
            if (auxN != node){
                updateForwardTable_a(auxN, QueueHead, QueueHead->neighbour, node);
            }
            
            for(auxE=auxN->nextEdgeIn; auxE!=NULL; auxE=auxE->nextEdge){

                if (mode == 'l'){
                    QueueHead = updatePriorityQueue_l(QueueHead, auxE, auxN, node);
                }
                else if(mode == 'w'){
                    QueueHead = updatePriorityQueue_w(QueueHead, auxE, auxN, node);
                }
            }
        }
        
        QueueHead = QueueHead->next;
        free(auxQ);   
    }

    freePriorityQueue(QueueHead);   

}


