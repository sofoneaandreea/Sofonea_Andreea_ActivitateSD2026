//seminar6-stive si cozi

//stiva=sdd,LIFO,reprezentare prin(as putea vector dar nu se fol ptc mereu trb realocat),lista simplu inlantuita(head ul e mereu cel nou adaugat),(lista dubla se poate dar retinerea primului elem e inutila)
//coada=sdd,FIFO,reprezentare prin lista dubla(acces si la primul si la ultimul)
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//trebuie sa folositi fisierul masini.txt
//sau va creati un alt fisier cu alte date

struct StructuraMasina {
	int id;
	int nrUsi;
	float pret;
	char* model;
	char* numeSofer;
	unsigned char serie;
};
typedef struct StructuraMasina Masina;

Masina citireMasinaDinFisier(FILE* file) {
	char buffer[100];
	char sep[3] = ",\n";
	fgets(buffer, 100, file);
	char* aux;
	Masina m1;
	aux = strtok(buffer, sep);
	m1.id = atoi(aux);
	m1.nrUsi = atoi(strtok(NULL, sep));
	m1.pret = atof(strtok(NULL, sep));
	aux = strtok(NULL, sep);
	m1.model = malloc(strlen(aux) + 1);
	strcpy_s(m1.model, strlen(aux) + 1, aux);

	aux = strtok(NULL, sep);
	m1.numeSofer = malloc(strlen(aux) + 1);
	strcpy_s(m1.numeSofer, strlen(aux) + 1, aux);

	m1.serie = *strtok(NULL, sep);
	return m1;
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
struct Nod//lista
{
	Masina masina;
	Nod* next;

};


void pushStack(Nod** cap, Masina masina)
{
	Nod* first = (Nod*)malloc(sizeof(Nod));//nu e neap first??
	first->masina = masina;
	first->next = (*cap);
	(*cap) = first;

}

Masina popStack(Nod** cap)
{

	if ((*cap) == NULL)
	{
		Masina rez;
		rez.id = -1;
		return rez;
	}
	Masina rez = (*cap)->masina;
	Nod* temp = (*cap)->next;
	free(*cap);
	*cap = temp;
	return rez;


}

char isemptyStack(Nod* cap)
{
	return cap == NULL;
}

void* citireStackMasiniDinFisier(const char* numeFisier)
{
	FILE* file = fopen(numeFisier, "r");
	if (!file)
		return NULL;

	Nod* cap = NULL;//asta e stiva
	while (!feof(file))
	{
		Masina masina = citireMasinaDinFisier(file);
		pushStack(&cap, masina);

	}
	fclose(file);
	return cap;


}

void dezalocareStivaDeMasini(/*stiva*/) {
	//sunt dezalocate toate masinile si stiva de elemente
}

int size(/*stiva*/) {
	//returneaza numarul de elemente din stiva
}



//QUEUE
//putem reprezenta o coada prin LSI, LDI sau vector
typedef struct NodDublu NodDublu;
struct NodDublu
{
	Masina info;
	NodDublu* next;
	NodDublu* prev;

};
typedef struct ListaDubla ListaDubla;
struct ListaDubla
{
	NodDublu* first;
	NodDublu* last;
};
void enqueue(ListaDubla* coada, Masina masina)//inserare
{
	NodDublu* nou = malloc(sizeof(NodDublu));
	nou->info = masina;
	nou->next = NULL;
	nou->prev = coada->last;
	if (coada->last)
	{
		coada->last->next = nou;
	}
	else
	{
		coada->first = nou;
	}
	coada->last = nou;
}

Masina dequeue(ListaDubla* coada)//stergere
{
	Masina rez;
	rez.id = -1;
	if (coada->first)
	{
		rez = coada->first->info;
		NodDublu* temp = coada->first;
		coada->first = temp->next;
		free(temp);
	}
	return rez;

}

ListaDubla citireCoadaDeMasiniDinFisier(const char* numeFisier)
{
	ListaDubla coada;
	coada.first = coada.last = NULL;
	FILE* file = fopen(numeFisier, "r");
	if (file)
	{
		while (!feof(file))
		{
			enqueue(&coada, citireMasinaDinFisier(file));
		}
		fclose(file);
	}
	return coada;
}

void dezalocareCoadaDeMasini(/*coada*/) {
	//sunt dezalocate toate masinile si coada de elemente
}


//metode de procesare
Masina getMasinaByID(Nod** stiva, int id)
{
	if ((*stiva) == NULL)
	{
		Masina rez;
		rez.id = -1;
		return rez;
	}
	Nod* stivaNoua = NULL;
	Masina rez;
	while ((*stiva))
	{
		Masina masinaNoua = popStack(stiva);
		if (masinaNoua.id == id)
		{
			rez = masinaNoua;
			break;
		}
		else
			pushStack(&stivaNoua, masinaNoua);
	}
	while (stivaNoua)
	{
		pushStack(stiva, popStack(&stivaNoua));
	}
	return rez;
}

float calculeazaPretTotal(/*stiva sau coada de masini*/);

int main() {

	Nod* stiva = citireStackMasiniDinFisier("masini.txt");
	afisareMasina(popStack(&stiva));
	afisareMasina(popStack(&stiva));

	afisareMasina(getMasinaByID(&stiva, 7));

	printf("\n\ncoada\n\n");
	ListaDubla coada = citireCoadaDeMasiniDinFisier("masini.txt");
}