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

void afisareMasina(Masina masina) {

	printf("ID:%d || Numar usi:%d || Pret:%5.2f || Model:%-10s || Nume Sofer:%-10s || Serie:%c \n\n", masina.id, masina.nrUsi, masina.pret, masina.model, masina.numeSofer, masina.serie);

}

void afisareVectorMasini(Masina* masini, int nrMasini) {
	if (masini == NULL || nrMasini == 0) {
		printf("vectorul de masini este gol\n");
		return;
	}
	for (int i = 0; i < nrMasini; i++)
		afisareMasina(masini[i]);
}

void adaugaMasinaInVector(Masina** masini, int* nrMasini, Masina masinaNoua) {
	Masina* temp = (Masina*)malloc(((*nrMasini) + 1) * sizeof(Masina));
	for (int i = 0; i < *nrMasini; i++)
		temp[i] = (*masini)[i];
	temp[*nrMasini] = masinaNoua;
	free(*masini);
	*masini = temp;
	(*nrMasini)++;
}

Masina citireMasinaFisier(FILE* file) {


	char linie[256];
	fgets(linie, 255, file);
	char delimitator[3] = ",\n";
	Masina masina;
	masina.id = atoi(strtok(linie, delimitator)); 
	masina.nrUsi = atoi(strtok(NULL, delimitator));
	masina.pret = atof(strtok(NULL, delimitator)); 
	char* aux = strtok(NULL, delimitator);
	masina.model = (char*)malloc((strlen(aux) + 1) * sizeof(char));
	strcpy(masina.model, aux);
	aux = strtok(NULL, delimitator);
	masina.numeSofer = (char*)malloc((strlen(aux) + 1) * sizeof(char));
	strcpy(masina.numeSofer, aux);
	masina.serie = strtok(NULL, delimitator)[0];
	return masina;
}



Masina* citireVectorMasiniFisier(const char* numeFisier, int* nrMasiniCitite) {
	FILE* file = fopen(numeFisier, "r");
	if (!file) {
		printf("eroare la deschidere\n");
		return;
	}
	else {
		Masina* vectorMasini = NULL;
		while (!feof(file)) {
			Masina masina = citireMasinaFisier(file);
			adaugaMasinaInVector(&vectorMasini, nrMasiniCitite, masina);
			
		}
		return vectorMasini;
	}
}

void dezalocareVectorMasini(Masina** vector, int* nrMasini) {
	//este dezalocat intreg vectorul de masini
}

int main() {

	int nrMasini = 0;
	Masina* masini = citireVectorMasiniFisier("masini.txt", &nrMasini);
	afisareVectorMasini(masini, nrMasini);



	return 0;
}