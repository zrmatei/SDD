//47
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Farmacie {
	char* nume;
	float suprafata;
	unsigned char nr_angajati;
	char* adresa;
}Farmacie;

typedef struct nodls {
	Farmacie* info;
	struct nodls* next, *prev;
}nodls;

nodls* insertSF(nodls* cap, nodls** coada, Farmacie* f) {
	nodls* nou = (nodls*)malloc(sizeof(nodls));
	nou->info = (Farmacie*)malloc(sizeof(Farmacie));

	nou->info->nume = (char*)malloc(sizeof(char) * strlen(f->nume) + 1);
	strcpy(nou->info->nume, f->nume);

	nou->info->suprafata = f->suprafata;
	nou->info->nr_angajati = f->nr_angajati;

	nou->info->adresa = (char*)malloc(sizeof(char) * strlen(f->adresa) + 1);
	strcpy(nou->info->adresa, f->adresa);

	nou->next = NULL;
	nou->prev = NULL;

	if (cap == NULL) {
		cap = nou;
		*coada = nou;
	}
	else {
		nodls* temp = cap;
		while (temp->next != NULL) {
			temp = temp->next;
		}
		temp->next = nou;
		nou->prev = temp;
		*coada = nou;
	}
	return cap;
}

void travers(nodls* cap) {
	nodls* temp = cap;
	while (temp != NULL) {
		printf("%s %0.2f %hhu %s\n", temp->info->nume, temp->info->suprafata, temp->info->nr_angajati, temp->info->adresa);
		temp = temp->next;
	}
}

void traversInvers(nodls* coada) {
	nodls* temp = coada;
	while (temp != NULL) {
		printf("%s, %f %hhu %s\n", temp->info->nume, temp->info->suprafata, temp->info->nr_angajati, temp->info->adresa);
		temp = temp->prev;
	}
}


void dezaloc(nodls* cap) {
	nodls* temp = cap;
	while (temp != NULL) {
		nodls* temp2 = temp->next;
		free(temp->info->nume);
		free(temp->info->adresa);
		free(temp->info);
		free(temp);
		temp = temp2;
	}
}

void determin(nodls* cap, int prag) {
	nodls* temp = cap;
	int contor = 0;
	while (temp != NULL) {
		if (temp->info->suprafata < prag) {
			contor++;
		}
		temp = temp->next;
	}
	printf("TOTAL: %d", contor);
}

void sterg(nodls** cap, nodls** coada, int prag) {
	nodls* temp = *cap;
	while (temp != NULL) {
		nodls* temp2 = temp->next;
		if (temp->info->nr_angajati > prag) {
			nodls* anterior = temp->prev;
			nodls* urm = temp->next;
			if (anterior != NULL) { //verific sa nu fiu pe cap
				anterior->next = urm; //nu am sters capul
			}
			else { //am sters capul, ma mut cu o poz in fata
				*cap = urm;
			}
			if (urm != NULL) { //verific sa nu fiu pe coada
				urm->prev = anterior;
			}
			else {
				*coada = anterior;
			}
			free(temp->info->nume);
			free(temp->info->adresa);
			free(temp->info);
			free(temp);
		}
		temp = temp2;
	}
}

void convertVector(nodls* cap, Farmacie** vect, int* poz, char* n_farmac) {
	nodls* temp = cap;
	int contor = 0;
	while (temp != NULL) {
		if (strcmp(temp->info->nume, n_farmac) == 0) {
			vect[*poz] = (Farmacie*)malloc(sizeof(Farmacie));
			vect[*poz]->nume = (char*)malloc(sizeof(char) * (strlen(temp->info->nume) + 1));
			strcpy(vect[*poz]->nume, temp->info->nume);
			vect[*poz]->adresa = (char*)malloc(sizeof(char) * (strlen(temp->info->adresa) + 1));
			strcpy(vect[*poz]->adresa, temp->info->adresa);
			(*poz)++;
		}
		temp = temp->next;
	}
}


int main() {
	
	nodls* cap = NULL;
	nodls* coada = NULL;
	char sep[] = ",\n";
	char buff[100];
	char* token;

	FILE* file = fopen("farm.txt", "r");
	while (fgets(buff, sizeof(buff), file)) {
		Farmacie* f;
		f = (Farmacie*)malloc(sizeof(Farmacie));
		
		token = strtok(buff, sep);
		f->nume = (char*)malloc(strlen(token) + 1);
		strcpy(f->nume, token);

		token = strtok(NULL, sep);
		f->suprafata = atof(token);

		token = strtok(NULL, sep);
		f->nr_angajati = atoi(token);

		token = strtok(NULL, sep);
		f->adresa = (char*)malloc(strlen(token) + 1);
		strcpy(f->adresa, token);

		cap = insertSF(cap, &coada, f);
	}
	fclose(file);
	travers(cap);
	determin(cap, 50);
	printf("//////////////////////////////////\n");
	//sterg(&cap, &coada, 5);
	travers(cap);

	printf("\n-------conversie-----------");
	int nr = 0;
	Farmacie** vect = (Farmacie**)malloc(10 * sizeof(Farmacie*)); 
	convertVector(cap, vect, &nr, "Dorel");
	for (int i = 0; i < nr; i++)
		printf("\n%s, %s ", vect[i]->nume, vect[i]->adresa);
	for (int i = 0; i < nr; i++) {
		free(vect[i]->nume);
		free(vect[i]->adresa);
		free(vect[i]);
	}
	free(vect);



	dezaloc(cap);
}