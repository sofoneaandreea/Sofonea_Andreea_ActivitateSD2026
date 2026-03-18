//LISTA DUBLU INLANTUITA- un tablou unidimenstional/colectie de elem de acelasi tip(omogena),
//nu e contigua si ai acces la elem precedent si urmator
//ca sa parcurgi invers ar trb sa ne ducem pana la capat si apoi sa ne intoarcem=>asa ca vom retine 
//adresa ultimului element=>se face o structura care retine 2 pointeri-primul si ultimul nod

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

struct Nod//nodul(unul singur care tine cele 2 adrese)
{
	struct Nod* prev;
	struct Nod* next;
	Masina info;
};
typedef struct Nod Nod;//alias

struct ListaDubla
{
	Nod* first;
	Nod* last;
};
typedef struct ListaDubla ListaDubla;

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

void afisareListaMasini(ListaDubla listaDubla) 
{
	Nod* aux=listaDubla.first;
	while (aux)
	{
		afisareMasina(aux->info);
		aux = aux->next;
	}
}

void afisareListaMasiniFinal(ListaDubla listaDubla)
{
	Nod *aux = listaDubla.last;
	while (aux)
	{
		afisareMasina(aux->info);
		aux = aux->prev;
	}
}

void adaugaMasinaInLista(ListaDubla * listaDubla, Masina masinaNoua) 
{
	Nod* nou;
	nou = malloc(sizeof(Nod));
	nou->info = masinaNoua;
	nou->prev = listaDubla->last;
	nou->next = NULL;
	if (listaDubla->last)
		listaDubla->last->next = nou;
	else
		listaDubla->first = nou;

	listaDubla->last = nou;
}

void adaugaMasinaInListaLaInceput(ListaDubla* listaDubla, Masina masinaNoua)
{
	Nod* nou;
	nou = malloc(sizeof(Nod));
	nou->info = masinaNoua;
	nou->next = listaDubla->first;
	nou->prev = NULL;
	if (listaDubla->first)
		listaDubla->first->prev = nou;
	else
		listaDubla->last = nou;

	listaDubla->first = nou;
}


ListaDubla citireLDMasiniDinFisier(const char* numeFisier) 
{
	FILE* fisier = fopen(numeFisier, "r");
	ListaDubla lista;
	lista.first = NULL;
	lista.last = NULL;
	Masina m;
	while (!feof(fisier))
	{
		m=citireMasinaDinFisier(fisier);
		adaugaMasinaInLista(&lista, m);
	}
	fclose(fisier);
	return lista;
}

void dezalocareLDMasini(ListaDubla* listaMasini) {
	Nod* aux;
	if (listaMasini->first)
	{
		if (listaMasini->first->next)
		{
			aux = listaMasini->first->next;
			while (aux)
			{
				free((aux->prev->info).model);
				free((aux->prev->info).numeSofer);
				free(aux->prev);
				aux = aux->next;

			}
		}
		free(listaMasini->last->info.model);
		free(listaMasini->last->info.numeSofer);
		free(listaMasini->first);
		listaMasini->first = NULL;
		listaMasini->last = NULL;

	}

}

float calculeazaPretMediu(ListaDubla listaMasini) {
	Nod* masina = listaMasini.first;
	float suma = 0;
	int nr = 0;
	while (masina)
	{
		nr++;
		suma += masina->info.pret;
		masina = masina->next;
	}
	if (nr != 0)
		return suma / nr;
	return 0;

}

void stergeMasinaDupaID(/*lista masini*/ int id) {
	//sterge masina cu id-ul primit.
	//tratati situatia ca masina se afla si pe prima pozitie, si pe ultima pozitie
}

char* getNumeSoferMasinaScumpa(/*lista dublu inlantuita*/) {
	//cauta masina cea mai scumpa si 
	//returneaza numele soferului acestei maasini.
	return NULL;
}

int main() {

	ListaDubla lista;
	lista= citireLDMasiniDinFisier("masini.txt");
	printf("de la inceput:\n\n");
	afisareListaMasini(lista);
	printf("\n\nde la final:\n\n");
	afisareListaMasiniFinal(lista);
	printf("\n\npret mediu: %.2f\n\n",calculeazaPretMediu(lista));
}