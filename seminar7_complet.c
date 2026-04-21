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

//creare structura pentru un nod dintr-o lista simplu inlantuita
typedef struct Nod Nod;
struct Nod
{
	Masina info;
	Nod* next;
};

//creare structura pentru tabela de dispersie
// aceasta este un vector de liste
typedef struct HashTable HashTable;
struct HashTable 
{
	Nod** vector;//un pointer la o lista de adrese
	int dim;
};
typedef struct HashTable HashTable;

Masina citireMasinaDinFisier(FILE* file) {
	char linie[256];
	char sep[3] = ",\n";
	fgets(linie, 256, file);
	char* aux;
	Masina m;
	m.id = atoi(strtok(linie,sep));
	m.nrUsi = atoi(strtok(NULL, sep));
	m.pret = atof(strtok(NULL, sep));
	aux = strtok(NULL, sep);
	m.model = malloc((strlen(aux) + 1)*sizeof(char));
	strcpy(m.model, aux);
	aux = strtok(NULL, sep);
	m.numeSofer = malloc((strlen(aux) + 1)*sizeof(char));
	strcpy(m.numeSofer, aux);
	m.serie = strtok(NULL, sep)[0];
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

void afisareListaMasini(Nod* lista) 
{
	Nod* aux = lista;
	while (aux)
	{
		afisareMasina(aux->info);
		printf("\n");
		aux = aux->next;
	}
}

void adaugaMasinaInLista(Nod** lista, Masina masinaNoua) 
{
	//adauga la final in lista primita o noua masina pe care o primim ca parametru
	Nod* temp = malloc(sizeof(Nod));
	temp->info = masinaNoua;
	temp->next = NULL;
	Nod* aux = (*lista);
	if ((*lista) == NULL)
		*lista = temp;
	else
	{
		while (aux->next)
		{
			aux = aux->next;
		}
		aux->next = temp;
	}
}


HashTable initializareHashTable(int dimensiune) 
{
	HashTable ht;
	ht.dim = dimensiune;
	// Trebuie să aloci vectorul de pointeri!
	ht.vector = malloc(sizeof(Nod*) * dimensiune);
	for (int i = 0; i < dimensiune; i++)
		ht.vector[i] = NULL;
	
	return ht;
}

int calculeazaHash(int id, int dimensiune) 
{
	return (id * 13) % dimensiune;
}

void inserareMasinaInTabela(HashTable hash, Masina m) 
{
	int hashcode = calculeazaHash(m.id, hash.dim);
	adaugaMasinaInLista(&hash.vector[hashcode], m);
}

HashTable citireMasiniDinFisier(const char* numeFisier) 
{
	FILE* f = fopen(numeFisier, "r");
	HashTable ht = initializareHashTable(5);
	if (f)
	{
		while (!feof(f))
		{
			Masina m = citireMasinaDinFisier(f);
			inserareMasinaInTabela(ht, m);
		}
	}
	fclose(f);
	return ht;
}

void afisareTabelaDeMasini(HashTable ht) 
{
	if (ht.vector[0] == NULL)
		printf("hashTable gol");
	else
	{
		for (int i = 0; i < ht.dim; i++)
		{
			if(ht.vector[i]!=NULL)
			{
				printf("cluster %d: \n", i);
				afisareListaMasini(ht.vector[i]);
				printf("\n");
			}
		}
	}
}

void dezalocareTabelaDeMasini(HashTable *ht) 
{
		for (int i = 0; i < ht->dim; i++)
		{
			
			while (ht->vector[i])
			{
				Nod* aux = ht->vector[i];
				free(aux->info.model);
				free(aux->info.numeSofer);
				ht->vector[i] = ht->vector[i]->next;
				free(aux);
			}
		}
		free(ht->vector);
		ht->vector = NULL;
		ht->dim = 0;
/*După ce ai golit toate listele din interiorul vectorului,
vectorul în sine încă ocupă memorie în Heap (alocată cu malloc în funcția de inițializare).
Deoarece primești HashTable* ht(prin pointer), este bine ca la final să pui ht->vector = NULL
și ht->dim = 0. Astfel, dacă cineva încearcă să mai folosească tabela după dezalocare,
programul nu va crăpa încercând să acceseze memorie ștearsă.*/
}

float* calculeazaPreturiMediiPerClustere(HashTable ht, int* nrClustere) {
	
	*nrClustere = 0;
	for (int i = 0; i < ht.dim; i++)
	{
		if (ht.vector[i]!=NULL)
		{
			(*nrClustere)++;
		}
	}
	if (*nrClustere == 0) return NULL;
	float* preturi = malloc(sizeof(float) * (*nrClustere));
	int j = 0;
	for (int i = 0; i < ht.dim; i++)
	{
		float suma = 0;//CA SA FIE REZ FLOAT!!
		int nr = 0;
		Nod* aux = ht.vector[i];
		while (aux)
		{
			suma = suma + aux->info.pret;
			nr++;
			aux = aux->next;//Când faci ht.vector[i] = ht.vector[i]->next;, tu modifici direct listele din HashTable-ul original.
		}
		if (nr != 0)
		{
			preturi[j] = suma / nr;
			j++;
		}
	}
	return preturi;
}

Masina getMasinaDupaID(HashTable ht ,int id) {
	Masina m;
	m.id = -1;
	int hashcode = calculeazaHash(id, ht.dim);
	Nod* aux = ht.vector[hashcode];
	while (aux)
	{
		if (aux->info.id == id)
		{
			m = aux->info;
			m.model = malloc((strlen(aux->info.model) + 1) * sizeof(char));//aici fac deep copy ca sa nu am doua variab care pointeaza la ac adresa
			strcpy(m.model, aux->info.model);
			m.numeSofer = malloc((strlen(aux->info.numeSofer) + 1) * sizeof(char));
			strcpy(m.numeSofer, aux->info.numeSofer);
			return m;
		}
			aux = aux->next;
	}
	return m;
}

int main() {

	HashTable ht;
	ht = citireMasiniDinFisier("masini.txt");
	afisareTabelaDeMasini(ht);
	int nrClustere;
	float* preturi = calculeazaPreturiMediiPerClustere(ht, &nrClustere);
	for (int i = 0; i < nrClustere; i++)
	{
		printf("\npret mediu cluster %d: %.2f\n", i, preturi[i]);
	}
	return 0;
}