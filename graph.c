/******************************************************************************
 *
 * File Name: graph.c
 * Authors: Ana Catarina Grilo & Margarida Fernandes
 *
 *****************************************************************************/

#include "graph.h"
#include "calendar.h"

extern char flag_sim;

/* Create a new graph */
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

/* check if a node already exits in graph */
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

/* Create a new node */
Node *createNode(int tail){

    Node *newNode = NULL;
    
    if((newNode = (Node*) calloc(1, sizeof(Node))) == NULL){ 
        printf("It was not possible to allocate memory\n");
	    exit(1);
    } 
    
    newNode->id = tail;
    newNode->visited = -1;
    newNode->visitedLength = -1;
    newNode->nextNode = NULL;
    newNode->nextEdgeOut = NULL;

    return newNode;
}

/* Insert new Node in the end of the Nodes List */
Node *insertNode(Node *listHead, Node *newNode){

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

/* Create a new edge*/
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

/* Insert new edge in the end of the egde list of a node */
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

/* Search for a edge in edge list */
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

/* Updates ForwardTable list */
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
        node_ft = updateEstimate(node_ft, node->nextEdgeOut, eventsHead);
        return node_ft;
    }

    return NULL;
}

/* Search for a destiny in ForwardTable list*/
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

/* Search for a destiny reached by a specific neighbor in RoutingTable list*/
RoutingTable *searchRoute(RoutingTable *rtHead, int dest_id, int nextHop){

    RoutingTable *auxT, *aux=NULL;

    if(rtHead == NULL){
        return NULL;
    }else{     
        auxT = rtHead;
        if(auxT->dest == dest_id && auxT->nextHop == nextHop)
            aux = auxT;
        while(auxT->nextDest != NULL){
            auxT = auxT->nextDest;
            if( auxT->dest == dest_id && auxT->nextHop == nextHop) 
                aux = auxT;
        }
    }
    return aux;
}

/* Creates a new destiny in ForwardTable list*/
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

/* Creates a new destiny reached by a specific neighbor in RoutingTable list*/
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

/* Searches for a better way to reach a destiny*/
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

/* Updates estimate to reach a destiny if condition to do so are verified*/
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
    //printf("event: origin=%d, length=%d, width=%d\n", event->origin, event->msg[1], event->msg[2]);
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

            node_rt = searchRoute(dest->nextRoute, event->msg[0], event->origin);
            if(node_rt == NULL){
                node_rt = createRT(dest->nextRoute, edge, event);
                dest->nextRoute = node_rt;
            }else{ //if it already exists updade cost_l, cost_w nver chages for the same edge
                node_rt->cost_l = event->msg[1] + edge->length;
            }
            if(dest->cost_l > event->msg[1] + edge->length){
                dest->cost_l = event->msg[1] + edge->length;
                dest->hop = event->originPointer;
                dest->nextHop = event->origin;
                dest->stab_time = event->An;
                dest->cost_w = aux_width;
                return dest;
            }else if(dest->nextHop == event->origin){
                //printf("inicio: %d\t%d\t%d\t%d\t%d\n", dest->dest, dest->cost_w, dest->cost_l, dest->nextHop, dest->hop->id);
                dest->cost_l = event->msg[1] + edge->length;
                dest->stab_time = event->An;
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
        printf("\nForwarding Table Node %d:\n", auxN->id);
        printf("Dest\tWidth\tLength\tNextHop\ttime\n");
        for(auxFT=auxN->nextDest; auxFT!=NULL; auxFT=auxFT->nextDest){
            printf("%d\t%d\t%d\t%d\t%f\n", auxFT->dest, auxFT->cost_w, auxFT->cost_l, auxFT->nextHop, auxFT->stab_time);
        }
    }
}

void printFT_SW(Graph *Head){

    Node *auxN; 
    ForwardTable *auxFT;

    for(auxN=Head->nextNode; auxN!=NULL; auxN=auxN->nextNode){
        printf("\nForwarding Table Node %d:\n", auxN->id);
        printf("Dest\tWidth\tLength\n");
        for(auxFT=auxN->nextDest; auxFT!=NULL; auxFT=auxFT->nextDest){
            printf("%d\t%d\t%d\n", auxFT->dest, auxFT->cost_w, auxFT->cost_l);
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

void printPath(Node *node, int dest_id){

    ForwardTable *auxT;

    if(node == NULL){
        return;
    }else if(node->id == dest_id){
        printf("%d\n", node->id);
        printf("\n\n");
        return;
    }else{     
        auxT = node->nextDest;
        if(auxT->dest == dest_id){
            if(auxT->hop->id != dest_id)
                printf("%d->", auxT->nextHop);
            printPath(auxT->hop, dest_id);
            return;
        }
        while(auxT->nextDest != NULL){
            auxT = auxT->nextDest;
            if(auxT->dest == dest_id){
                if(auxT->hop->id != dest_id)
                    printf("%d->", auxT->nextHop);
                printPath(auxT->hop, dest_id);
                return;
            }
        }
    }


    return;
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


