#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

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

typedef struct Nod Nod;
struct Nod
{
	Masina info;
	Nod* next;
	Nod* prev;
};

typedef struct ListaDubla LD;
struct ListaDubla
{
	Nod* first;
	Nod* last;
};

void afisareLista(LD lista)
{
	Nod* aux = lista.first;
	while (aux)
	{
		afisareMasina(aux->info);
		aux = aux->next;
		printf("\n");
	}
}

typedef struct HashTable HashTable;
struct HashTable
{
	int dim;
	LD* vector;//pointer la lista de adrese
};

HashTable initializareHash(int dim)
{
	HashTable ht;
	ht.dim = dim;
	ht.vector = malloc(sizeof(LD) * dim);
	for (int i = 0; i < dim; i++)
	{
		ht.vector[i].first = NULL;
		ht.vector[i].last = NULL;
	}
	return ht;
}
void adaugaInLista(LD* lista, Masina m)
{
	Nod* nou = malloc(sizeof(Nod));
	nou->next = NULL;
	nou->info = m;
	nou->prev = lista->last;
	if (lista->first == NULL)
		lista->first = nou;
	else
		lista->last->next = nou;
	lista->last = nou;
}
int calculeazaHash(int id, int dim)
{
	return (id * 13) % dim;
}
void inserareInHash(HashTable ht,Masina m)
{
	int hashCode = calculeazaHash(m.id, ht.dim);
	adaugaInLista(&ht.vector[hashCode], m);
}

HashTable citireHash(const char* numeFisier)
{
	Masina m;
	HashTable ht = initializareHash(5);
	FILE* f = fopen(numeFisier, "r");
	if (f)
	{
		while (!feof(f))
		{
			m = citireMasinaDinFisier(f);
			inserareInHash(ht, m);
		}
	}
	fclose(f);
	return ht;
}
void afisareHash(HashTable ht)
{
	for (int i = 0; i < ht.dim; i++)
	{
		printf("cluster %d: \n", i);
		afisareLista(ht.vector[i]);
		printf("\n");
	}
}

void dezalocareHash(HashTable* ht)
{
	for (int i = 0; i < ht->dim; i++)
	{
		if (ht->vector[i].first!=NULL)//i definit vectorul ca LD* vector (un vector de structuri, nu de pointeri), atunci ht->vector[i] este o structură, nu un pointer.
			//ht->vector[i] este obiectul în sine. Pentru a ajunge la first, trebuie să folosești punctul.
		{
			Nod* aux = ht->vector[i].first;
			while (aux)
			{
				Nod* sters = aux;
				free(sters->info.model);
				free(sters->info.numeSofer);
				aux = aux->next;
				free(sters);
			}
			ht->vector[i].first = NULL;
			ht->vector[i].last = NULL;
		}
	}
	free(ht->vector);
	ht->vector = NULL;
	ht->dim = 0;
}

void stergeDupaID(HashTable* ht,int id)
{
	int hash = calculeazaHash(id, ht->dim);
	Nod* aux = ht->vector[hash].first;
	int ok = 0;
	while (aux && ok==0)
	{
		if (aux->info.id == id)
		{
			if (aux->prev == NULL)
			{
				ht->vector[hash].first = NULL;
				ht->vector[hash].last = NULL;
				break;
			}
			if (aux->next == NULL)
			{
				aux->prev->next = NULL;
				ht->vector[hash].last = NULL;
				break;
			}
			aux->prev->next = aux->next;
			aux->next->prev = aux->prev;
			free(aux->info.model);
			free(aux->info.numeSofer);
			free(aux);
			ok = 1;
		}
		aux = aux->next;
	}
}
int main()
{
	HashTable ht = citireHash("masini.txt");
	//afisareHash(ht);
	stergeDupaID(&ht, 10);
	afisareHash(ht);
}