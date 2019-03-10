#ifndef PROCESS_H_INCLUDED
#define PROCESS_H_INCLUDED

/** Definirea structurii de tip proces **/

typedef struct {
	char name[21]; // Numele procesului
	int life; // Viata procesului
	int priority; // Prioritatea procesului
}Process;

#endif
