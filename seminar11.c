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


struct NodArbore {
	Masina masina;
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


int calculeazaInaltimeArbore(NodArbore* root) {
	if (root) {
		return max(calculeazaInaltimeArbore(root->left), calculeazaInaltimeArbore(root->right)) + 1;
	}
	else {
		return 0;
	}
}


int calculGE(NodArbore* root) {
	if (root) {
		return calculeazaInaltimeArbore(root->left) - calculeazaInaltimeArbore(root->right);
	}
	else {
		return 0;
	}
}


void rotireStanga(NodArbore** root) {
	NodArbore* temp = (*root)->right;
	(*root)->right = temp->left;
	temp->left = *root;
	*root = temp;
}


void rotireDreapta(NodArbore** root) {
	NodArbore* temp = (*root)->left;
	(*root)->left = temp->right;
	temp->right = *root;
	*root = temp;
}


void adaugaMasinaInArboreEchilibrat(NodArbore** root, Masina masinaNoua) {
	if (*root) {
		if ((*root)->masina.id > masinaNoua.id) {
			adaugaMasinaInArboreEchilibrat(&(*root)->left, masinaNoua);
		}
		else {
			adaugaMasinaInArboreEchilibrat(&(*root)->right, masinaNoua);
		}

		int grad = calculGE(*root);

		if (grad == 2) {
			if (calculGE((*root)->left) == -1) {
				rotireStanga(&(*root)->left);
			}

			rotireDreapta(root);
		}
		if (grad == -2) {
			if (calculGE((*root)->right) == 1) {
				rotireDreapta(&(*root)->right);
			}

			rotireStanga(root);
		}

	}
	else {
		NodArbore* nou = (NodArbore*)malloc(sizeof(NodArbore));
		nou->masina = masinaNoua;
		nou->left = NULL;
		nou->right = NULL;
		(*root) = nou;
	}
}


NodArbore* citireArboreDeMasiniDinFisier(const char* numeFisier) {
	FILE* file = fopen(numeFisier, "r");
	NodArbore* root = NULL;
	while (!feof(file)) {
		Masina masina = citireMasinaDinFisier(file);
		adaugaMasinaInArboreEchilibrat(&root, masina);
	}
	return root;
}


void afisareMasiniDinArbore(NodArbore* root) {
	if (root) {
		afisareMasina(root->masina);
		afisareMasiniDinArbore(root->left);
		afisareMasiniDinArbore(root->right);
	}
	else {
		return;
	}
}


void dezalocareArboreDeMasini(NodArbore** root) {
	if (*root) {
		dezalocareArboreDeMasini(&(*root)->left);
		dezalocareArboreDeMasini(&(*root)->right);
		free((*root)->masina.numeSofer);
		free((*root)->masina.model);
		free((*root));
		*root = NULL;

	}
}


Masina getMasinaByID(NodArbore* root, int id) {
	Masina m;
	m.id = -1;
	if (root == NULL) {
		return m;
	}
	else if (root->masina.id > id) {
		return getMasinaByID(root->left, id);
	}
	else if (root->masina.id < id) {
		return getMasinaByID(root->right, id);
	}
	else {
		m = root->masina;
		m.model = (char*)malloc(strlen(root->masina.model) + 1);
		strcpy(m.model, root->masina.model);
		m.numeSofer = (char*)malloc(strlen(root->masina.numeSofer) + 1);
		strcpy(m.numeSofer, root->masina.numeSofer);
		return m;
	}
}


int determinaNumarNoduri(NodArbore* root) {
	if (root != NULL) {
		return 1 + determinaNumarNoduri(root->left) + determinaNumarNoduri(root->right);
	}
	else {
		return 0;
	}
}


float calculeazaPretTotal(NodArbore* root) {
	if (root) {
		return root->masina.pret + calculeazaPretTotal(root->right) + calculeazaPretTotal(root->left);
	}
	else {
		return 0;
	}

}


float calculeazaPretulMasinilorUnuiSofer(NodArbore* root, const char* numeSofer) {
	float sum = 0;
	if (root) {

		sum += calculeazaPretulMasinilorUnuiSofer(root->left, numeSofer);
		sum += calculeazaPretulMasinilorUnuiSofer(root->right, numeSofer);
		if (strcmp(root->masina.numeSofer, numeSofer) == 0) {
			sum += root->masina.pret;
		}
	}
	return sum;
}

int main() {
	NodArbore* root = citireArboreDeMasiniDinFisier("masini.txt");

	printf("Afisare arbore cu masini:\n");
	afisareMasiniDinArbore(root);
	printf("----------------------------\n");

	float calculPret = calculeazaPretulMasinilorUnuiSofer(root, "Ionescu");
	printf("Masinile lui %s valoreaza %2.f um\n", "Ionescu", calculPret);
	printf("----------------------------\n");

	printf("Masina cu id-ul %d:\n", 5);
	afisareMasina(getMasinaByID(root, 5));
	printf("----------------------------\n");

	printf("Arborele are %d noduri.\n", determinaNumarNoduri(root));
	printf("----------------------------\n");

	printf("Masinile din arbore valoreaza %2.f um.\n", calculeazaPretTotal(root));
	printf("----------------------------\n");

	dezalocareArboreDeMasini(&root);
	printf("Test dezalocare:\n");
	afisareMasiniDinArbore(root);
	printf("----------------------------\n");
	return 0;
}