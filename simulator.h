/******************************************************************************
 *
 * File Name: simulator.h
 * Authors: Ana Catarina Grilo & Margarida Fernandes
 *
 *****************************************************************************/

#ifndef SIMULATOR_INCLUDED
#define SIMULATOR_INCLUDED

#include "main.h"

void simulations(Node *nodeHead, Event *eventHead);

void simulationsInteractive(Node *destNode, Event *eventHead);

void stats(Graph *Head);

#endif