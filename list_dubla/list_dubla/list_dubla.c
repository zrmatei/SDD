#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <malloc.h>
#include <string.h>

typedef struct {
	char* nume;
	int p, q;
}Magazin;

typedef struct nodls {
	Magazin* info;
	struct nodls* next, *prev;
}nodls;

nodls* insertSF(nodls* cap, nodls** coada, Magazin* m) {
	nodls* nou = (nodls*)malloc(sizeof(nodls));
	nou->info = (Magazin*)malloc(sizeof(Magazin));
	nou->info->nume = (char*)malloc(sizeof(char) * (strlen(m->nume) + 1));
	strcpy(nou->info->nume, m->nume);

	nou->info->p = m->p;
	nou->info->q = m->q;

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
nodls* insertBeginning(nodls* cap, nodls** coada, Magazin* m) {
	nodls* nou = (nodls*)malloc(sizeof(nodls));
	nou->info = (Magazin*)malloc(sizeof(Magazin));
	nou->info->nume = (char*)malloc(sizeof(char) * (strlen(m->nume) + 1));
	strcpy(nou->info->nume, m->nume);
	nou->info->p = m->p;
	nou->info->q = m->q;

	nou->prev = NULL;
	nou->next = NULL;
	if (cap == NULL) {
		cap = nou;
		*coada = nou;
	}
	else {
		nodls* temp = cap;
		while (temp->prev != NULL) {
			temp = temp->prev;
			
		}
		temp->prev = nou;
		nou->next = temp;
		cap = nou;
	}
	return cap;
}

void travers(nodls* cap) {
	nodls* temp = cap;
	while (temp != NULL) {
		printf("%s %d %d\n", temp->info->nume, temp->info->p, temp->info->q);
		temp = temp->next;
	}
}

void traversInvers(nodls* coada) {
	nodls* temp = coada;
	while (temp != NULL) {
		printf("%s %d %d\n", temp->info->nume, temp->info->p, temp->info->q);
		temp = temp->prev;
	}
}

void dezaloc(nodls* cap) {
	nodls* temp = cap;
	while (temp != NULL) {
		nodls* temp2 = temp->next;
		free(temp->info->nume);
		free(temp->info);
		free(temp);

		temp = temp2;

	}
}

void sterg(nodls** cap, nodls** coada, int cantitate) {
	nodls* temp = *cap;
	while (temp != NULL) {
		nodls* temp2 = temp->next;
		if (temp->info->q > cantitate) {
			nodls* anterior = temp->prev;
			nodls* urmator = temp->next;
			if (anterior != NULL) { //verific sa nu fiu pe cap dupa stergere nod ce nu repsecta conditia
				anterior->next = urmator; //legatura intre nodul anterior celui sters si cel urmator
			}
			else {
				*cap = urmator; //inseamna ca am sters capul si trebuie sa mut capul pe urmatorul nod
			}
			if (urmator != NULL) { //verific sa nu fiu pe coada
				urmator->prev = anterior; //legatura intre nodul urmator celui sters si cel anterior
			}
			else {
				*coada = anterior; //am sters coada, deci ma mut cu o pozitie in spate
			}
			free(temp->info->nume);
			free(temp->info);
			free(temp);
		}
		temp = temp2; //trec pe urmatorul nod
	}
}

void stergPrimele(nodls** cap, int primeleX) {
	nodls* temp = *cap;
	while (temp != NULL && primeleX > 0) {
		nodls* urmator = temp->next;
		free(temp->info->nume);
		free(temp->info);
		free(temp);
		if (urmator != NULL) {
			urmator->prev = NULL;
			*cap = urmator;
		}
		else {
			*cap = NULL;
		}
		temp = urmator;
		primeleX--;
	}
}
void stergUltimele(nodls** coada, int ultimeleX) {
	nodls* temp = *coada;
	while (temp != NULL && ultimeleX > 0) {
		nodls* anterior = temp->prev;
		free(temp->info->nume);
		free(temp->info);
		free(temp);

		if (anterior != NULL) {
			anterior->next = NULL;
			*coada = anterior; 
		}
		else {
			*coada = NULL;
		}

		temp = anterior;
		ultimeleX--;
	}
}

//parcurg lista si vad daca am pe noduri o val mai mare decat cea data ca param.
void determin(nodls* cap, int min) {
	int contor = 0;
	nodls* temp = cap;
	while (temp != NULL) {
		if (temp->info->q > min) {
			contor++;
			printf("nr cantitati mai mari: %d", contor);
		}
		temp = temp->next;
	}
	if (contor == 0) {
		printf("nu am valori mai mari decat acea valoare");
	}
}


	int main() {
		Magazin* m;
		nodls* cap = NULL;
		nodls* coada = NULL;
		int nr;
		FILE* f = fopen("magazin.txt", "r");
		fscanf(f, "%d", &nr);
		char buff[50];
		for (int i = 0; i < nr; i++) {
			fscanf(f, "%s", buff);
			m = (Magazin*)malloc(sizeof(Magazin));
			m->nume = (char*)malloc(sizeof(char) * strlen(buff) + 1);
			strcpy(m->nume, buff);

			fscanf(f, "%d", &m->p);
			fscanf(f, "%d", &m->q);

			cap = insertSF(cap, &coada, m);

		}
		fclose(f);

		//sterg(&cap, &coada, 1);
		

		travers(cap);
		determin(cap, 3); 
		printf("\nSTERG PRIMELE X: \n");
		stergPrimele(&cap, 2);
		//stergUltimele(&coada, 2);
		travers(cap);
		dezaloc(cap);
	}
