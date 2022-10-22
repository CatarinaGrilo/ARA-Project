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
 * Implement the backward's Dijkstra algorithm using a priority queue
 *****************************************************************************/

/* Returns the minimum value between 2 integers */
int min(int a, int b){

    return (a > b) ? b : a;
}


/* Creates a new element for the Priority Queue */
PriorityQueue* createElementPriorityQueue(int dist, int width, Node *node, Node *neighbour){ 

    PriorityQueue *newElement;

    // Create a new element for Priority Queue
    if((newElement = (PriorityQueue*) calloc(1, sizeof(PriorityQueue))) == NULL){ 
		printf("It was not possible to allocate memory\n");
	    exit(1);
    }

    // Initializes field's values
    newElement->dist = dist;
    newElement->width = width;
    newElement->destNode = node;
    newElement->neighbour = neighbour;
    newElement->next = NULL;

    return newElement;
}

/* Updates the Priority Queue by orderly inserting new element according to widest-shortest paths*/
PriorityQueue* updatePriorityQueue_l(PriorityQueue *Head, Edge *edge, Node *neighbour, Node *destiny){ 

    PriorityQueue *auxH, *auxT, *newElement, *afterHead = NULL;
    Node *node;
    int length, width;

    /* Values of new element of Queue*/
    if (neighbour == destiny){
        // Node is the destiny
        length = edge->length + 0;
        width = min(edge->width, 100000);
        node = edge->destNode;
    }
    else{
        length = edge->length + neighbour->nextDest->cost_l;
        width = min(edge->width, neighbour->nextDest->cost_w);
        node = edge->destNode;
    }
    
    /* If node has already been visited, don't add it to Queue */
    if ( edge->destNode->visited == 1){
        return Head;
    }

    /* Insert node orderly */

    // If Queue is empty, create new element
    if (Head == NULL){
        newElement = createElementPriorityQueue(length, width , edge->destNode, neighbour);
        return newElement;
    }
    // If there is no 2nd element it, add new element in 2nd position
    else if(Head->next == NULL){
        newElement = createElementPriorityQueue(length, width , edge->destNode, neighbour);
        Head->next = newElement;
        return Head;
    }
    
    afterHead = Head->next;
    // 1st element from Queue shouldn't be taken into account when ordering, only order the following nodes

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

/* Updates the Priority Queue by orderly inserting new element according to shortest-widest paths*/
PriorityQueue* updatePriorityQueue_w(PriorityQueue *Head, Edge *edge, Node *neighbour, Node *destiny){ 

    PriorityQueue *auxH, *auxT, *newElement, *afterHead = NULL;
    Node *node;
    int length, width;

    /* Values of new element of Queue*/
    if (neighbour == destiny){
        // Node is the destiny
        length = edge->length + 0;
        width = min(edge->width, 100000);
        node = edge->destNode;
    }
    else{
        length = edge->length + neighbour->nextDest->cost_l;
        width = min(edge->width, neighbour->nextDest->cost_w);
        node = edge->destNode;
    }
    

    /* If node has already been visited, don't add it to Queue */
    if ( edge->destNode->visited == 1){
        return Head;
    }

    /* Insert node orderly */

    // If Queue is empty, create new element
    if (Head == NULL){
        newElement = createElementPriorityQueue(length, width , edge->destNode, neighbour);
        return newElement;
    }
    // If there is no 2nd element it, add new element in 2nd position
    else if(Head->next == NULL){
        newElement = createElementPriorityQueue(length, width , edge->destNode, neighbour);
        Head->next = newElement;
        return Head;
    }
    
    afterHead = Head->next;
    // 1st element from Queue shouldn't be taken into account when ordering, only order the following nodes

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

/* Print present state of Priority Queue */
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


/* Free the memory used for Priority Queue */
void freePriorityQueue(PriorityQueue* QueueHead){

    PriorityQueue* aux;

    while(QueueHead!= NULL){
        aux = QueueHead;
        QueueHead = QueueHead->next;
        free(aux);
    }
}

/* Reset all the visited nodes to unvisted nodes*/
void resetNodeVisited(Node *Head){

    while (Head != NULL){
        Head->visited = 0;
        Head = Head->nextNode;
    }
}

/* Updates Forward Table of node, with the destiny it can reach and stores the cost (length, width) 
   and the node it should hop to to reach the destination*/
void updateForwardTable_a(Node *node, PriorityQueue *element, Node *nextHop, Node *destiny){

    ForwardTable *newElement;

    // Allocates memory for new element
    if((newElement = (ForwardTable*) calloc(1, sizeof(ForwardTable))) == NULL){ 
        printf("It was not possible to allocate memory\n");
        exit(1);
    }

    // Initializes the fields
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
        // Insert in the 1st position of Forward Table
        newElement->nextDest = node->nextDest;
        node->nextDest = newElement;
    }
}

/* Runs the stable state algorithm */
void algorithm(Node *nodeHead, char mode){

    PriorityQueue* QueueHead = NULL;
    PriorityQueue* auxQ = NULL;
    Node* auxN = NULL, *node = nodeHead;
    Edge* auxE = NULL;


    for(node = nodeHead; node != NULL; node = node->nextNode){
    
        resetNodeVisited(nodeHead);
        // Create destiny
        QueueHead = createElementPriorityQueue(0, 100000, node, NULL);
        
        while(QueueHead != NULL){ 
            // Pop 1st element from Queue
            auxQ = QueueHead;
            auxN = auxQ->destNode;

            if (auxN->visited != 1){
                // Visit the node
                auxN->visited = 1; 
                
                if (auxN != node){
                    updateForwardTable_a(auxN, QueueHead, QueueHead->neighbour, node);
                }
                
                // Add nodes to queue for each link
                for(auxE=auxN->nextEdgeIn; auxE!=NULL; auxE=auxE->nextEdge){

                    if (mode == 'l'){ // Widest Shortest
                        QueueHead = updatePriorityQueue_l(QueueHead, auxE, auxN, node);
                    }
                    else if(mode == 'w'){ // Shortest Widest
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


/* Runs the stable state algorithm for a specific destiny*/
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
    // Create destiny
    QueueHead = createElementPriorityQueue(0, 100000, node, NULL);
    
    while(QueueHead != NULL){ 
        // Pop 1st element from Queue
        auxQ = QueueHead;
        auxN = auxQ->destNode;

        if (auxN->visited != 1){
            // Visit the node
            auxN->visited = 1; 
            
            if (auxN != node){
                updateForwardTable_a(auxN, QueueHead, QueueHead->neighbour, node);
            }
            // Add nodes to queue for each link
            for(auxE=auxN->nextEdgeIn; auxE!=NULL; auxE=auxE->nextEdge){

                if (mode == 'l'){ // Widest Shortest
                    QueueHead = updatePriorityQueue_l(QueueHead, auxE, auxN, node);
                }
                else if(mode == 'w'){ // Shortest Widest
                    QueueHead = updatePriorityQueue_w(QueueHead, auxE, auxN, node);
                }
            }
        }
        
        QueueHead = QueueHead->next;
        free(auxQ);   
    }

    freePriorityQueue(QueueHead);   
}


