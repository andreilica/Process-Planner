#include <stdio.h> 
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include "queue.h" // Includerea fisierul ce contine structura de coada

#define BUFF_MAX 100 // Dimensiunea string-ului pentru comenzi

/** Functie care este apelata folosind comanda 's' **/

void display(queue *coada, FILE *output, int plan)
{
	queue *ptr;
    if( is_empty(coada) )
    {
        fprintf(output,"\n");
        return;
    }

    for (ptr = coada; ptr != NULL; ptr = ptr->next)
		if (plan == 1 || plan == 2 || plan == 3 || plan == 4)
			fprintf(output, "%s %d\n", ptr->process.name, ptr->process.life);
}

/** Functie care este apelata folosind comanda 'a' **/
/** Adauga elemente noi de tip proces intr-o coada **/

unsigned int add(const char* buff, int pos, queue **coada) 
{

	int x, y;
	unsigned int length;
	char aux[21];
	int pos_aux;
	Process process;
	pos_aux = -1;
	x = y = 0;
	length = strlen(buff);
	while (isalnum(buff[pos]) && pos < length)
	{
		aux[++pos_aux] = buff[pos];
		pos++;
	}
	aux[++pos_aux] = '\0';
	strcpy(process.name, aux);
	pos++;
	while (isdigit(buff[pos]) && pos < length)
	{
		x = x * 10 + buff[pos] - '0';
		pos++;
	}
	pos++;
	while (isdigit(buff[pos]) && pos < length)
	{
		y = y * 10 + buff[pos] - '0';
		pos++;
	}
	process.life = x;
	process.priority = y;
	(*coada) = enqueue(*coada,process);
	return pos;
}

/** Functie care este apelata folosind comanda 'ma' **/
/** Adauga elemente noi de tip proces intr-o coada, mai multe odata.**/

void multiple_add(const char* buff, int pos, queue **coada)
{
	unsigned int length;
	length = strlen(buff);
	while (pos < length)
	{
		pos = add(buff, pos, coada);
		pos++;
	}
}

/** Functie care este apelata folosind comanda 'e' **/
/** Trezeste un proces dat prin denumire si il muta din starea WAITING in starea READY **/

void event(const char* buff, int pos, queue **waiting, queue **ready)
{
	
	Process proc;
	unsigned int length;
	char aux[21];
	int pos_aux;
	pos_aux = -1;
	length = strlen(buff);
	while (isalnum(buff[pos]) && pos < length)
	{
		aux[++pos_aux] = buff[pos];
		pos++;
	}
	aux[++pos_aux] = '\0';

	queue *ptr, *ptr2;
	ptr = (*waiting);

	if(is_empty(*waiting))
	{
		*waiting = NULL;
		return;
	}

	if (strcmp(ptr->process.name, aux) == 0 )
	{
		proc = ptr->process;
		(*ready) = enqueue(*ready, proc);
		(*waiting) = dequeue(*waiting);
		return;
	}

	while (ptr != NULL && strcmp(ptr->process.name, aux))
	{
		ptr2 = ptr;
		ptr = ptr->next;
	}

	if (ptr == NULL)
		return;
	else 
	{
		ptr2->next = ptr->next;
		(*ready) = enqueue(*ready, ptr->process);
		free(ptr);
	}
}

/** Functie folosita pentru a afla procesul cu viata minima dintr-o coada. **/

Process find_min_life(queue *coada)
{
	Process min_proc;

	queue *p = coada;
	min_proc = coada -> process;

	while(p != NULL)
	{
		if(p->process.life < min_proc.life)
			min_proc = p->process;

		p = p->next;
	}

	return min_proc;
}	

/** Functie folosita pentru a afla procesul cu prioritatea maxima dintr-o coada. **/

Process find_max_priority(queue *coada)
{
	Process max_proc;

	queue *p = coada;
	max_proc = coada -> process;

	while(p != NULL)
	{
		if(p->process.priority > max_proc.priority)
			max_proc = p->process;

		p = p->next;
	}

	return max_proc;
}	

/** Functie folosita pentru a sterge un anumit element de tip proces, primit ca paramentru, dintr-o coada.**/

queue *elem_del(queue *coada, Process elem)
{
	if(coada == NULL)
		return NULL;

	queue *p = coada;
	queue *aux;

	if(strcmp(p->process.name,elem.name) == 0 )
		return dequeue(coada);

	while(p != NULL)
	{
		if(strcmp(p->next->process.name,elem.name) == 0 )
		{
			aux = p -> next;
			p->next = p->next->next;
			free(aux);
			break;
		}
		p = p -> next;
	}

	return coada;
}

/** Functie care este apelata folosind comanda 't' **/
/** Trece o unitate de timp si face modificarile necesare in cozi, in functie de tipul planificarii ( primit ca parametru ). **/

void tick(queue **ready, queue** running, queue **terminated, int plan, int cuanta, int level, int *flag) 
{
	Process proc;
	Process proc_aux;

	if(plan == 1) // Planificare FCFS
	{
		if (is_empty(*running))
		{
			if (!is_empty(*ready))
			{
				proc = (*ready)->process;
				(*ready) = dequeue(*ready);
				(*running) = enqueue(*running, proc);
			}
		}
		else
		{
			
			if (--((*running)->process.life) == 0)
			{
				proc = (*running)->process;
				(*running) = dequeue(*running);
				(*terminated) = enqueue(*terminated, proc);

				if (is_empty(*running))
				{
					if (!is_empty(*ready))
					{
						proc = (*ready)->process;
						(*ready) = dequeue(*ready);
						(*running) = enqueue(*running, proc);
					}
				}
			}
		}
	}


	if(plan == 2) // Planificare SJF
	{
		if (is_empty(*running))
		{
			if (!is_empty(*ready))
			{
				proc = find_min_life(*ready);
				(*ready) = elem_del(*ready, proc);
				(*running) = enqueue(*running, proc);
			}
		}
		else
		{
			
			if (--((*running)->process.life) == 0)
			{
				proc = (*running)->process;
				(*running) = dequeue(*running);
				(*terminated) = enqueue(*terminated, proc);

				if (is_empty(*running))
				{
					if (!is_empty(*ready))
					{
						proc = find_min_life(*ready);
						(*ready) = elem_del(*ready, proc);
						(*running) = enqueue(*running, proc);
					}
				}
			}
		}
	}


	if(plan == 3) // Planificare Round Robin
    {
        if (is_empty(*running))
        {
            if (!is_empty(*ready))
            {
            	*flag = 0;
                proc = (*ready)->process;
                (*ready) = dequeue(*ready);
                (*running) = enqueue(*running, proc);
            }
        }
        else
        {
 
            (*running)->process.life--;
            (*flag)++;
 
            if ( (*running)->process.life == 0)
            {
            	*flag = 0;
                proc = (*running)->process;
                (*running) = dequeue(*running);
                (*terminated) = enqueue(*terminated, proc);
 
                if (is_empty(*running))
                {
                    if (!is_empty(*ready))
                    {
                    	*flag = 0;
                        proc = (*ready)->process;
                        (*ready) = dequeue(*ready);
                        (*running) = enqueue(*running, proc);
                    }
                }
            }
 
            if( (*flag) == cuanta && !is_empty(*running))
            {
                (*flag) = 0;
                proc = (*running)->process;
                (*running) = dequeue(*running);
                (*ready) = enqueue(*ready, proc);
 
                if (is_empty(*running))
                {
                    if (!is_empty(*ready))
                    {
                    	*flag = 0;
                        proc = (*ready)->process;
                        (*ready) = dequeue(*ready);
                        (*running) = enqueue(*running, proc);
                    }
                }
            }
 
        }
    }


    if(plan == 4) // Planificare cu prioritati
    {
        if (is_empty(*running))
        {
            if (!is_empty(*ready))
            {
            	*flag = 0;
                proc = find_max_priority(*ready);
				(*ready) = elem_del(*ready, proc);
				(*running) = enqueue(*running, proc);
            }
        }
        else
        {
 
            (*running)->process.life--;
            (*flag)++;
 
            if ( (*running)->process.life == 0)
            {
            	*flag = 0;
                proc = (*running)->process;
                (*running) = dequeue(*running);
                (*terminated) = enqueue(*terminated, proc);
 
                if (is_empty(*running))
                {
                    if (!is_empty(*ready))
                    {
                    	*flag = 0;
                        proc = find_max_priority(*ready);
						(*ready) = elem_del(*ready, proc);
						(*running) = enqueue(*running, proc);
                    }
                }
            }
 
            if( (*flag) == cuanta && !is_empty(*running))
            {
                (*flag) = 0;
                proc = (*running)->process;
                (*running) = dequeue(*running);
                (*ready) = enqueue(*ready, proc);
 
                if (is_empty(*running))
                {
                    if (!is_empty(*ready))
                    {
                    	*flag = 0;
                        proc = find_max_priority(*ready);
						(*ready) = elem_del(*ready, proc);
						(*running) = enqueue(*running, proc);
                    }
                }
            }

            if(!is_empty(*ready))
            {
            	proc = find_max_priority(*ready);

            	if(proc.priority > (*running) -> process.priority)
            	{
            		(*flag) = 0;
            		proc_aux = (*running) -> process;
            		(*running) = dequeue(*running);
            		(*ready) = dequeue(*ready);
            		(*ready) = enqueue(*ready, proc_aux);
            		(*running) = enqueue(*running, proc);

            	}
            }
 
        }
    }


}

/** Functie care este apelata folosind comanda 'e' **/
/** Adoarme un proces din starea RUNNING in starea WAITING **/

void wait(queue **running, queue **waiting)
{
	if(!is_empty(*running))
	{
		*waiting = enqueue(*waiting, (*running)->process);
		(*running) = dequeue(*running);
	}
}

int main(int argc, char const *argv[])
{
	FILE *input;
	FILE *output;

	char buff[BUFF_MAX];
	
	input = fopen(argv[1], "r"); // Fisierul de input
	output = fopen(argv[2], "w"); // Fisierul de output

 	int pos, x, y, plan, cuanta, level;
	int flag = 0;
	unsigned int length;

	queue *ready = NULL, *running = NULL, *terminated = NULL, *waiting = NULL;   // Declararea cozilor folosite in program

	if (!output)
   	{
       fprintf(stderr, "Eroare! Nu am putut deschide fisierul destinatie!\n");
       return 0;
   	}

   	/** Parsarea documentului de input si stabilirea tipului planificarii si comenzilor primite **/

	if(input == NULL)
	{
		fprintf(stderr, "Eroare, nu s-a putut deschide fisierul de input\n");
		return 0;
	}
	else
	{
		if (fgets(buff, BUFF_MAX, input) != NULL)
		{
			switch(buff[0])
			{
				case '1':
					printf("%s\n", "FCFS");
					plan = 1;
				break;
				case '2' :
					printf("%s\n", "SJF");
					plan = 2;
				break;	
				case '3' :
					printf("%s\n", "Round Robin");
					plan = 3;
					pos = 2;
					x = 0;
					length = strlen(buff);
					while (isdigit(buff[pos]) && pos < length)
					{
						x = x * 10 + buff[pos] - '0';
						pos++;
					}

					cuanta = x;
					
				break;
				case '4' :
					printf("%s\n", "Priority planification");
					plan = 4;
					pos = 2;
					x = y = 0;
					length = strlen(buff);
					while (isdigit(buff[pos]) && pos < length)
					{
						x = x * 10 + buff[pos] - '0';
						pos++;
					}
					pos++;
					while (isdigit(buff[pos]) && pos < length)
					{
						y = y * 10 + buff[pos] - '0';
						pos++;
					}
					cuanta = x;
					level = y;
				break;
				default: break;
			}
		}
		while(fgets(buff, BUFF_MAX, input) != NULL)
		{
			switch(buff[0])
			{
				case 'a':
					add(buff, 2, &ready);
					tick(&ready, &running, &terminated, plan, cuanta, level, &flag);
					break;
				case 'm':
					multiple_add(buff, 3, &ready);
					tick(&ready, &running, &terminated, plan, cuanta, level, &flag);
					break;
				case 'e':
					event(buff,2, &waiting, &ready);
					tick(&ready, &running, &terminated, plan, cuanta, level, &flag);
					break;
				case 't':
					tick(&ready, &running, &terminated, plan, cuanta, level, &flag);
					break;
				case 's':
					display(running, output, plan);
					break;
				case 'w':
					wait(&running, &waiting);
					tick(&ready, &running, &terminated, plan, cuanta, level, &flag);
					break;
				default:
				break;
			}
		}
	}

	/** Eliberarea de memorie prin golirea cozilor folosite in program pentru a evita memory leak-urile **/

	while (!is_empty(ready))
		ready = dequeue(ready);
	while (!is_empty(running))
		running = dequeue(running);
	while (!is_empty(terminated))
		terminated = dequeue(terminated);
	while (!is_empty(waiting))
		waiting = dequeue(waiting);	

	/** Inchiderea fisierelor folosite **/

	fclose(input);	
	fclose(output);
	
	return 0;
}