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
    int length;
    int width;
    int dest;
    struct _Node *destPointer;
    struct _queueLength *next;
} queueLength;

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