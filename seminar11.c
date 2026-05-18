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

//1. 
// structuri necesare
//dorim stocarea unui graf intr-o lista de liste
//astfel avem nod ListaPrincipala si NodListaSecundara

typedef struct NodP NodP;
typedef struct NodS NodS;

struct NodP {
	Masina info;
	NodP* next;
	NodS* vecini;
};

struct NodS {
	NodS* next;
	NodP* info;
};

//2.
//functii de inserare in liste
//si in principala si in secundara

void inserareListaP(NodP** graf, Masina masina) {
	NodP* temp = malloc(sizeof(NodP));
	temp->info = masina;
	temp->next = NULL;
	temp->vecini = NULL;
	if (*graf) {
		NodP* aux = (*graf);
		while (aux->next) {
			aux = aux->next;
		}
		aux->next = temp;
	}
	else {
		(*graf) = temp;
	}
}

void inserareListaS(NodS** cap, NodP* vecin) {
	NodS* temp = malloc(sizeof(NodS));
	temp->info = vecin;
	temp->next = NULL;
	if (*cap) {
		NodS* aux = (*cap);
		while (aux->next) {
			aux = aux->next;
		}
		aux->next = temp;
	}
	else {
		(*cap) = temp;
	}
}

//3.
//functie de cautarea in lista principala dupa ID
NodP* cautaNodDupaID(NodP* listaPrincipala, int id) {
	while (listaPrincipala) {
		if (listaPrincipala->info.id == id) {
			return listaPrincipala;
		}
		listaPrincipala = listaPrincipala->next;
	}
	return NULL;
}

//4.
//inserare muchie
void inserareMuchie(NodP* listaPrincipala, int idStart, int idStop) {
	NodP* start = cautaNodDupaID(listaPrincipala, idStart);
	NodP* stop = cautaNodDupaID(listaPrincipala, idStop);
	if (start && stop) {
		inserareListaS(&start->vecini, stop);
		inserareListaS(&stop->vecini, start);
		// graf neorientat, start-stop & stop-start
	}
}

NodP* citireNoduriMasiniDinFisier(const char* numeFisier) {
	FILE* f = fopen(numeFisier, "r");
	NodP* graf = NULL;
	while (!feof(f)) {
		inserareListaP(&graf, citireMasinaDinFisier(f));
	}
	fclose(f);
	return graf;
}

void citireMuchiiDinFisier(const char* numeFisier, NodP* graf) {
	FILE* f = fopen(numeFisier, "r");
	NodS* muchii = NULL;
	while (!feof(f)) {
		int idStart = 0;
		int idStop = 0;
		fscanf(f, "%d %d", &idStart, &idStop);
		inserareMuchie(graf, idStart, idStop);
	}
	fclose(f);
}

void afisareListaVecini(int id, NodP* graf) {
	NodP* temp = cautaNodDupaID(graf, id);
	NodS* cap = temp->vecini;
	while (cap) {
		afisareMasina(cap->info->info);
		cap = cap->next;
	}
}

void dezalocareNoduriGraf(void* listaPrincipala) {
	//sunt dezalocate toate masinile din graf 
	//si toate nodurile celor doua liste
}

int main() {
	NodP* graf = citireNoduriMasiniDinFisier("masini.txt");
	citireMuchiiDinFisier("muchii.txt", graf);
	afisareListaVecini(3, graf);
	return 0;
}