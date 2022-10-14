/******************************************************************************
 *
 * File Name: graph.c
 * Authors: Ana Catarina Grilo & Margarida Fernandes
 *
 *****************************************************************************/

#include "graph.h"
#include "calendar.h"

extern char flag_sim;

// Data structure to store the graph, list of all nodes that exist in the network
struct Graph
{
    struct Node* nextNode;
    
};
// Data structure to store Edgeacency list nodes of the graph
struct Node
{
    int id;
    struct Node* nextNode;
    struct Edge* nextEdgeOut;
    struct Edge* nextEdgeIn;
    struct ForwardTable* nextDest;
    struct DistTable* distTable;
};

// Data structure to store a graph edge
struct Edge
{
    int dest;
    int width, length;
    float An;
    struct Edge* nextEdge;
    struct Node* destNode;
};

// Data structure to store a graph edge
struct ForwardTable
{
    int dest;
    int cost_l;
    int cost_w;
    int nextHop;
    float stab_time;
    struct Node *hop;
    struct ForwardTable* nextDest;
    RoutingTable* nextRoute; //ter as varias opções de chegar ao mm caminho 
};

struct RoutingTable//Info dos vizinhos para o mm destino
{
    int dest;
    int cost_l;
    int cost_w;
    int nextHop;
    struct Node *hop;
    struct RoutingTable* nextDest;
};



Graph *createGraph(Graph *listHead, int tail, int head, int width, int length){ 

    Node *newNodeT = NULL;
    Node *newNodeH = NULL;
    Edge *newEdgeT = NULL;
    Edge *newEdgeH = NULL;
    
    newNodeT = searchGraph(listHead, tail);
    if (newNodeT == NULL){
        newNodeT = createNode(tail);
        listHead->nextNode = insertNode(listHead->nextNode, newNodeT);
    }

    newNodeH = searchGraph(listHead, head);
    if (newNodeH == NULL){
        newNodeH = createNode(head);
        listHead->nextNode = insertNode(listHead->nextNode, newNodeH);
    }

    newEdgeT = createEdge(head, width, length, newNodeH);
    newNodeT->nextEdgeOut = insertEdge(newNodeT->nextEdgeOut, newEdgeT);

    newEdgeH = createEdge(tail, width, length, newNodeT);
    newNodeH->nextEdgeIn = insertEdge(newNodeH->nextEdgeIn, newEdgeH);

    return listHead;
}

Node *searchGraph(Graph *listHead, int id){

    Node *auxT = listHead->nextNode;

    if(auxT == NULL){
        return NULL;
    }else{
        if(auxT->id == id){
            return auxT;
        }
        while(auxT->nextNode !=NULL){
            auxT = auxT->nextNode;
            if( auxT->id == id ){
                return auxT;
            }
        }
    }
    return NULL;
}

Node *createNode(int tail){

    Node *newNode = NULL;
    
    if((newNode = (Node*) calloc(1, sizeof(Node))) == NULL){ 
        printf("It was not possible to allocate memory\n");
	    exit(1);
    } 
    
    newNode->id = tail;
    newNode->nextNode = NULL;
    newNode->nextEdgeOut = NULL;

    return newNode;
}

Node *insertNode(Node *listHead, Node *newNode){ //Insert the new Node in the end of the Nodes List

    Node *auxH, *auxT;

    if(listHead == NULL){
        listHead = newNode;
    }else{
        auxH = listHead;
        auxT = listHead->nextNode;
        while(auxT != NULL){
            auxH = auxT;
            auxT = auxT->nextNode;
        }
        auxH->nextNode= newNode;
    }
    return listHead;
}

Edge *createEdge(int head, int width, int length, Node *dest){ 

    Edge *newEdge;

    if((newEdge = (Edge*) calloc(1, sizeof(Edge))) == NULL){ //Create a new Edgeacent
		printf("It was not possible to allocate memory\n");
	    exit(1);
    }

    newEdge->dest = head;
    newEdge->width = width;
    newEdge->length = length;
    newEdge->An = 0;
    newEdge->destNode = dest;
    newEdge->nextEdge = NULL;

    return newEdge;
}

Edge *insertEdge(Edge *listHead, Edge *newEdge){ 

    Edge *auxH, *auxT;

    if(listHead == NULL){
        listHead = newEdge;
    }else{
        auxH = listHead;
        auxT = listHead->nextEdge;
        while(auxT != NULL){
            auxH = auxT;
            auxT = auxT->nextEdge;
        }
        auxH->nextEdge = newEdge;
    }
    return listHead;
}

Edge *searchEdge(Edge *listHead, int id){

    Edge *auxT;

    if(listHead == NULL){
        return NULL;
    }else{     
        auxT = listHead;
        if(auxT->dest == id)
            return auxT;
        while(auxT->nextEdge != NULL){
            auxT = auxT->nextEdge;
            if( auxT->dest == id ) 
                return auxT;
        }
    }
    return NULL;
}

ForwardTable *updateForwardTable(Node *node, Event *eventsHead){

    ForwardTable *node_ft = NULL;   
    RoutingTable *node_rt =NULL;
    Edge *edge=NULL;

    node_ft = searchDestiny(node->nextDest, eventsHead->msg[0]);
    if(node_ft == NULL){ //If destination does not exist in forwarding table
        node->nextDest = createDestiny(node->nextDest, node->nextEdgeOut, eventsHead);
        if(flag_sim=='w'){
            edge = searchEdge(node->nextEdgeOut, eventsHead->origin);
            node_rt = createRT(node->nextDest->nextRoute, edge, eventsHead);
            node->nextDest->nextRoute = node_rt;
        }
        return node->nextDest;
    }else{
        //printf("node=%d\n", node->id);
        // If not, add it
        // If yes, update RT
            // Check if is better estimate, if yes uptadte FT
        node_ft = updateEstimate(node_ft, node->nextEdgeOut, eventsHead);
        return node_ft;
    }

    return NULL;
}

ForwardTable *searchDestiny(ForwardTable *ftHead, int dest_id){

    ForwardTable *auxT;

    if(ftHead == NULL){
        return NULL;
    }else{     
        auxT = ftHead;
        if(auxT->dest == dest_id)
            return auxT;
        while(auxT->nextDest != NULL){
            auxT = auxT->nextDest;
            if( auxT->dest == dest_id ) 
                return auxT;
        }
    }
    return NULL;
}

RoutingTable *searchRoute(RoutingTable *rtHead, int dest_id, int nextHop){

    RoutingTable *auxT;

    if(rtHead == NULL){
        return NULL;
    }else{     
        auxT = rtHead;
        if(auxT->dest == dest_id && auxT->nextHop == nextHop)
            return auxT;
        while(auxT->nextDest != NULL){
            auxT = auxT->nextDest;
            if( auxT->dest == dest_id && auxT->nextHop == nextHop) 
                return auxT;
        }
    }
    return NULL;
}

// COLOCAR FLAG PARA DIFERENCIAR SHORTEST and WIDEST
// Create RT to append in FT 
ForwardTable *createDestiny(ForwardTable *ftHead, Edge *edgesHead, Event *event){

    int aux_width=0;
    ForwardTable *newDest = NULL;
    Edge *edge = NULL;

    if((newDest = (ForwardTable*) calloc(1, sizeof(ForwardTable))) == NULL){   
        printf("It was not possible to allocate memory\n");
        exit(1);
    }
    edge = searchEdge(edgesHead, event->origin);
    if(edge == NULL){
        return NULL;
    }  
    if(edge->width > event->msg[2]){
        aux_width = event->msg[2];
    }else{
        aux_width = edge->width;
    }
    newDest->dest = event->msg[0];
    newDest->cost_l = event->msg[1] + edge->length;
    newDest->cost_w = aux_width;
    newDest->nextHop = event->origin;
    newDest->stab_time = event->An;
    newDest->hop = event->originPointer;
    newDest->nextDest = NULL;

    if(ftHead == NULL){
        ftHead = newDest;
    }else{
        newDest->nextDest = ftHead;
        ftHead = newDest;
    }

    return ftHead;
}

RoutingTable *createRT(RoutingTable *rtHead, Edge *edge, Event *event){

    int aux_width=0;
    RoutingTable *newRoute = NULL;

    if((newRoute = (RoutingTable*) calloc(1, sizeof(RoutingTable))) == NULL){   
        printf("It was not possible to allocate memory\n");
        exit(1);
    }
    if(edge->width > event->msg[2]){
        aux_width = event->msg[2];
    }else{
        aux_width = edge->width;
    }
    newRoute->dest = event->msg[0];
    newRoute->cost_l = event->msg[1] + edge->length;
    newRoute->cost_w = aux_width;
    newRoute->nextHop = event->origin;
    newRoute->hop = event->originPointer;
    newRoute->nextDest = NULL;

    if(rtHead == NULL){
        rtHead = newRoute;
    }else{
        newRoute->nextDest = rtHead;
        rtHead = newRoute;
    }

    return rtHead;
}

RoutingTable *searchRouteNeighbour(RoutingTable *rtHead, int cost_w, int cost_l){

    RoutingTable *auxT, *aux=NULL;

    if(rtHead == NULL){
        return NULL;
    }else{     
        auxT = rtHead;
        if(auxT->cost_w >= cost_w && auxT->cost_l < cost_l)
            aux=auxT;
        while(auxT->nextDest != NULL){
            auxT = auxT->nextDest;
            if(auxT->cost_w >= cost_w && auxT->cost_l < cost_l) 
                aux=auxT;
        }
    }
    return aux;
}

// Alterar o Update Estimate, ter de percorrer a RT table antes de alterar
ForwardTable *updateEstimate(ForwardTable *dest, Edge *edgeHead , Event *event){

    int aux_width=0;
    Edge *edge = NULL;
    RoutingTable *node_rt =NULL, *aux=NULL;

    edge = searchEdge(edgeHead, event->origin);
    if(edge == NULL){
        return NULL;
    }
    //printf("edgewidht=%d\n",edge->width);

    //operation of extention on width, we get the minimum capacity
    if(edge->width > event->msg[2]){
        aux_width = event->msg[2];
    }else{
        aux_width = edge->width;
    }
    //printf("auxwidht=%d\n",aux_width);
    //printf("deswidht=%d\n",dest->cost_w);
    //printf("inicio: %d\t%d\t%d\t%d\t%d\n", dest->dest, dest->cost_w, dest->cost_l, dest->nextHop, dest->hop->id);
    if(flag_sim=='l'){
        if(dest->cost_l > event->msg[1] + edge->length){
            dest->cost_l = event->msg[1] + edge->length;
            dest->hop = event->originPointer;
            dest->nextHop = event->origin;
            dest->stab_time = event->An;
            dest->cost_w = aux_width;
            return dest;
        }else if(dest->cost_l == event->msg[1] + edge->length){
            if(dest->cost_w < aux_width){
                dest->cost_w = aux_width;
                dest->cost_l = event->msg[1] + edge->length;
                dest->hop = event->originPointer;
                dest->stab_time = event->An;
                dest->nextHop = event->origin;
                return dest;
            }
            return NULL;
        }
        return NULL;
    }else{
        if(dest->cost_w < aux_width){
            dest->cost_w = aux_width;
            dest->cost_l = event->msg[1] + edge->length;
            dest->hop = event->originPointer;
            dest->stab_time = event->An;
            dest->nextHop = event->origin;

            node_rt = searchRoute(dest->nextRoute, event->msg[0], event->origin);
            if(node_rt == NULL){
                node_rt = createRT(dest->nextRoute, edge, event);
                dest->nextRoute = node_rt;
            }else{ //if it already exists updade cost_l, cost_w nver chages for the same edge
                node_rt->cost_l = event->msg[1] + edge->length;
            }
            return dest;
        }else if(dest->cost_w == aux_width){
            if(dest->cost_l > event->msg[1] + edge->length){
                dest->cost_l = event->msg[1] + edge->length;
                dest->hop = event->originPointer;
                dest->nextHop = event->origin;
                dest->stab_time = event->An;
                dest->cost_w = aux_width;

                node_rt = searchRoute(dest->nextRoute, event->msg[0], event->origin);
                if(node_rt == NULL){
                    node_rt = createRT(dest->nextRoute, edge, event);
                    dest->nextRoute = node_rt;
                }else{ //if it already exists updade cost_l, cost_w nver chages for the same edge
                    node_rt->cost_l = event->msg[1] + edge->length;
                }
                return dest;
            }else if(dest->nextHop == event->origin){
                //printf("inicio: %d\t%d\t%d\t%d\t%d\n", dest->dest, dest->cost_w, dest->cost_l, dest->nextHop, dest->hop->id);
                dest->cost_l = event->msg[1] + edge->length;
                dest->stab_time = event->An;

                node_rt = searchRoute(dest->nextRoute, event->msg[0], event->origin);
                if(node_rt == NULL){
                    node_rt = createRT(dest->nextRoute, edge, event);
                    dest->nextRoute = node_rt;
                }else{ //if it already exists updade cost_l, cost_w nver chages for the same edge
                    node_rt->cost_l = event->msg[1] + edge->length;
                }

                aux = searchRouteNeighbour(dest->nextRoute, dest->cost_w, dest->cost_l);
                if(aux != NULL){
                    dest->cost_l = aux->cost_l;
                    dest->cost_w = aux->cost_w;
                    dest->hop = aux->hop;
                    dest->nextHop = aux->nextHop;
                    dest->stab_time = event->An;
                }
                //printf("fim:\t%d\t%d\t%d\t%d\t%d\n", dest->dest, dest->cost_w, dest->cost_l, dest->nextHop, dest->hop->id);
                return dest;
            }
            return NULL;
        }
        return NULL;
    }
    //printf("fim:\t%d\t%d\t%d\t%d\t%d\n", dest->dest, dest->cost_w, dest->cost_l, dest->nextHop, dest->hop->id);
}


void printFT(Graph *Head){

    Node *auxN; 
    ForwardTable *auxFT;

    for(auxN=Head->nextNode; auxN!=NULL; auxN=auxN->nextNode){
        printf("Forwarding Table Node %d:\n", auxN->id);
        printf("Dest\tWidth\tLength\tNextHop\ttime\n");
        for(auxFT=auxN->nextDest; auxFT!=NULL; auxFT=auxFT->nextDest){
            printf("%d\t%d\t%d\t%d\t%f\n", auxFT->dest, auxFT->cost_w, auxFT->cost_l, auxFT->nextHop, auxFT->stab_time);
        }
    }
}

void printGraph(Graph *Head){

    Node *auxN; 
    Edge *auxE;

    for(auxN=Head->nextNode; auxN!=NULL; auxN=auxN->nextNode){
        printf("Edges of Node %d:\n", auxN->id);
        printf("Out\n");
        printf("Dest\tWidth\tLength\tDestPoint\n");
        for(auxE=auxN->nextEdgeOut; auxE!=NULL; auxE=auxE->nextEdge){
            printf("%d\t%d\t%d\t%d\n", auxE->dest, auxE->width, auxE->length, auxE->destNode->id);
        }
        printf("In\n");
        printf("Dest\tWidth\tLength\tDestPoint\n");
        for(auxE=auxN->nextEdgeIn; auxE!=NULL; auxE=auxE->nextEdge){
            printf("%d\t%d\t%d\t%d\n", auxE->dest, auxE->width, auxE->length, auxE->destNode->id);
        }
    }
}

void freeGraph(Node *listHead){

    Node *auxN;
    Edge *auxEHeadIn, *auxEIn;
    Edge *auxEHeadOut, *auxEOut;
    ForwardTable *auxFTHead, *auxFT;
    RoutingTable *auxRTHead, *auxRT;

    while(listHead!= NULL){
        auxN=listHead;
        auxEHeadIn = listHead->nextEdgeIn;
        while(auxEHeadIn!=NULL){
            auxEIn = auxEHeadIn;
            auxEHeadIn = auxEIn->nextEdge;
            free(auxEIn);
        }
        auxEHeadOut = listHead->nextEdgeOut;
        while(auxEHeadOut!=NULL){
            auxEOut = auxEHeadOut;
            auxEHeadOut = auxEOut->nextEdge;
            free(auxEOut);
        }
        auxFTHead = listHead->nextDest;
        while(auxFTHead!=NULL){
            auxFT = auxFTHead;
            auxRTHead = auxFTHead->nextRoute;
            while(auxRTHead!=NULL){
                auxRT = auxRTHead;
                auxRTHead = auxRT->nextDest;
                free(auxRT);
            }
            auxFTHead = auxFT->nextDest;
            free(auxFT);
        }

        listHead=listHead->nextNode;
        free(auxN); 
    }
}


