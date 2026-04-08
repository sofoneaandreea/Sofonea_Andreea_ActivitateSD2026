//seminar7-hashtable-hash ul e ca o functie matematica
// daca ai acelasi rez->coliziune->metode de rez a coliziunilor(cauti alt loc in tabel pt coliziune,rehashing-alta modalitate de calcul(+linear probing),
// creare liste pt rezultate cu ac hash(chain)-mai multe pe ac index)

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
struct HashTable {
	int dim;
	Nod** vector;//vector de pointeri
};
typedef struct HashTable HashTable;

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

void afisareListaMasini(Nod* lista)
{
	if (lista)
	{
		while (lista->next)
		{
			afisareMasina(lista->info);
			lista = lista->next;
		}
		afisareMasina(lista->info);
	}
}

void adaugaMasinaInLista(Nod** lista, Masina masinaNoua) //la final-dublu pointer ptc modifici
{
	Nod* nou = (Nod*)malloc(sizeof(Nod));
	nou->info = masinaNoua;
	nou->next = NULL;//inserare la final
	//ne luam pointer auxiliar pt parcurgere ca modificarea listei ramane
	Nod* temp = *lista;//daca ii dai doar lista ar tine un pointer care tine adresa listei dar tie iti trb primul element din lista
	if (*lista)
	{
		while (temp->next)
		{
			temp = temp->next;
		}
		temp->next = nou;//ultimul curent tine adresa nodului pe care il adaugam
	}
	else
		*lista = nou;

}


HashTable initializareHashTable(int dimensiune)
{
	HashTable ht;
	//initializeaza vectorul de liste si seteaza fiecare lista ca fiind NULL;
	ht.dim = dimensiune;
	ht.vector = malloc(dimensiune * sizeof(Nod*));
	for (int i = 0; i < dimensiune; i++)
		ht.vector[i] = NULL;
	return ht;
}

int calculeazaHash(int id, int dimensiune) //id-un atribut al masini pentru clusterizare=grupare-fiecare lista e un cluster
{
	//este calculat hash-ul in functie de dimensiunea tabelei si un atribut al masinii
	return (id * 3) % dimensiune; //se inm cu nr prim ca sa fie mai putine coliziuni

}

void inserareMasinaInTabela(HashTable hash, Masina masina) {
	//este folosit mecanismul CHAINING
	//este determinata pozitia si se realizeaza inserarea pe pozitia respectiva
	int hashcode = calculeazaHash(masina.id, hash.dim);
	if (!hash.vector[hashcode])//daca nu exista n ai coliziune
	{
		adaugaMasinaInLista(&hash.vector[hashcode], masina);
	}
	else//avem coliziune
	{
		adaugaMasinaInLista(&hash.vector[hashcode], masina);
	}
	//se putea un singur apel,am scris as ca sa fie explicit
}

HashTable citireMasiniDinFisier(const char* numeFisier)
{
	FILE* f = fopen(numeFisier, "r");
	HashTable hash = initializareHashTable(3);
	if (f)
	{
		while (!feof(f))
		{
			Masina mCitita = citireMasinaDinFisier(f);
			inserareMasinaInTabela(hash, mCitita);
		}
	}
	fclose(f);
	return hash;
}

void afisareTabelaDeMasini(HashTable ht)
{
	for (int i = 0; i < ht.dim; i++)
	{
		printf("cluster %d:\n", i + 1);
		afisareListaMasini(ht.vector[i]);
		printf("\n-------------------------------------------------\n");
	}
}

void dezalocareTabelaDeMasini(HashTable* ht) {
	//sunt dezalocate toate masinile din tabela de dispersie
}

float* calculeazaPreturiMediiPerClustere(HashTable ht, int* nrClustere) {
	//calculeaza pretul mediu al masinilor din fiecare cluster.
	//trebuie sa returnam un vector cu valorile medii per cluster.
	//lungimea vectorului este data de numarul de clustere care contin masini
	return NULL;
}

Masina getMasinaDupaId(HashTable ht, int id) {
	Masina m;
	m.id = -1;
	int hashCode = calculeazaHash(id, ht.dim);
	Nod* cautare = ht.vector[hashCode];
	while (cautare)//parcurgi lista
	{
		if (cautare->info.id == id)
		{
			m = cautare->info;//e shallow copy
			m.numeSofer = (char*)malloc((strlen(cautare->info.numeSofer) + 1) * sizeof(char));
			strcpy(m.numeSofer, cautare->info.numeSofer);
			m.model = (char*)malloc((strlen(cautare->info.model) + 1) * sizeof(char));
			strcpy(m.model, cautare->info.model);
			return m;
		}
		cautare = cautare->next;
	}
	return m;
}

int main() {

	HashTable ht = citireMasiniDinFisier("masini.txt");
	afisareTabelaDeMasini(ht);
	Masina test = getMasinaDupaId(ht, 8);
	afisareMasina(test);

	return 0;
}