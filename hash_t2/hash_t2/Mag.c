#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <malloc.h>
#include <string.h>

typedef struct {
	char* nume;
	int pret;
	int q;
}Magazin;


typedef struct nodls{
	Magazin info;
	struct nodls* next;
}nodls;

typedef struct hashT {
	struct nodls** vect;
	int nrElem;
}hashT;


void insertLS(nodls** cap, Magazin m) {
	nodls* nou = (nodls*)malloc(sizeof(nodls));

	nou->info.nume = (char*)malloc(sizeof(char) * strlen(m.nume) + 1);
	strcpy(nou->info.nume, m.nume);

	nou->info.pret = m.pret;
	nou->info.q = m.q;

	nou->next = NULL;

	if (*cap == NULL) {
		*cap = nou;
	}
	else {
		nodls* temp = *cap;
		while (temp->next != NULL) {
			temp = temp->next;
		}
		temp->next = nou;
	}
}

void traversLS(nodls* cap) {
	nodls* temp = cap;
	while (temp != NULL) {
		printf(" %s %d %d | \n", temp->info.nume, temp->info.pret, temp->info.q);
		temp = temp->next;
	}
}

void dezalocLS(nodls* cap) {
	nodls* temp = cap;
	while (temp != NULL) {
		nodls* temp2 = temp->next;
		free(temp->info.nume);
		free(temp);
		temp = temp2;
	}
}

int functieHash(char cheie[20], hashT tabela) {
	return cheie[0] % tabela.nrElem;
}

void insertHash(hashT tabela, Magazin m) {
	int poz = functieHash(m.nume, tabela);
	insertLS(&tabela.vect[poz], m);
}

void traversHash(hashT tabela) {
	for (int i = 0; i < tabela.nrElem; i++) {
		if (tabela.vect[i] != NULL) {
			printf("Pozitia: %d -> ", i);
			traversLS(tabela.vect[i]);
		}
	}
}

void dezalocHash(hashT tabela) {
	for (int i = 0; i < tabela.nrElem; i++) {
		if (tabela.vect[i] != NULL) {
			dezalocLS(tabela.vect[i]);
		}
	}
	free(tabela.vect);

}

int main() {
	int nr;
	Magazin m;
	char buff[30];
	hashT tabela;
	tabela.nrElem = 15;
	tabela.vect = (nodls**)malloc(sizeof(nodls*) * tabela.nrElem);
	for (int i = 0; i < tabela.nrElem; i++) {
		tabela.vect[i] = NULL;
	}
	FILE* f = fopen("magazin.txt", "r");
	fscanf(f, "%d", &nr);
	for (int i = 0; i < nr; i++) {
		fscanf(f, "%s", buff);
		m.nume = (char*)malloc(sizeof(char) * strlen(buff) + 1);
		strcpy(m.nume, buff);

		fscanf(f, "%d", &m.pret);
		fscanf(f, "%d", &m.q);

		insertHash(tabela, m);
	}
	fclose(f);

	traversHash(tabela);
	dezalocHash(tabela);
}