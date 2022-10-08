/******************************************************************************
 *
 * File Name: graph.h
 * Authors: Ana Catarina Grilo & Margarida Fernandes
 *
 *****************************************************************************/

#ifndef GRAPH_INCLUDED
#define GRAPH_INCLUDED

#include "main.h"

Graph *createGraph(Graph *listHead, int tail, int head, int width, int length);

Node *searchGraph(Graph *listHead, int id);

Node *createNode(int tail);

Node *insertNode(Node *listHead, Node *newNode);

Edge *createEdge(int head, int width, int length, Node *dest);

Edge *insertEdge(Edge *listHead, Edge *newEdge);

Edge *searchEdge(Edge *listHead, int id);

ForwardTable *updateForwardTable(Node *node, Event *eventsHead);

ForwardTable *searchDestiny(ForwardTable *ftHead, int dest_id);

ForwardTable *createDestiny(ForwardTable *ftHead, Edge *edgesHead, Event *event);

ForwardTable *updateEstimate(ForwardTable *dest, Edge *edge , Event *event);

void resetFT(Node *listHead);

void printGraph(Graph *Head);

void freeGraph(Node *listHead);

#endif