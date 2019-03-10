#include "queue.h"

/** Functiile pentru coada **/

int is_empty(queue *coada)
{
   return (coada == NULL);
}

void afisare(queue *lista)
{
	while(lista != NULL)
	{
		printf("%s %d ", lista->process.name, lista->process.life);
		lista = lista->next;
	}
	printf("\n");
}

queue *enqueue(queue *coada, Process new_proc)
{
    struct queue *tmp, *p;
    tmp=(struct queue *)malloc(sizeof(queue));
    if(tmp==NULL)
    {
        printf("Memory not available\n");
     	exit(1);
    }
    tmp->process = new_proc;
    tmp->next=NULL;
    if(is_empty(coada))      
        return tmp;
	p = coada;

	while(p->next != NULL)
		p = p->next;

	p->next = tmp;

	return coada;

}

queue *dequeue(queue *coada)
{
    struct queue *tmp;
    if( is_empty(coada) )
        return NULL;

    tmp=coada;
    coada= coada->next;
    free(tmp);
    return coada;
}
