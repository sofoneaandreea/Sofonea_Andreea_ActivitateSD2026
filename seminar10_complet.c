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

//creare structura pentru un nod dintr-un arbore binar de cautare

struct NodArbore
{
	Masina info;
	struct NodArbore* left;
	struct NodArbore* right;
};
typedef struct NodArbore NodArbore;
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


void adaugaMasinaInArbore(NodArbore** root, Masina masinaNoua) 
{
	//mergi recursiv deci nu trb sa faci copie
	//decizi o cheie,de exemplu id-in functie de id,il pune in stanga sau in dreapta,practic id ul e numarul nodului
	if (*root)
	{
		if ((*root)->info.id > masinaNoua.id)
		{
			adaugaMasinaInArbore(&(*root)->left, masinaNoua);
		}
		else
		{
			adaugaMasinaInArbore(&(*root)->right, masinaNoua);
		}
	}
	else
	{
		NodArbore* nou = malloc(sizeof(NodArbore));
		nou->info = masinaNoua;
		nou->left = NULL;
		nou->right = NULL;
		(*root) = nou;
	}
}

NodArbore* citireArboreDeMasiniDinFisier(const char* numeFisier) 
{
	FILE* f = fopen(numeFisier, "r");
	NodArbore* root = NULL;
	if (f)
	{
		while (!feof(f))
		{
			Masina m=citireMasinaDinFisier(f);
			adaugaMasinaInArbore(&root, m);
		}
	}
	fclose(f);
	return root;
}

void afisareMasiniDinArbore(NodArbore* root) 
{
	//parcurgerea arborelui poate fi realizata in TREI moduri: RSD(preordine),SRD(inordine),SDR(postordine)
	
	//preordine:
	if (root != NULL)
	{
		afisareMasina(root->info);
		afisareMasiniDinArbore(root->left);
		afisareMasiniDinArbore(root->right);
	}

}

void afisareInordine(NodArbore* root)
{
	if (root != NULL)
	{
		afisareInordine(root->left);
		afisareMasina(root->info);
		afisareInordine(root->right);
	}
	//AFISAREA INORDINE LE ORDONEAZA CRESCATOR
}

void dezalocareArboreDeMasini(NodArbore** root) 
{
	if ((*root) != NULL)
	{
		dezalocareArboreDeMasini(&(*root)->left);
		dezalocareArboreDeMasini(&(*root)->right);
		free((*root)->info.numeSofer);
		free((*root)->info.model);
		free(*root);
		*root = NULL;
	}
}

Masina getMasinaByID(NodArbore* root,int id) 
{
	//e functia de cautare(binara)
	if (root == NULL)
	{
		Masina m;
		m.id = -1;
		return m;
	}
	else if (id < root->info.id)
	{
		return getMasinaByID(root->left, id);
	}
	else if(id>root->info.id)
	{
		return getMasinaByID(root->right, id);
	}
	else
	{
		Masina m = root->info;
		m.numeSofer = malloc((strlen(root->info.numeSofer) + 1) * sizeof(char));
		strcpy(m.numeSofer, root->info.numeSofer);
		m.model = malloc((strlen(root->info.model) + 1) * sizeof(char));
		strcpy(m.model, root->info.model);
		return m;
	}
}

int determinaNumarNoduri(NodArbore* root) 
{
	
	if (root != NULL)
	{
		return determinaNumarNoduri(root->left) + determinaNumarNoduri(root->right) + 1;
	}
	else
	{
		return 0;
	}
}

int calculeazaInaltimeArbore(NodArbore* root) 
{
	if (root)
	{
		return 1 + max(calculeazaInaltimeArbore(root->left), calculeazaInaltimeArbore(root->right));
	}
	else 
	{
		return 0;
	}
}

float calculeazaPretTotal(NodArbore* root) 
{
	if (root)
	{
		return root->info.pret + calculeazaPretTotal(root->left) + calculeazaPretTotal(root->right);
	}
	return 0;
}

float calculeazaPretulMasinilorUnuiSofer(NodArbore* root, const char* numeSofer) 
{
	if (root)
	{
		if (strcmp(root->info.numeSofer, numeSofer) == 0)
		{
			return root->info.pret + calculeazaPretulMasinilorUnuiSofer(root->left, numeSofer) + calculeazaPretulMasinilorUnuiSofer(root->right, numeSofer);
		}
		else
		{
			return calculeazaPretulMasinilorUnuiSofer(root->left, numeSofer) + calculeazaPretulMasinilorUnuiSofer(root->right, numeSofer);
		}
	}
	else
	{
		return 0;
	}
}

int main() 
{
	NodArbore* root = citireArboreDeMasiniDinFisier("masini_arbore.txt");
	afisareInordine(root);
	afisareMasina(getMasinaByID(root, 3));
	printf("nr noduri: %d \n", determinaNumarNoduri(root));
	printf("inaltime: %d\n", calculeazaInaltimeArbore(root));
	printf("pret total: %.2f\n", calculeazaPretTotal(root));
	printf("pret total Ionescu: %.2f", calculeazaPretulMasinilorUnuiSofer(root, "Ionescu"));
	return 0;
}