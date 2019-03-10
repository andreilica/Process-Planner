#ifndef QUEUE_H_INCLUDED
#define QUEUE_H_INCLUDED
#include "Process.h" // Includerea fisierului ce contine structura de tip proces
#include <stdlib.h>
#include <stdio.h>

/** Definirea structurii de coada **/

typedef struct queue
{
	Process process; // Elementele de tip proces
	struct queue *next; // Pointer-ul spre urmatoarea pozitie
} queue;

/** Headerele functiilor pentru coada **/

int is_empty(queue *coada); // Verifica daca o coada este goala
queue *enqueue(queue *coada, Process new_proc); // Adauga un element de tip proces intr-o coada
queue *dequeue(queue *coada); // Sterge un element din coada
void afisare(queue *lista); // Afisarea elementelor unei cozi

#endif