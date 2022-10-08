/******************************************************************************
 *
 * File Name: calendar.c
 * Authors: Ana Catarina Grilo & Margarida Fernandes
 *
 *****************************************************************************/

#include "calendar.h"

#include "graph.h"
#include "simulator.h"

extern float time_simul;

struct Event{
    float An;
    int dest;
    int origin;
    int msg[3]; //[0]-destino, [1]-lenght, [2]-width
    Node *destPointer;
    Node *originPointer;
    Event *nextEvent;
};

Event *createEvent(Event *eventsHead, Node *wakeupNode, Edge *neighbour, int dest, int cost_l, int cost_w){ 

    float Sn;
    Event *newEvent = NULL;
    time_t t;
    srand((unsigned) time(&t));
    
    if((newEvent = (Event*) calloc(1, sizeof(Event))) == NULL){ 
        printf("It was not possible to allocate memory\n");
	    exit(1);
    }

    Sn = 1 + ((float)rand()/(float)(RAND_MAX)) * 2;

    newEvent->An = time_simul + Sn;
    if (newEvent->An < neighbour->An){ 
        newEvent->An = neighbour->An; 
    } else{
        neighbour->An = newEvent->An;
    }  
    newEvent->dest = neighbour->dest;
    newEvent->origin = wakeupNode->id;
    newEvent->msg[0] = dest;
    newEvent->msg[1] = cost_l;
    newEvent->msg[2] = cost_w;
    newEvent->destPointer = neighbour->destNode;
    newEvent->originPointer = wakeupNode;
    newEvent->nextEvent = NULL;

    eventsHead = insertEvent_ordered(eventsHead, newEvent);
    return eventsHead;
}

Event *insertEvent_ordered(Event *Listhead, Event *newEvent){

    Event *auxH, *auxT;

    if(Listhead == NULL){
        Listhead = newEvent;
    }else{
        if(Listhead->An > newEvent->An){
            newEvent->nextEvent = Listhead;
            Listhead = newEvent;
        }
        else{
            auxH = Listhead;
            auxT = Listhead->nextEvent;
            if(auxT==NULL){
                Listhead->nextEvent = newEvent;
            }
            else{
                while((auxT!=NULL) && (auxT->An < newEvent->An)){
                    auxH = auxT;
                    auxT = auxT->nextEvent;
                }
                newEvent->nextEvent = auxT;
                auxH->nextEvent = newEvent;
            }
        }
    }
    return Listhead;
}

void calendarEvolution(Event *eventsHead, Node *wakeupNode){
    
    eventsHead = announceNode(eventsHead, wakeupNode); //First wake up the node, create the respective events and insert them in the calendar
    //printf("no foi anunciado\n");
    while(eventsHead != NULL){
        //printf("An=%f, dest=%d, origin=%d, msg_dest=%d, l=%d, w=%d\n", eventsHead->An, eventsHead->dest, eventsHead->origin, eventsHead->msg[0], eventsHead->msg[1],eventsHead->msg[2]);
        eventsHead = doEvent(eventsHead, eventsHead->destPointer);
        eventsHead = popEvent(eventsHead);    
    }

    return;
}

Event *announceNode(Event *eventsHead, Node *wakeupNode){

    Edge *inNeighbour;

    if(wakeupNode == NULL || wakeupNode->nextEdgeIn==NULL){
        return eventsHead;
    }else{
        inNeighbour = wakeupNode->nextEdgeIn;
        eventsHead = createEvent(eventsHead, wakeupNode, inNeighbour, wakeupNode->id, 0, 1000000);
        while(inNeighbour->nextEdge != NULL){
            inNeighbour = inNeighbour->nextEdge;
            eventsHead = createEvent(eventsHead, wakeupNode, inNeighbour, wakeupNode->id , 0, 1000000);
        }
    }
    return eventsHead;
}

Event *doEvent(Event *eventsHead, Node *nodeDest){
   
    Node *node = NULL;
    ForwardTable *node_Ftable = NULL;

    node = nodeDest;
    time_simul = eventsHead->An;
    node_Ftable = updateForwardTable(node, eventsHead);
    if (node_Ftable != NULL){
        eventsHead = sendToNeighbour(eventsHead, node, node_Ftable);
    }
    return eventsHead;
}

Event *sendToNeighbour(Event *eventsHead, Node *origin, ForwardTable *node_Ftable){

    Edge *inNeighbour;

    if(origin == NULL || origin->nextEdgeIn==NULL){
        return eventsHead;
    }
    else{
        inNeighbour = origin->nextEdgeIn;
        if(inNeighbour->dest != eventsHead->msg[0]){
            eventsHead = createEvent(eventsHead, origin, inNeighbour, node_Ftable->dest ,node_Ftable->cost_l, node_Ftable->cost_w);
        }
        while(inNeighbour->nextEdge != NULL){
            inNeighbour = inNeighbour->nextEdge;
            if( inNeighbour->dest != eventsHead->msg[0]){
                eventsHead = createEvent(eventsHead, origin, inNeighbour, node_Ftable->dest ,node_Ftable->cost_l, node_Ftable->cost_w);
            }
        }
    }
    return eventsHead;
}

Event *popEvent(Event *eventsHead){

    Event *auxH = NULL;
    
    auxH = eventsHead;
    eventsHead = eventsHead->nextEvent;
    free(auxH);

    return eventsHead;
}




