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

RoutingTable *searchRoute(RoutingTable *rtHead, int dest_id, int nextHop);

ForwardTable *createDestiny(ForwardTable *ftHead, Edge *edgesHead, Event *event);

RoutingTable *createRT(RoutingTable *rtHead, Edge *edgesHead, Event *event);

RoutingTable *searchRouteNeighbour(RoutingTable *rtHead, int cost_w, int cost_l);

ForwardTable *updateEstimate(ForwardTable *dest, Edge *edge , Event *event);

void printFT_SW(Graph *Head);

void printFT(Graph *Head);

void printGraph(Graph *Head);

void freeGraph(Node *listHead);

#endif