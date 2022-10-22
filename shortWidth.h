/******************************************************************************
 *
 * File Name: shortWidth.h
 * Authors: Ana Catarina Grilo & Margarida Fernandes
 *
 *****************************************************************************/

#ifndef SHORTWIDTH_INCLUDED
#define SHORTWIDTH_INCLUDED

#include "main.h"

typedef struct queueWidth queueWidth;
typedef struct queueLength queueLength;
typedef struct toAnalise toAnalise;

/* Queue (list) to do dijkstra backwards to find width */
struct queueWidth{
    int width;
    int dest;
    Node *destPointer;
    queueWidth *next;
};
/* List of nodes that can reach a destination when was run dijkstra backwards to find width */
struct toAnalise{
    int widthAllowed;
    Node *node;
    toAnalise *next;
};

/* Queue (list) to do dijkstra forward to find length, after finding the width */
struct queueLength{
    int length;
    int dest;
    Node *destPointer;
    queueLength *next;
};

queueWidth *createQueueWidth(queueWidth *head, Edge *neighbour, int dest ,int width);

queueWidth *insertQueueWidth_ordered(queueWidth *head, queueWidth *newElem);

toAnalise *createtoAnalise(toAnalise *head, Node *node, int width);

toAnalise *insertAnalise(toAnalise *head, toAnalise *newElem);

queueLength *createQueueLength(queueLength *head, Edge *neighbour, int dest, int length);

queueLength *insertQueueLength_ordered(queueLength *head, queueLength *newElem);

void shortWidth(Node *nodeHead);

toAnalise *dijkstraBackWidth(queueWidth *head, Node *nodeDest, toAnalise *headAnalise);

void dijkstraBackLength(queueLength *head, Node *nodeDest, toAnalise *headAnalise);

queueWidth *nodeDestiny(queueWidth *head, Node *nodeDest);

queueLength *nodeDestinyLenght(queueLength *head, Node *nodeDest, toAnalise *headAnalise);

queueWidth *analiseQueue(queueWidth *head);

queueLength *analiseQueueLength(queueLength *head, toAnalise *headAnalise);

ForwardTable *updateFT_SW(Node *node, queueWidth *head);

int updateFT_SW_Length(Node *node, queueLength *head, toAnalise *headAnalise);

ForwardTable *newDestiny(ForwardTable *ftHead, queueWidth *head);

queueWidth *popQueueWidth(queueWidth *head);

queueLength *popQueueLength(queueLength *head);

toAnalise *popAnalise(toAnalise *head);

#endif