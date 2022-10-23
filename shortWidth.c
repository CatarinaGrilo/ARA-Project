/******************************************************************************
 *
 * File Name: shortWidth.c
 * Authors: Ana Catarina Grilo & Margarida Fernandes
 *
 *****************************************************************************/

#include "shortWidth.h"
#include "graph.h"

int flag=0; //this serves the porpuse of truncating the Dijkstra length

/* Create a new element of QueueWidth */
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
    //printf("Criou: node=%d, width=%d\n", newElem->destPointer->id, neighbour->width);
    head = insertQueueWidth_ordered(head, newElem);
    return head;
}

/* Inserts a element of QueueWidth in the list in descending order */
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

/* Create a new element of toAnalise */
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

/* Inserts a element of toAnalise in list */
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

/* Create a new element of QueueLength */
queueLength *createQueueLength(queueLength *head, Edge *neighbour, int dest, int length){

    queueLength *newElem = NULL;
    
    if((newElem = (queueLength*) calloc(1, sizeof(queueLength))) == NULL){ 
        printf("It was not possible to allocate memory\n");
	    exit(1);
    };
    newElem->length = length + neighbour->length;
    newElem->destPointer = neighbour->destNode;
    newElem->dest = dest;
    newElem->next = NULL;
    //printf("Criou: node=%d, length=%d, width=%d\n", newElem->destPointer->id, newElem->length, neighbour->width);
    head = insertQueueLength_ordered(head, newElem);
    return head;
}

/* Inserts a element of QueueLength in the list in ascending order */
queueLength *insertQueueLength_ordered(queueLength *head, queueLength *newElem){

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

/* "Algorithm" that computes the shortest-widest for all nodes */
void shortWidth(Node *nodeHead){
    
    Node *auxT = NULL; 
    queueWidth *queueHeadW = NULL;
    queueLength *queueHeadL = NULL;
    toAnalise *headAnalise=NULL;
    
    if(nodeHead == NULL){
        return;
    }else{
        auxT = nodeHead;
        headAnalise = dijkstraBackWidth(queueHeadW, auxT, headAnalise);
        while(headAnalise!=NULL){
            //printf("ANALISE: WIDTH ALLOWED=%d", headAnalise->widthAllowed);
            flag = 0;
            dijkstraBackLength(queueHeadL, auxT, headAnalise);
            headAnalise = popAnalise(headAnalise);
        }
        while( auxT->nextNode != NULL){
            auxT = auxT->nextNode;
            headAnalise = dijkstraBackWidth(queueHeadW, auxT, headAnalise);
            while(headAnalise!=NULL){
                //printf("ANALISE: WIDTH ALLOWED=%d", headAnalise->widthAllowed);
                flag=0;
                dijkstraBackLength(queueHeadL, auxT, headAnalise);
                headAnalise = popAnalise(headAnalise);
            }
        }
    }
    return;
}

void shortWidthInteractive(Node *destNode){
    queueWidth *queueHeadW = NULL;
    queueLength *queueHeadL = NULL;
    toAnalise *headAnalise=NULL;
    
    if(destNode == NULL){
        return;
    }else{
        headAnalise = dijkstraBackWidth(queueHeadW, destNode, headAnalise);
        while(headAnalise!=NULL){
            //printf("ANALISE: WIDTH ALLOWED=%d", headAnalise->widthAllowed);
            flag = 0;
            dijkstraBackLength(queueHeadL, destNode, headAnalise);
            headAnalise = popAnalise(headAnalise);
        }
    }
    return;
} 

/* Backwards Dijkstra's algorithm just to find the widths */
toAnalise *dijkstraBackWidth(queueWidth *head, Node *nodeDest, toAnalise *headAnalise){


    printf("no destino %d\n",nodeDest->id);
    head = nodeDestiny(head, nodeDest);
    while(head != NULL){
        if(head->destPointer->visited != head->dest){
            //printf("width=%d, dest=%d, choosen=%d\n", head->width, head->dest ,head->destPointer->id);
            head = analiseQueue(head);
            headAnalise = createtoAnalise(headAnalise, head->destPointer, head->destPointer->nextDest->cost_w);
        }
        head = popQueueWidth(head);    
    }
    return headAnalise;
}

/* Backwards Dijkstra's algorithm just to find the lengths, after finding the width */
void dijkstraBackLength(queueLength *head, Node *nodeDest, toAnalise *headAnalise){

    
    //printf("\n-----Analise node=%d-----\n",headAnalise->node->id);
    head = nodeDestinyLenght(head, nodeDest, headAnalise);
    while(head != NULL){
        if(head->destPointer->visitedLength != head->dest || head->destPointer->visitedLength_aux!=headAnalise->node->id){
            //printf("length=%d, dest=%d, choosen=%d\n", head->length, head->dest ,head->destPointer->id);
            if(flag!=1)
                head = analiseQueueLength(head, headAnalise);
        }
        head = popQueueLength(head);    
    }
    return;
}

/* See edges od in-neighbors of destiny and put on queueWidth list */
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

/* See edges od in-neighbors of destiny and put on queueLength list */
queueLength *nodeDestinyLenght(queueLength *head, Node *nodeDest, toAnalise *headAnalise){

    Edge *inNeighbour;
    if(nodeDest == NULL || nodeDest->nextEdgeIn==NULL){
        return head;
    }else{
        inNeighbour = nodeDest->nextEdgeIn;
        if(headAnalise->widthAllowed <= inNeighbour->width){
            head = createQueueLength(head, inNeighbour, nodeDest->id ,0);
        }
        while(inNeighbour->nextEdge != NULL){
            inNeighbour = inNeighbour->nextEdge;
            if(headAnalise->widthAllowed <= inNeighbour->width){
                head = createQueueLength(head, inNeighbour, nodeDest->id ,0);
            }
        }
    }
    return head;
}

/* Analise first element of list, because its the one with better estimate */
queueWidth *analiseQueue(queueWidth *head){
   
    Node *node = NULL;
    Edge *inNeighbour;
    ForwardTable *node_Ftable = NULL;

    node = head->destPointer;
    node_Ftable = updateFT_SW(node, head);
    if (node_Ftable != NULL && node->nextEdgeIn!=NULL){
        inNeighbour = node->nextEdgeIn;
        if(inNeighbour->dest != head->dest){
            head = createQueueWidth(head, inNeighbour, head->dest, node_Ftable->cost_w);
        }
        while(inNeighbour->nextEdge != NULL){
            inNeighbour = inNeighbour->nextEdge;
            if(inNeighbour->dest != head->dest){
                head = createQueueWidth(head, inNeighbour, head->dest, node_Ftable->cost_w);
            }
        }
    }
    node->visited=head->dest;
    return head;
}

/* Analise first element of list, because its the one with better estimate */
queueLength *analiseQueueLength(queueLength *head, toAnalise *headAnalise){
   
    Node *node = NULL;
    Edge *inNeighbour;
    int length;

    node = head->destPointer;
    length = updateFT_SW_Length(node, head, headAnalise);
    //printf("analise length=%d\n",length);
    if (length!=-1 && node->nextEdgeIn!=NULL && flag!=1){
        inNeighbour = node->nextEdgeIn;
        if(inNeighbour->dest != head->dest && headAnalise->widthAllowed <=inNeighbour->width){
            head = createQueueLength(head, inNeighbour, head->dest, length);
        }
        while(inNeighbour->nextEdge != NULL){
            inNeighbour = inNeighbour->nextEdge;
            if(inNeighbour->dest != head->dest && headAnalise->widthAllowed <=inNeighbour->width){
                head = createQueueLength(head, inNeighbour, head->dest, length);
            }
        }
    }
    node->visitedLength=head->dest;
    node->visitedLength_aux=headAnalise->node->id;
    return head;
}

/* Update the estimate of width */
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

/* Update the estimate of length */
int updateFT_SW_Length(Node *node, queueLength *head, toAnalise *headAnalise){

    ForwardTable *node_ft = NULL;

    if(node->visitedLength!=headAnalise->node->id && node->id==headAnalise->node->id){
        node_ft = searchDestiny(node->nextDest, head->dest);
        if(node_ft->cost_l > head->length){
            node_ft->cost_l = head->length;
            flag=1;
            //printf("TABELA:node=%d length=%d\n", node->id, node_ft->cost_l);
            return node_ft->cost_l;
        }
    }
    node_ft = searchDestiny(node->nextDest, head->dest);
    //printf("Update:node=%d length=%d length_node=%d\n", node->id, head->length, node_ft->length_aux);
    if(node_ft->length_aux >= head->length){
        node_ft->length_aux = head->length;
        //printf("Update2:node=%d length=%d length_node=%d\n", node->id, head->length, node_ft->length_aux);
        return node_ft->length_aux;
    }
    return -1;

}

/* Create a new destiny in the ForwardTable list */
ForwardTable *newDestiny(ForwardTable *ftHead, queueWidth *head){

    ForwardTable *newDest = NULL;

    if((newDest = (ForwardTable*) calloc(1, sizeof(ForwardTable))) == NULL){   
        printf("It was not possible to allocate memory\n");
        exit(1);
    }
    newDest->dest = head->dest;
    newDest->cost_l = 100000000;
    newDest->length_aux = 100000000;
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