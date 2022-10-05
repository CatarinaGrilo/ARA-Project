/******************************************************************************
 *
 * File Name: calendar.h
 * Authors: Ana Catarina Grilo & Margarida Fernandes
 *
 *****************************************************************************/

#ifndef CALENDAR_INCLUDED
#define CALENDAR_INCLUDED

#include "main.h"

Event *createEvent(Event *eventsHead, Node *wakeupNode, Edge *neighbour, int dest, int cost_l, int cost_w);

Event *insertEvent_ordered(Event *Listhead, Event *newEvent);

void calendarEvolution(Event *eventsHead, Node *wakeupNode);

Event *announceNode(Event *eventsHead, Node *wakeupNode);

Event *doEvent(Event *eventsHead, Node *nodeDest);

Event *sendToNeighbour(Event *eventsHead, Node *origin, ForwardTable *node_Ftable);

Event *popEvent(Event *eventsHead);

#endif