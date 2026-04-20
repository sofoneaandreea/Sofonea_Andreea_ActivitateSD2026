#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>



struct StructuraMasina {
	int id;
	int nrUsi;
	float pret;
	char* model;
	char* numeSofer;
	unsigned char serie;
};
typedef struct StructuraMasina Masina;

Masina citireMasinaDinFisier(FILE* file) 
{
	char delimitator[3] = ",\n";
	char linie[256];
	Masina m;
	fgets(linie, 255, file);
	m.id = atoi(strtok(linie, delimitator));
	m.nrUsi = atoi(strtok(NULL, delimitator));
	m.pret = atof(strtok(NULL, delimitator));
	char* aux = strtok(NULL, delimitator);
	m.model = malloc((strlen(aux) + 1) * sizeof(char));
	strcpy(m.model, aux);
	aux = strtok(NULL, delimitator);
	m.numeSofer = malloc((strlen(aux) + 1) * sizeof(char));
	strcpy(m.numeSofer, aux);
	m.serie = strtok(NULL, delimitator)[0];
	return m;
}

void afisareMasina(Masina masina) {
	printf("Id: %d\n", masina.id);
	printf("Nr. usi : %d\n", masina.nrUsi);
	printf("Pret: %.2f\n", masina.pret);
	printf("Model: %s\n", masina.model);
	printf("Nume sofer: %s\n", masina.numeSofer);
	printf("Serie: %c\n\n", masina.serie);
}

//STACK
//Alegeti prin ce veti reprezenta stiva si creati structura necesara acestei stive
//putem reprezenta o stiva prin LSI, LDI sau vector
typedef struct Nod Nod;
struct Nod
{
	Masina info;
	Nod* next;
};
void pushStack(Nod** lista, Masina masina) //LIFO-deci se adauga la inceput
{
	Nod* nou = malloc(sizeof(Nod));
	nou->info = masina;
	nou->next = (*lista);
	(*lista) = nou;//nou va fi primul in lista
}

Masina popStack(Nod** lista) 
{
	//trebuie sa verif daca lista e null
	if ((*lista) == NULL)
	{
		Masina m;
		m.id = -1;
		return m;
	}
	Masina m = (*lista)->info;//shallow copy
	Nod* aux = (*lista)->next;
	//pentru pointeri adica model si nume se copiaza adresa de memorie asa ca nu poti da free-se pierde
	//la restul, id, pret etc se copiaza efectiv
	free((*lista));//am eliberat primul elem;MEREU CAND STERGI CEVA TREBUIE FREE
	(*lista) = aux;
	return m;
}

int emptyStack(Nod* lista) 
{
	return lista == NULL;
}

void* citireStackMasiniDinFisier(const char* numeFisier) 
{
	FILE* f = fopen(numeFisier, "r");
	if (!f)
		return;
	Nod* stack=NULL;
	while (!feof(f))
	{
		Masina m;
		m = citireMasinaDinFisier(f);
		pushStack(&stack, m);
	}
	fclose(f);
	return stack;
}

void dezalocareStivaDeMasini(Nod** stack) 
{
	if ((*stack) == NULL)
		printf("stiva e goala");
	else
	{
		
		while (*stack)
		{
			Nod* aux = (*stack);
			free(aux->info.model);
			free(aux->info.numeSofer);
			(*stack) = (*stack)->next;
			free(aux);
		}
//aici nu dau free(*stack) ptc deja e eliberata in while-ai da free(NULL)-cand dai stack=stack next o sa devina NULL
	}
}

int size(Nod* stack) 
{
	int nr=0;
	Nod* aux = stack;
	while (aux)
	{
		nr++;
		aux = aux->next;
	}
	return nr;
}

//QUEUE
typedef struct NodDublu NodD;
struct NodDublu
{
	Masina info;
	NodD* next;
	NodD* prev;
};
typedef struct ListaDubla LD;
struct ListaDubla
{
	NodD* first;
	NodD* last;
};
void enqueue(LD* queue, Masina masina)//FIFO-deci adaug la final
{
	NodD* aux = malloc(sizeof(NodD));
	aux->next = NULL;
	aux->prev = queue->last;
	aux->info = masina;
	if (queue->first == NULL)
	{
		queue->first = aux;
	}
	else
	{
		queue->last->next = aux;
	}
	queue->last = aux;
}

Masina dequeue(LD* queue)//de la inceput
{
	Masina m;
	m.id = -1;
	if (queue->first)
	{
		m = queue->first->info;
		NodD* aux = queue->first;
		queue->first = queue->first->next;
		free(aux);
	}
	return m;
}

LD citireCoadaDeMasiniDinFisier(const char* numeFisier) 
{
	FILE* f = fopen(numeFisier, "r");
	Masina m;
	LD queue;
	if (!f)
		return;
	else
	{
		while (!feof(f))
		{
			m = citireMasinaDinFisier(f);
			enqueue(&queue, m);
		}
	}
	fclose(f);
	return queue;
}

void dezalocareCoadaDeMasini(LD* queue) 
{
	if (queue->first == NULL)
		printf("coada e goala");
	else
	{
		NodD* aux = queue->first;
		while (aux)
		{
			NodD* sters = aux;
			aux = aux->next;
			free(sters->info.model);
			free(sters->info.numeSofer);
			free(sters);
		}
		queue->first = NULL;//folosind aux=first oricum ajunge pe ultimul care e null dar pt siguranta scrii
		queue->last = NULL;//asta e obligatoriu-nodul a fost eliberat cu free,dar variab inca pastreaza adresa daca nu il setezi ca null
		//Acești pointeri nu fac parte din nodul șters.Ei sunt variabile 
		//separate, declarate de tine în main sau în structura de control-adica lista dubla.
		//Aceștia trebuie puși pe NULL pentru că ei rămân în picioare în main.
		//Ei sunt singura cale prin care restul programului știe dacă lista mai există sau nu.
		//stack ul(lista simpla0 fiiind declarata ea ca pointer,la dezalocare nodul de inceput oricum ajunge pe null si de aia nu mai scrii explicit(ca la first)
		//restul nodurilor sunt irelevante-se sterg la free,dar doar first si last trebuie puse explicit cu null ptc altfel ar deveni dangling pointer
		//dangling pointer-tin o adresa "valida" dar la care,de fapt,nu e nimic -
	}
}


//metode de procesare
Masina getMasinaByID(Nod** stack, int id)
{
	Masina m;
	m.id = -1;
	if ((*stack) == NULL)
		return m;
	else
	{
		Nod* stackNou = NULL;
		while (*stack)
		{
		    m = popStack(stack);
			if (m.id == id)
			{
				break;
			}
			else
				pushStack(&stackNou, m);
		}
		while (stackNou)
		{
			pushStack(stack, popStack(&stackNou));
		}
	}
	return m;
}

float calculeazaPretTotal(LD queue)
{
	NodD* aux = queue.first;
	float pretTotal = 0;
	while (aux)
	{
		pretTotal = pretTotal + aux->info.pret;
		aux = aux->next;
	}
	return pretTotal;
}

int main() 
{
	Nod* stack = citireStackMasiniDinFisier("masini.txt");
	printf("masina cu id 5: \n");
	Masina m = getMasinaByID(&stack, 5);
	afisareMasina(m);
	printf("\n");
	int nr = size(stack);
	for (int i = 0; i < nr; i++)
	{
		afisareMasina(popStack(&stack));
		printf("\n");
	}

	LD queue;
	queue = citireCoadaDeMasiniDinFisier("masini.txt");
	printf("\n pret total coada masini: %.2f", calculeazaPretTotal(queue));

	return 0;
}