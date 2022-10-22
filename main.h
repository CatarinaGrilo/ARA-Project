/******************************************************************************
 *
 * File Name: graph.h
 * Authors: Ana Catarina Grilo & Margarida Fernandes
 *
 *****************************************************************************/

#ifndef MAIN_INCLUDED
#define MAIN_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#define N 4096


typedef struct Graph Graph;
typedef struct Node Node;
typedef struct Edge Edge;
typedef struct ForwardTable ForwardTable;
typedef struct RoutingTable RoutingTable;
typedef struct Event Event;

struct Graph{
    Node *nextNode;
};

struct Node{
    int id;
    int visited;
    int visitedLength;
    int visitedLength_aux;
    Node *nextNode;
    Edge *nextEdgeOut;
    Edge *nextEdgeIn;
    ForwardTable *nextDest;

};

struct Edge{
    int dest;
    int width;
    int length;
    float An;
    Edge *nextEdge;
    Node *destNode;
};

struct ForwardTable{
    int dest;
    int cost_l;
    int length_aux;
    int cost_w;
    int nextHop;
    float stab_time;
    Node *hop;
    ForwardTable* nextDest;
    RoutingTable* nextRoute;
} ;

//Info dos vizinhos para o mm destino
struct RoutingTable{
    int dest;
    int cost_l;
    int cost_w;
    int nextHop;
    Node *hop;
    RoutingTable* nextDest;
};

struct Event{
    float An;
    int dest;
    int origin;
    int msg[3];
    Node *destPointer;
    Node *originPointer;
    Event *nextEvent;
};




/**********************************************************************************************/

// typedef struct _Graph{
//     struct _Nodes *nextNode;
// } Graph;

// typedef struct _Node{
//     int id;
//     int visited;
//     int visitedLength;
//     int visitedLength_aux;
//     struct _Node *nextNode;
//     struct _Edge *nextEdgeOut;
//     struct _Edge *nextEdgeIn;
//     struct _ForwardTable *nextDest;

// } Node;

// typedef struct _Edge{
//     int dest;
//     int width;
//     int length;
//     float An;
//     struct _Edge *nextEdge;
//     struct _Node *destNode;
// } Edge;

// typedef struct _ForwardTable{
//     int dest;
//     int cost_l;
//     int length_aux;
//     int cost_w;
//     int nextHop;
//     float stab_time;
//     struct _Node *hop;
//     struct _ForwardTable* nextDest;
//     struct _RoutingTable* nextRoute;
// } ForwardTable;

// typedef struct _RoutingTable//Info dos vizinhos para o mm destino
// {
//     int dest;
//     int cost_l;
//     int cost_w;
//     int nextHop;
//     struct _Node *hop;
//     struct _RoutingTable* nextDest;
// } RoutingTable;

// typedef struct _Event{
//     float An;
//     int dest;
//     int origin;
//     int msg[3];
//     struct _Node *destPointer;
//     struct _Node *originPointer;
//     struct _Event *nextEvent;
// } Event;




#endif