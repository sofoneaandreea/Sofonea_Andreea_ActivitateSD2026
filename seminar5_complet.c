
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

typedef struct Nod Nod;
struct Nod
{
	Masina info;
	Nod* prev;
	Nod* next;
};

typedef struct ListaDubla LD;

struct ListaDubla
{
	Nod* first;
	Nod* last;
};

Masina citireMasinaDinFisier(FILE* file) 
{
	char linie[256];
	char delimitator[3] = ",\n";
	fgets(linie, 255, file);
	Masina m;
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

void afisareListaMasini(LD lista) 
{
	
		Nod* aux = lista.first;
		while (aux)
		{
			afisareMasina(aux->info);
			aux = aux->next;
		}
}


void adaugaInListaLaFinal( LD* lista,Masina masinaNoua) 
{
	Nod* temp = malloc(sizeof(Nod));
	temp->info = masinaNoua;
	temp->next = NULL;
	temp->prev = lista->last;
	if (lista->last)
		lista->last->next = temp;
	else
		lista->first = temp;
	lista->last = temp;
	
}

void adaugaLaInceputInLista(LD*lista, Masina masinaNoua)
{
	Nod* temp = malloc(sizeof(Nod));
	temp->info = masinaNoua;
	temp->prev = NULL;
	temp->next = lista->first;
	if (lista->first)
		lista->first->prev = temp;
	else
		lista->last = temp;
	lista->first = temp;
}

LD citireLDMasiniDinFisier(const char* numeFisier) 
{
	FILE* f = fopen(numeFisier, "r");
	LD lista;
	lista.first = NULL;
	lista.last = NULL;
	while (!feof(f))
	{
		Masina m = citireMasinaDinFisier(f);
		adaugaInListaLaFinal(&lista, m);
	}
	fclose(f);
	return lista;
}

void dezalocareLDMasini(LD* lista) 
{
	if (lista == NULL|| lista->first==NULL)
		printf("lista e goala");
	else
	{
		Nod* aux = lista->first->next;
		while (aux)
		{
			free(aux->prev->info.model);
			free(aux->prev->info.numeSofer);
			free(aux->prev);
			aux = aux->next;
		}
		free(lista->last->info.model);
		free(lista->last->info.numeSofer);
		free(lista->last);
		lista->first = NULL;
		lista->last = NULL;
	}
}

float calculeazaPretMediu(LD lista) 
{
	Nod* p = lista.first;
	float suma = 0;
	int nr = 0;
	while (p)
	{
		suma = suma + p->info.pret;
		nr++;
		p = p->next;
	}
	if (nr == 0)
		return 0;
	else
		return suma / nr;

}

void stergeMasinaDupaID(LD* lista, int id) 
{
	while (lista && lista->first->info.id == id)
	{
		Nod* sters = lista->first;
		lista->first = lista->first->next;
		if (lista->first == NULL)
			lista->last = NULL;
		else
			lista->first->prev = NULL;
		free(sters->info.model);
		free(sters->info.numeSofer);
		free(sters);
	}
	Nod* p = lista->first;
	while (p)
	{
		if (p->info.id == id)
		{
			Nod* sters = p;
			p->prev->next = p->next;
			if (p->next != NULL)
				p->next->prev = p->prev;
			else
				lista->last = p->prev;
			p = p->next;//obligatoriu inainte de free- daca dai free inainte pierzi p->next
			free(sters->info.model);
			free(sters->info.numeSofer);
			free(sters);
		}
		else
			p = p->next;
	}
}

char* getNumeSoferMasinaScumpa(LD lista) 
{
	Nod* p = lista.first;
	float max=0;
	char* nume=NULL;
	while (p)
	{
		if (p->info.pret > max)
		{
			max = p->info.pret;
			nume = p->info.numeSofer;
		}
		p = p->next;
	}
	return nume;
}

int main() 
{
	LD lista;
	lista = citireLDMasiniDinFisier("masini.txt");
	afisareListaMasini(lista);
	printf("\npretul mediu este: %5.2f\n", calculeazaPretMediu(lista));
	printf("\nnumele soferului cu masina cea mai scumpa: %s", getNumeSoferMasinaScumpa(lista));
	printf("\nstergere masini cu id 10: \n");
	stergeMasinaDupaID(&lista, 10);
	afisareListaMasini(lista);

	return 0;
}
