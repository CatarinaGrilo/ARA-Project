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

void stats(Graph *Head){

    Node *auxN;
    ForwardTable *auxFT;
    FILE *fpTime, *fpWidth, *fpLength;

    fpTime = fopen("dados_time.txt","w");
    fpWidth = fopen("dados_width.txt","w");
    fpLength = fopen("dados_length.txt","w");

    for(auxN=Head->nextNode; auxN!=NULL; auxN=auxN->nextNode){
        for(auxFT=auxN->nextDest; auxFT!=NULL; auxFT=auxFT->nextDest){
            fprintf(fpTime,"%f\n", auxFT->stab_time);
            fprintf(fpWidth,"%d\n", auxFT->cost_w);
            fprintf(fpLength,"%d\n", auxFT->cost_l);
        }
    }
    fclose(fpTime);
    fclose(fpWidth);
    fclose(fpLength);
    
    return;
}