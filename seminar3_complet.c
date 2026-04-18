
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

void afisareMasina(Masina masina) 
{
	printf("ID:%d || Numar usi:%d || Pret:%5.2f || Model:%-10s || Nume Sofer:%-10s || Serie:%c \n\n", masina.id, masina.nrUsi, masina.pret, masina.model, masina.numeSofer, masina.serie);

}

void afisareVectorMasini(Masina* masini, int nrMasini) 
{
	if (nrMasini == 0 || masini == NULL)
	{
		printf("vector gol");
		return;
	}
	for (int i = 0; i < nrMasini; i++)
	{
		afisareMasina(masini[i]);
		printf("\n");
	}
}

void adaugaMasinaInVector(Masina** masini, int * nrMasini, Masina masinaNoua) {
	//adauga in vectorul primit o noua masina pe care o primim ca parametru
	//ATENTIE - se modifica numarul de masini din vector;

	Masina* temp = malloc(((*nrMasini) + 1) * sizeof(Masina));
	for (int i = 0; i < (*nrMasini); i++)
		temp[i] = (*masini)[i];
	temp[(*nrMasini)] = masinaNoua;
	free(*masini);
	*masini = temp;
	(*nrMasini)++;
}

Masina citireMasinaFisier(FILE* file) {
	Masina masina;
	char delim[3] = ",\n";
	char linie[256];
	fgets(linie, 255, file);
	
	masina.id = atoi(strtok(linie, delim));
	masina.nrUsi = atoi(strtok(NULL, delim));
	masina.pret = atof(strtok(NULL, delim));
	char* aux = strtok(NULL, delim);
	masina.model = malloc((1 + strlen(aux)) * sizeof(char));
	strcpy(masina.model, aux);
	aux = strtok(NULL, delim);
	masina.numeSofer = malloc((1 + strlen(aux)) * sizeof(char));
	strcpy(masina.numeSofer, aux);
	masina.serie = strtok(NULL, delim)[0];
	return masina;
}

Masina* citireVectorMasiniFisier(const char* numeFisier, int* nrMasiniCitite) {
	

	FILE* f = fopen(numeFisier, "r");
	Masina* masini = NULL;
	Masina m;
	(*nrMasiniCitite) = 0;
	if (!f)
	{
		printf("fisierul nu merge");
		return;
	}

	while (!feof(f))
	{
		m=citireMasinaFisier(f);
		adaugaMasinaInVector(&masini, nrMasiniCitite, m);
	}
	fclose(f);
	return masini;
}

void dezalocareVectorMasini(Masina** vector, int* nrMasini) {
	if (vector != NULL && *vector != NULL)
	{
		for (int i = 0; i < *nrMasini; i++)
		{
			free((*vector)[i].model);
			free((*vector)[i].numeSofer);
		}
		free(*vector);
		*nrMasini = 0;
	}
}

int main() 
{
	Masina* vect;
	int nrMasini=0;
	vect = citireVectorMasiniFisier("masini.txt", &nrMasini);
	afisareVectorMasini(vect, nrMasini);

	return 0;
}
