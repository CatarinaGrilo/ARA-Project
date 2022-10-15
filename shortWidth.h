/******************************************************************************
 *
 * File Name: shortWidth.h
 * Authors: Ana Catarina Grilo & Margarida Fernandes
 *
 *****************************************************************************/

#ifndef SHORTWIDTH_INCLUDED
#define SHORTWIDTH_INCLUDED

#include "main.h"

typedef struct _queueWidth{
    int width;
    int dest;
    struct _Node *destPointer;
    struct _queueWidth *next;
} queueWidth;

typedef struct _toAnalise{
    int widthAllowed;
    struct _Node *node;
    struct _toAnalise *next;
}toAnalise;

typedef struct _queueLength{
    int widthAllowed;
    int length;
    int source;
    struct _Node *destPointer;
    struct _queueLength *next;
} queueLength;

queueWidth *createQueueWidth(queueWidth *head, Edge *neighbour, int dest ,int width);

queueWidth *insertQueueWidth_ordered(queueWidth *head, queueWidth *newElem);

toAnalise *createtoAnalise(toAnalise *head, Node *node, int width);

toAnalise *insertAnalise(toAnalise *head, toAnalise *newElem);

queueLength *createQueueLength(queueLength *head, Edge *neighbour, int source , int length);

queueLength *insertQueueLength(queueLength *head, queueLength *newElem);

void shortWidth(Node *nodeHead);

void dijkstraBackWidth(queueWidth *head, Node *nodeDest);

int dijkstraLength(queueLength *head, toAnalise *headAnalise, int visited);

queueWidth *nodeDestiny(queueWidth *head, Node *nodeDest);

queueLength *nodeSource(queueLength *head, Node *nodeSource, int withAllowed, int visited);

queueWidth *analiseQueue(queueWidth *head);

queueLength *analiseQueueLength(queueLength *head, int visited, int withAllowed);

ForwardTable *updateFT_SW(Node *node, queueWidth *head);

ForwardTable *newDestiny(ForwardTable *ftHead, queueWidth *head);

queueWidth *popQueueWidth(queueWidth *head);

queueLength *popQueueLength(queueLength *head);

toAnalise *popAnalise(toAnalise *head);

#endif