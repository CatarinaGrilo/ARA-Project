/******************************************************************************
 *
 * File Name: graph.h
 * Authors: Ana Catarina Grilo & Margarida Fernandes
 *
 *****************************************************************************/

#ifndef GRAPH_H
#define GRAPH_H

#include <stdio.h>
#include <stdlib.h>

typedef struct _Graph{
    struct _Nodes *nextNode;
} Graph;

typedef struct _Node{
    int id;
    struct _Node *nextNode;
    struct _Edge *nextEdgeOut;
    struct _Edge *nextEdgeIn;
} Node;

typedef struct _Edge{
    int dest;
    int width;
    int length;
    struct _Edge *nextEdge;
} Edge;

Graph *createGraph(Graph *listHead, int tail, int head, int width, int length);

Node *searchGraph(Graph *listHead, int id);

Node *createNode(int tail);

Node *insertNode(Node *listHead, Node *newNode);

Edge *createEdge(int head, int width, int length);

Edge *insertEdge(Edge *listHead, Edge *newEdge);

void printGraph(Graph *Head);

void freeGraph(Node *listHead);

#endif