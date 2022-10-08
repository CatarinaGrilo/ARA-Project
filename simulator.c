/******************************************************************************
 *
 * File Name: simulator.c
 * Authors: Ana Catarina Grilo & Margarida Fernandes
 *
 *****************************************************************************/

#include "simulator.h"

#include "calendar.h"
#include "graph.h"

extern float time_simul;

/**********************************     Simulation      ***********************
 * Wake up each node separately, and fill the forwarding tables of the nodes  
 * that manage to reach the node that woke up. And do the same for the 
 * remaining nodes in the node list.
 *****************************************************************************/
void simulations(Node *nodeHead, Event *eventHead){
    
    Node *auxT = NULL; 
    
    if(nodeHead == NULL){
        return;
    }else{
        auxT = nodeHead;
        time_simul = 0;
        calendarEvolution(eventHead, auxT);
        while( auxT->nextNode != NULL){
            auxT = auxT->nextNode;
            calendarEvolution(eventHead, auxT);
        }
    }
    return;
}

void simulationsInteractive(Node *destNode, Event *eventHead){
    
    Node *auxT = NULL; 
    
    if(destNode == NULL){
        return;
    }else{
        auxT = destNode;
        time_simul = 0;
        calendarEvolution(eventHead, auxT);
    }
    return;
} 