/******************************************************************************
 *
 * File Name: algorithms.h
 * Authors: Ana Catarina Grilo & Margarida Fernandes
 *
 *****************************************************************************/

#ifndef ALGORITHMS_INCLUDED
#define ALGORITHMS_INCLUDED

#include "main.h"


typedef struct PriorityQueue PriorityQueue;

struct PriorityQueue{
    int dist;
    int width;
    Node* destNode;
    Node* neighbour;
    PriorityQueue* next;

};


int min(int a, int b);
PriorityQueue* createElementPriorityQueue(int dist, int width, Node *node, Node *neighbour);
PriorityQueue* updatePriorityQueue_l(PriorityQueue *Head, Edge *edge, Node *neighbour, Node *destiny);
PriorityQueue* updatePriorityQueue_w(PriorityQueue *Head, Edge *edge, Node *neighbour, Node *destiny);
void printPriorityQueue(PriorityQueue* QueueHead);
void freePriorityQueue(PriorityQueue* QueueHead);  
void createDestinysFT(Node* nodeHead);
void resetNodeVisited(Node *Head);
void printForwardTable(Node *Head);
void updateForwardTable_a(Node *node, PriorityQueue *element, Node *nextHop, Node *destiny);
void algorithm(Node *nodeHead, char mode);
void algorithmInteractive(Node *nodeHead, char mode);

#endif