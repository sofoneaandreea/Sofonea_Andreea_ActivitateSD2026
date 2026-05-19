#define _CRT_SECURE_NO_WARNINGS

#include<stdio.h>
#include<string.h>
#include<stdlib.h>

struct Pilot
{
	int id;
	float greutate;
	char* nume;
	int varsta;
};

typedef struct Pilot Pilot;
typedef struct NodArbore NodArbore;
struct NodArbore
{
	Pilot info;
	NodArbore* left;
	NodArbore* right;
};

Pilot citirePilot(FILE* f)
{
	Pilot p;
	char sep[3] = ",\n";
	char linie[100];
	fgets(linie, 100, f);
	p.id = atoi(strtok(linie, sep));
	p.greutate = atof(strtok(NULL, sep));
	char* aux = strtok(NULL, sep);
	p.nume = malloc((strlen(aux) + 1) * sizeof(char));
	strcpy(p.nume, aux);
	p.varsta = atoi(strtok(NULL, sep));
	return p;
}

void AdaugaNod(NodArbore** root, Pilot pilotNou)
{
	if(*root)
	{
		if (pilotNou.id < (*root)->info.id)
		{
			AdaugaNod(&(*root)->left, pilotNou);
		}

		if (pilotNou.id > (*root)->info.id)
		{
			AdaugaNod(&(*root)->right, pilotNou);
		}
	}
	else
	{
		NodArbore* nou = malloc(sizeof(NodArbore));
		nou->info = pilotNou;
		nou->left = NULL;
		nou->right = NULL;
		(*root) = nou;
	}
}

NodArbore* citireArbore(const char* numeFisier)
{
	FILE* f = fopen(numeFisier, "r");
	NodArbore* root = NULL;
	Pilot p;
	if (f)
	{
		while (!feof(f))
		{
			p = citirePilot(f);
			AdaugaNod(&root, p);
		}
		fclose(f);
		return root;
	}
}

void AfisarePilot(Pilot p)
{
	printf("Id:%d\n", p.id);
	printf("Greutate:%.2f\n", p.greutate);
	printf("Nume:%s\n", p.nume);
	printf("Varsta:%d\n", p.varsta);
}

void AfisareArbore(NodArbore* root)
{
	//inordine
	if (root)
	{
		
		AfisareArbore(root->left);
		AfisarePilot(root->info);
		AfisareArbore(root->right);
		
	}
}
int NrPiloti(NodArbore* root)
{
	if (root)
		return (1 + NrPiloti(root->left) + NrPiloti(root->right));
	else
		return 0;
}

Pilot cautaPilot(NodArbore* root, int id)
{
	Pilot p;
	p.id = -1;
	if (root)
	{
		if (id < root->info.id)
			return cautaPilot(root->left, id);
		if (id > root->info.id)
			return cautaPilot(root->right, id);
		if (id == root->info.id)
		{
			p = root->info;
			p.nume = malloc((strlen(root->info.nume) + 1) * sizeof(char));
			strcpy(p.nume, root->info.nume);
		}
	}
	return p;
}

void dezalocareArbore(NodArbore** root)
{
	if (*root)
	{
		
		dezalocareArbore(&(*root)->left);
		dezalocareArbore(&(*root)->right);
		free((*root)->info.nume);
		(*root) = NULL;
	}
}
void dezalocarePilot(Pilot* p)
{
	free(p->nume);
	p->greutate = 0;
	p->id = 0;
	p->varsta = 0;
	p->nume = NULL;

}
int main()
{
	NodArbore* root = citireArbore("Piloti.txt");
	AfisareArbore(root);
	printf("\nNr piloti: %d\n\n", NrPiloti(root));
	Pilot p = cautaPilot(root, 3);
	AfisarePilot(p);
	dezalocareArbore(&root);
	AfisareArbore(root);
	dezalocarePilot(&p);
	AfisarePilot(p);
}