//LISTA SIMPLU INLANTUITA este o structura liniara si omogena
//acces secvential,accesezi tot pana la final(nu te intorci)
//1 retine adresa lui 2,2 al lui 3,3-4,etc.
//la ultimul elem adresa pe care o tine e null
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

struct Nod
{
	Masina info;
	struct Nod* next;

};
typedef struct Nod Nod;

Masina citireMasinaDinFisier(FILE* file) {
	char buffer[100];
	char sep[3] = ",\n";
	fgets(buffer, 100, file);
	char* aux;
	Masina m1;
	aux = strtok(buffer, sep);
	m1.id = atoi(aux);
	m1.nrUsi = atoi(strtok(NULL, sep));
	m1.pret= atof(strtok(NULL, sep));
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

void afisareListaMasini(Nod* cap) 
{
	while (cap)//pana la ultimul-null
	{
		afisareMasina(cap->info);
		cap = cap->next;//urmatorul element
	}
}

void adaugaMasinaInLista(Nod** cap ,Masina masinaNoua)//in cazul in care primesti null,trb sa modifici
{
	Nod* temp=malloc(sizeof (Nod)); //aloc spatiu ca sa ne dea adresa
	temp->info = masinaNoua;//shallow copy ptc masina noua nu o sa mai fie folosita
	temp->next = NULL;
	//de fiecare data trb sa verif daca ai lista
	if (!(*cap))
	{
		//niciodata o funct de procesare nu trb sa afiseze nimic in consola
		*cap = temp;//temp e pe stiva si nu ma int adresa de pe o stiva care dispare

	}
	else
	{
		Nod* p = *cap;
		while (p->next)//trebuie sa il am si pe penultimul,daca as verifica doar p nu l mai am pe cel anterior
		{
			p = p->next;
			
		}
		p->next = temp;
	}
}

void adaugaLaInceputInLista(Nod** cap, Masina masinaNoua)
{
	Nod* temp = (Nod*)malloc(sizeof(Nod));
	temp->info = masinaNoua;
	temp->next = *cap;
	*cap = temp;
}

void* citireListaMasiniDinFisier(const char* numeFisier) 
{
	FILE* file = fopen(numeFisier, "r");
	Nod* cap = NULL;//lista
	while (!feof(file))
	{
		adaugaMasinaInLista(&cap, citireMasinaDinFisier(file));
	}
	fclose(file);
	//functia nu e void ,e pointer la void ptc trb sa returnam cap,altfel pierdem adresa primului si nu mai avem acces
	//functia returneaza o adresa la ceva necunoscut==pointer generic
	//se poate lasa asa sau  poate fi pointer la nod
	return cap;
}

void dezalocareListaMasini(Nod** temp) 
{
	while (*temp)
	{
		free((*temp)->info.numeSofer);
		free((*temp)->info.model);
		Nod* nou = *temp;
		nou = (*temp)->next;
		free(*temp);
		*temp = nou;

	}
}

float calculeazaPretMediu(Nod* cap)
{
	float suma = 0;
	int nr = 0;

	while (cap)
	{
		suma += cap->info.pret;
		nr++;
		cap = cap->next;
	}

	if (nr > 0)
		return suma / nr;
	else
		return 0;
}

void stergeMasiniDinSeria(Nod** cap, char serieCautata)
{
	while (*cap && (*cap)->info.serie == serieCautata)
	{
		Nod* temp = *cap;
		*cap = (*cap)->next;

		free(temp->info.model);
		free(temp->info.numeSofer);
		free(temp);
	}

	Nod* p = *cap;

	while (p && p->next)
	{
		if (p->next->info.serie == serieCautata)
		{
			Nod* temp = p->next;
			p->next = temp->next;

			free(temp->info.model);
			free(temp->info.numeSofer);
			free(temp);
		}
		else
		{
			p = p->next;
		}
	}
}

float calculeazaPretulMasinilorUnuiSofer(Nod* cap, const char* numeSofer)
{
	float suma = 0;

	while (cap)
	{
		if (strcmp(cap->info.numeSofer, numeSofer) == 0)
		{
			suma += cap->info.pret;
		}

		cap = cap->next;
	}

	return suma;
}

int main() 
{
	//adresa primului nod-pointer la nod
	//Nod* cap = NULL;initial,neavand elem in lista,va retine null 
	
	
	Nod* cap = citireListaMasiniDinFisier("masini.txt");
	afisareListaMasini(cap);
	//dezalocareListaMasini(&cap);
	//return 0;

	printf("\n\nPret mediu: %.2f\n", calculeazaPretMediu(cap));

	printf("\nPret masini Gigel: %.2f\n",
		calculeazaPretulMasinilorUnuiSofer(cap, "Gigel"));

	stergeMasiniDinSeria(&cap, 'A');

	printf("\nLista dupa stergere:\n");
	afisareListaMasini(cap);
}