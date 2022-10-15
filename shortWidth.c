/******************************************************************************
 *
 * File Name: shortWidth.c
 * Authors: Ana Catarina Grilo & Margarida Fernandes
 *
 *****************************************************************************/

#include "shortWidth.h"
#include "graph.h"

//Queue to do dijkstra backwards to find width
struct queueWidth{
    int width;
    int dest;
    struct _Node *destPointer;
    struct _queueWidth *next;
};

//Queue to do dijkstra forward to find length, after finding the width
struct toAnalise{
    int widthAllowed;
    struct _Node *node;
    struct _toAnalise *next;
};

struct queueLength{
    int length;
    int source;
    struct _Node *destPointer;
    struct _queueLength *next;
};

queueWidth *createQueueWidth(queueWidth *head, Edge *neighbour, int dest ,int width){

    int aux_width;
    queueWidth *newElem = NULL;
    
    if((newElem = (queueWidth*) calloc(1, sizeof(queueWidth))) == NULL){ 
        printf("It was not possible to allocate memory\n");
	    exit(1);
    }

    if(neighbour->width > width){
        aux_width = width;
    }else{
        aux_width = neighbour->width;
    }

    newElem->width = aux_width;
    newElem->dest = dest;
    newElem->destPointer = neighbour->destNode;
    newElem->next = NULL;

    head = insertQueueWidth_ordered(head, newElem);
    return head;
}

queueWidth *insertQueueWidth_ordered(queueWidth *head, queueWidth *newElem){

    queueWidth *auxH, *auxT;

    if(head == NULL){
        head = newElem;
    }else{
        if(head->width < newElem->width){
            newElem->next = head;
            head = newElem;
        }
        else{
            auxH = head;
            auxT = head->next;
            if(auxT==NULL){
                head->next = newElem;
            }
            else{
                while((auxT!=NULL) && (auxT->width > newElem->width)){
                    auxH = auxT;
                    auxT = auxT->next;
                }
                newElem->next = auxT;
                auxH->next = newElem;
            }
        }
    }
    return head;
}

toAnalise *createtoAnalise(toAnalise *head, Node *node, int width){

    toAnalise *newElem = NULL;
    
    if((newElem = (toAnalise*) calloc(1, sizeof(toAnalise))) == NULL){ 
        printf("It was not possible to allocate memory\n");
	    exit(1);
    }
    newElem->widthAllowed = width;
    newElem->node = node;
    newElem->next = NULL;

    head = insertAnalise(head, newElem);
    return head;
}

toAnalise *insertAnalise(toAnalise *head, toAnalise *newElem){

    toAnalise *auxH, *auxT;

    if(head == NULL){
        head = newElem;
    }else{
        auxH = head;
        auxT = head->next;
        while(auxT != NULL){
            auxH = auxT;
            auxT = auxT->next;
        }
        auxH->next= newElem;
    }
    return head;
}

queueLength *createQueueLength(queueLength *head, Edge *neighbour, int source , int length){

    queueLength *newElem = NULL;
    
    if((newElem = (queueLength*) calloc(1, sizeof(queueLength))) == NULL){ 
        printf("It was not possible to allocate memory\n");
	    exit(1);
    };
    newElem->length = length + neighbour->length;
    newElem->destPointer = neighbour->destNode;
    newElem->source = source;
    newElem->next = NULL;
    //printf("Criou: node=%d, length=%d, width=%d\n", newElem->destPointer->id, newElem->length, neighbour->width);
    head = insertQueueLength(head, newElem);
    return head;
}

queueLength *insertQueueLength(queueLength *head, queueLength *newElem){

    queueLength *auxH, *auxT;

    if(head == NULL){
        head = newElem;
    }else{
        if(head->length > newElem->length){
            newElem->next = head;
            head = newElem;
        }
        else{
            auxH = head;
            auxT = head->next;
            if(auxT==NULL){
                head->next = newElem;
            }
            else{
                while((auxT!=NULL) && (auxT->length < newElem->length)){
                    auxH = auxT;
                    auxT = auxT->next;
                }
                newElem->next = auxT;
                auxH->next = newElem;
            }
        }
    }
    return head;
}

void shortWidth(Node *nodeHead){
    
    Node *auxT = NULL; 
    queueWidth *queueHead = NULL;
    
    if(nodeHead == NULL){
        return;
    }else{
        auxT = nodeHead;
        dijkstraBackWidth(queueHead, auxT);
        while( auxT->nextNode != NULL){
            auxT = auxT->nextNode;
            dijkstraBackWidth(queueHead, auxT);
        }
    }
    return;
}

void dijkstraBackWidth(queueWidth *head, Node *nodeDest){
    
    toAnalise *headAnalise=NULL;
    queueLength *queueHead = NULL;

    head = nodeDestiny(head, nodeDest);
    printf("\nno destino %d\n",nodeDest->id);
    while(head != NULL){
        if(head->destPointer->visited != head->dest){
            //printf("width=%d, dest=%d, choosen=%d\n", head->width, head->dest ,head->destPointer->id);
            head = analiseQueue(head);
            headAnalise = createtoAnalise(headAnalise, head->destPointer, head->destPointer->nextDest->cost_w);
        }
        head = popQueueWidth(head);    
    }
    while(headAnalise != NULL){
        headAnalise->node->nextDest->cost_l = dijkstraLength(queueHead, headAnalise, nodeDest->id);
        headAnalise  = popAnalise(headAnalise);    
    }

    return;
}

int dijkstraLength(queueLength *head, toAnalise *headAnalise, int visited){

    Node *aux = headAnalise->node;

    printf("no source %d\n",aux->id);
    head = nodeSource(head, aux, headAnalise->widthAllowed, visited);
    while(head != NULL){
       //printf("lenght=%d, source=%d, choosen=%d\n", head->length, head->source, head->destPointer->id);
        if(head->destPointer->id == visited){
            return head->length;
        }else if(head->destPointer->visitedLength != visited){
            head = analiseQueueLength(head, visited, headAnalise->widthAllowed);
        }
        head = popQueueLength(head);    
    }
    return -1;
}

queueWidth *nodeDestiny(queueWidth *head, Node *nodeDest){

    Edge *inNeighbour;

    if(nodeDest == NULL || nodeDest->nextEdgeIn==NULL){
        return head;
    }else{
        inNeighbour = nodeDest->nextEdgeIn;
        head = createQueueWidth(head, inNeighbour, nodeDest->id ,100000000);
        while(inNeighbour->nextEdge != NULL){
            inNeighbour = inNeighbour->nextEdge;
            head = createQueueWidth(head, inNeighbour, nodeDest->id, 100000000);
        }
    }
    return head;
}

queueLength *nodeSource(queueLength *head, Node *nodeSource, int withAllowed, int visited){

    Edge *outNeighbour;

    if(nodeSource == NULL || nodeSource->nextEdgeOut == NULL){
        return head;
    }else{
        outNeighbour = nodeSource->nextEdgeOut;
        if(outNeighbour->width >= withAllowed){
            head = createQueueLength(head, outNeighbour, nodeSource->id, 0);
            if(head->destPointer->id == visited)
                return head;
        }
        while(outNeighbour->nextEdge != NULL){
            outNeighbour = outNeighbour->nextEdge;
            if(outNeighbour->width >= withAllowed){
                head = createQueueLength(head, outNeighbour, nodeSource->id, 0);
                if(head->destPointer->id == visited)
                    return head;
            }
        }
    }
    return head;
}

queueWidth *analiseQueue(queueWidth *head){
   
    Node *node = NULL;
    Edge *inNeighbour;
    ForwardTable *node_Ftable = NULL;

    node = head->destPointer;
    node_Ftable = updateFT_SW(node, head);
    if (node_Ftable != NULL && node->nextEdgeIn!=NULL){
        inNeighbour = node->nextEdgeIn;
        head = createQueueWidth(head, inNeighbour, head->dest, node_Ftable->cost_w);
        while(inNeighbour->nextEdge != NULL){
            inNeighbour = inNeighbour->nextEdge;
            head = createQueueWidth(head, inNeighbour, head->dest, node_Ftable->cost_w);
        }
    }
    node->visited=head->dest;
    return head;
}

queueLength *analiseQueueLength(queueLength *head, int visited, int withAllowed){
   
    Node *node = NULL;
    Edge *outNeighbour;

    node = head->destPointer;
    if (node->nextEdgeOut!=NULL){
        outNeighbour = node->nextEdgeOut;
        if(outNeighbour->width >= withAllowed){
            head = createQueueLength(head, outNeighbour, head->source, head->length);
            if(head->destPointer->id == visited)
                return head;
        }
        while(outNeighbour->nextEdge != NULL){
            outNeighbour = outNeighbour->nextEdge;
            if(outNeighbour->width >= withAllowed){
                head = createQueueLength(head, outNeighbour, head->source, head->length);
                if(head->destPointer->id == visited)
                    return head;
            }
        }
    }
    node->visited=visited;
    return head;
}

ForwardTable *updateFT_SW(Node *node, queueWidth *head){

    ForwardTable *node_ft = NULL;

    node_ft = searchDestiny(node->nextDest, head->dest);
    if(node_ft == NULL){ //If destination does not exist in forwarding table
        node->nextDest = newDestiny(node->nextDest, head);
        return node->nextDest;
    }else{
        //printf("node=%d\n", node->id);
        if(node_ft->cost_w<head->width)
            node_ft->cost_w = head->width;
        return node_ft;
    }

    return NULL;

}

ForwardTable *newDestiny(ForwardTable *ftHead, queueWidth *head){

    ForwardTable *newDest = NULL;

    if((newDest = (ForwardTable*) calloc(1, sizeof(ForwardTable))) == NULL){   
        printf("It was not possible to allocate memory\n");
        exit(1);
    }
    newDest->dest = head->dest;
    newDest->cost_l = 100000000;
    newDest->cost_w = head->width;
    newDest->nextHop = -1;
    newDest->stab_time = 0;
    newDest->hop = NULL;
    newDest->nextDest = NULL;

    if(ftHead == NULL){
        ftHead = newDest;
    }else{
        newDest->nextDest = ftHead;
        ftHead = newDest;
    }

    return ftHead;
}

queueWidth *popQueueWidth(queueWidth *head){

    queueWidth *auxH = NULL;
    
    auxH = head;
    head = head->next;
    free(auxH);

    return head;
}

queueLength *popQueueLength(queueLength *head){

    queueLength *auxH = NULL;
    
    auxH = head;
    head = head->next;
    free(auxH);

    return head;
}

toAnalise *popAnalise(toAnalise *head){

    toAnalise *auxH = NULL;
    
    auxH = head;
    head = head->next;
    free(auxH);

    return head;
}