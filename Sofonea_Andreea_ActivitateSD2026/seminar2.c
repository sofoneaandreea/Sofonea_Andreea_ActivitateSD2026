#include<stdio.h>
#include<malloc.h>

struct telefon
{
	int id;
	int RAM;
	char* producator;
	float pret;
	char serie;
};

struct telefon initializare(int id, int RAM, char* producator, float pret, char serie)
{
	struct telefon t;
	t.id = id;
	t.RAM = RAM;
	t.pret = pret;
	t.producator = (char*)malloc((strlen(producator) + 1) * sizeof(char));
	strcpy(t.producator, producator);
	t.serie = serie;
	return t;
}

void afisare(struct telefon t)
{
	printf("Id: %d\nRAM: %d\nPret: %5.2f\nProducator: %s\nSerie: %c\n", t.id, t.RAM, t.pret, t.producator, t.serie);
}

void afisareVector(struct telefon* vector, int nrElemente)
{
	for (int i = 0; i < nrElemente; i++)
	{
		afisare(vector[i]);
		printf("\n");
	}
}

struct telefon copiazaTelefon(struct telefon t)
{
	struct telefon tel = initializare(t.id, t.RAM, t.producator, t.pret, t.serie);
	return tel;
}

struct telefon* copiazaPrimeleElemente(struct telefon* vector, int nrElemente, int nrElementeCopiate)
{
	if (nrElementeCopiate > nrElemente)
	{
		nrElementeCopiate = nrElemente;
	}
	struct telefon* vectnou = (struct telefon*)malloc(sizeof(struct telefon) * nrElementeCopiate);

	for (int i = 0; i < nrElementeCopiate; i++)
		vectnou[i] = copiazaTelefon(vector[i]);

	return vectnou;

}

void dezalocare(struct telefon** vector, int* nrElemente)
{
	for (int i = 0; i < *nrElemente; i++)
	{
		free((*vector)[i].producator);
		(*vector)[i].producator = NULL;
	}
	free(*vector);
	*vector = NULL;
	*nrElemente = 0;
}

void copiazaTelefoaneScumpe(struct telefon* vector, char nrElemente, float pretMin, struct telefon** vectorNou, int* dimensiune)
{
	*dimensiune = 0;
	for (int i = 0; i < nrElemente; i++)
	{

		if (vector[i].pret >= pretMin)
		{
			(*dimensiune)++;
		}
	}

	*vectorNou = malloc(*dimensiune * sizeof(struct telefon));

	for (int i = 0, j = 0; i < nrElemente; i++)
	{
		if (vector[i].pret >= pretMin)
		{
			(*vectorNou)[j] = copiazaTelefon(vector[i]);
			j++;
		}
	}
}

int main()
{
	struct telefon t;
	t = initializare(1, 8, "Samsung", 3000.5, 'A');
	afisare(t);
	printf("\n");
	int nrTelefoane = 3;
	struct telefon* telefoane = (struct telefon*)malloc(sizeof(struct telefon) * nrTelefoane);
	telefoane[0] = initializare(1, 8, "Samsung", 3001.5, 'A');
	telefoane[1] = initializare(2, 8, "Samsung", 2000, 'S');
	telefoane[2] = t;

	afisareVector(telefoane, nrTelefoane);

	int nrTelefoaneCopiate = 2;
	struct telefon* telcopiate = copiazaPrimeleElemente(telefoane, nrTelefoane, nrTelefoaneCopiate);
	printf("\n\nTelefoane copiate: \n\n");
	afisareVector(telcopiate, nrTelefoaneCopiate);
	dezalocare(&telcopiate, &nrTelefoaneCopiate);
	printf("\n\nTelefoane copiate dupa dezalocare: \n\n");
	afisareVector(telefoane, nrTelefoane);

	struct telefon* vectorNou;
	int dim;
	copiazaTelefoaneScumpe(telefoane, nrTelefoane, 2500, &vectorNou, &dim);
	printf("\n\ntelefoane scumpe: \n\n");
	afisareVector(vectorNou, dim);
}