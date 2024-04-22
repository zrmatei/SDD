#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <malloc.h>
#include <string.h>

typedef struct {
	char* nume;
	int p, q;
}Magazin;

typedef struct nodls {
	Magazin info;
	struct nodls* next;
}nodls;

nodls* insertSF(nodls* cap, Magazin m) {
	nodls* nou = (nodls*)malloc(sizeof(nodls));
	nou->info.nume = (char*)malloc(sizeof(char) * (strlen(m.nume) + 1));
	strcpy(nou->info.nume, m.nume);
	
	nou->info.p = m.p;
	nou->info.q = m.q;

	nou->next = NULL;

	if (cap == NULL) {
		cap = nou;
	}
	else {
		nodls* temp = cap;
		while (temp->next != NULL) {
			temp = temp->next;
		}
		temp->next = nou;
	}
	return cap;
}

void travers(nodls* cap) {
	nodls* temp = cap;
	while (temp != NULL) {
		printf("%s %d %d\n", temp->info.nume, temp->info.p, temp->info.q);
		temp = temp->next;
	}
}

void dezaloc(nodls* cap) {
	nodls* temp = cap;
	while (temp != NULL) {
		nodls* temp2 = temp->next;
		free(temp->info.nume);
		free(temp);

		temp = temp2;

	}
}

int main() {
	Magazin m;
	nodls* cap = NULL;
	FILE* f = fopen("magazin.txt", "r");
	char buff[50];
	while(fgets(buff, sizeof(buff), f) ) {
		fscanf(f, "%s", buff);
		m.nume = (char*)malloc(sizeof(char) * strlen(buff) + 1);
		strcpy(m.nume, buff);

		fscanf(f, "%d", &m.p);
		fscanf(f, "%d", &m.q);

		cap = insertSF(cap, m);

	}
	fclose(f);

	travers(cap);
	dezaloc(cap);
}