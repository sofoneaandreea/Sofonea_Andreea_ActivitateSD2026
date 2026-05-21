#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>
#include<string.h>


struct Produs
{
	int id;
	float pret;
	char* denumire;
};
typedef struct Produs Produs;

typedef struct NodArbore NodArbore;
struct NodArbore
{
	Produs produs;
	NodArbore* left;
	NodArbore* right;
};

void AfisareProdus(Produs p)
{
	printf("id: %d\n", p.id);
	printf("pret: %.2f\n", p.pret);
	printf("denumire: %s\n\n", p.denumire);
}
Produs citireProdus(FILE* f)
{
	char sep[3] = ",\n";
	char linie[100];
	fgets(linie, 100, f);
	Produs p;
	p.id = atoi(strtok(linie, sep));
	p.pret = atof(strtok(NULL, sep));
	char* aux = strtok(NULL ,sep);
	p.denumire = malloc((strlen(aux) + 1) * sizeof(char));
	strcpy(p.denumire, aux);
	return p;
}

int detInaltime(NodArbore* root)
{
	if (root)
		return 1 + max(detInaltime(root->left), detInaltime(root->right));
	else
		return 0;
}

int detGE(NodArbore* root)
{
	if (root)
		return detInaltime(root->left) - detInaltime(root->right);
	else
		return 0;
}

void RotireStanga(NodArbore** root)
{
	NodArbore* temp = (*root)->right;
	(*root)->right = temp->left;
	temp->left = (*root);
	(*root) = temp;
}

void RotireDreapta(NodArbore** root)
{
	NodArbore* temp = (*root)->left;
	(*root)->left = temp->right;
	temp->right = (*root);
	(*root) = temp;
}

void AdaugaProdus(NodArbore** root,Produs p)
{
	if ((*root))
	{
		if (p.id < (*root)->produs.id)
			AdaugaProdus(&(*root)->left, p);
		else
			AdaugaProdus(&(*root)->right, p);

		int ge = detGE((*root));

		if (ge == -2)
		{
			if (detGE((*root)->right) == 1)
				RotireDreapta(&(*root)->right);
			RotireStanga((*root));
		}

		if (ge == 2)
		{
			if (detGE((*root)->left) == -1)
				RotireStanga(&(*root)->left);
			RotireDreapta(&(*root));
		}
	}
	else
	{
		NodArbore* nou = malloc(sizeof(NodArbore));
		nou->produs = p;
		nou->left = NULL;
		nou->right = NULL;
		(*root) = nou;
	}
}

NodArbore* citesteArbore(const char* numeFisier)
{
	FILE* f = fopen(numeFisier, "r");
	NodArbore* root=NULL;
	Produs p;
	if (f)
	{
		while (!feof(f))
		{
			p = citireProdus(f);
			AdaugaProdus(&root, p);
		}
	}
	return root;
}

void AfiseazaArbore(NodArbore* root)
{
	//inordine
	if (root)
	{
		
		AfiseazaArbore(root->left);
		AfisareProdus(root->produs);
		AfiseazaArbore(root->right);
		
	}
}

int nrProduse(NodArbore* root)
{
	if (root)
		return 1 + nrProduse(root->left) + nrProduse(root->right);
	else
		return 0;
}

void dezalocareArbore(NodArbore** root)
{
	if (*root)
	{
		dezalocareArbore(&(*root)->left);
		dezalocareArbore(&(*root)->right);
		free((*root)->produs.denumire);
		free(*root);
		(*root) = NULL;
	}
}

Produs getProdusById(NodArbore* root,int id)
{
	Produs p;
	p.id = -1;
	p.denumire = NULL;
	p.pret = 0;
	if (root)
	{
		
		if (id < root->produs.id)
			return getProdusById(root->left, id);
		else
			if (id > root->produs.id)
				return getProdusById(root->right, id);
			else
		       if (id == root->produs.id)
		         {
			          p = root->produs;
			          p.denumire = malloc((strlen(root->produs.denumire) + 1) * sizeof(char));
			          strcpy(p.denumire, root->produs.denumire);
			          return p;
		        }
	}
	return p;
	
}

void dezalocareProdus(Produs* p)
{
	free(p->denumire);
	p->id = -1;
	p->denumire = NULL;
	p->pret = 0;

}
int main()
{
	NodArbore* root = citesteArbore("Produse.txt");
	AfiseazaArbore(root);
	printf("\nNr produse: %d", nrProduse(root));
	//dezalocareArbore(&root);
	//AfiseazaArbore(root);
	printf("\n\nprodus cautat:\n");
	AfisareProdus(getProdusById(root, 7));
}