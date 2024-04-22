#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct Serviciu {
	unsigned int id_serv;
	char* tip_serv;
	char* nume_client;
	float suma_p;
	unsigned int timp_est;
}Serviciu;

typedef struct nodls {
	Serviciu info;
	struct nodls* next;
}nodls;

typedef struct hashT {
	nodls** vect;
	int nrElem;
}hashT;

void inserareLS(nodls** cap, Serviciu* s) {
	nodls* nou = (nodls*)malloc(sizeof(nodls));

	nou->info.id_serv = s->id_serv;

	nou->info.tip_serv = (char*)malloc(sizeof(char) * strlen(s->tip_serv) + 1);
	strcpy(nou->info.tip_serv, s->tip_serv);
	nou->info.nume_client = (char*)malloc(sizeof(char) * strlen(s->nume_client) + 1);
	strcpy(nou->info.nume_client, s->nume_client);

	nou->info.suma_p = s->suma_p;
	nou->info.timp_est = s->timp_est;

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
		printf("%u %s %s %0.2f %u\n", temp->info.id_serv, temp->info.tip_serv, temp->info.nume_client, temp->info.suma_p, temp->info.timp_est);
		temp = temp->next;
	}
}

void dezalocLS(nodls* cap) {
	nodls* temp = cap;
	while (temp != NULL) {
		nodls* temp2 = temp->next;
		free(temp->info.tip_serv);
		free(temp->info.nume_client);
		free(temp);
		temp = temp2;
	}
}
//cheia e nume client
int functieHash(char cheie[35], hashT tabela) {
	return cheie[0] % tabela.nrElem;
}
//ca sa accesez am nevoie de param servicii
void insertHash(hashT tabela, Serviciu* s) {
	int poz = functieHash(s->nume_client, tabela);
	inserareLS(&tabela.vect[poz], s);
}

void traversHash(hashT tabela) {
	for (int i = 0; i < tabela.nrElem; i++) {
		if (tabela.vect[i] != NULL) {
			printf("Pozitia: %d\n", i);
			traversLS(tabela.vect[i]);
		}
	}
}

void dezalocHash(hashT tabela) {
	for (int i = 0; i < tabela.nrElem; i++) {
		if (tabela.vect[i] != NULL) {
			free(tabela.vect[i]);
		}
	}
	free(tabela.vect);
}

void det_nr_serv(hashT tabela, char* nume) {
	int poz = functieHash(nume, tabela);
	nodls* lista = tabela.vect[poz];
	int nrServ = 0;
	while (lista != NULL) {
		if (strcmp(lista->info.nume_client, nume) == 0) {
			nrServ++;
		}
		lista = lista->next;
	}
	printf("\nTOTAL: %d", nrServ);
}


void sterge_ultim(hashT tabela, char* serv) {
	int poz = functieHash(serv, tabela);
	nodls* lista = tabela.vect[poz];
	nodls* ultim = NULL;
	nodls* penultim = NULL;

	while (lista != NULL) {
		if (strcmp(lista->info.tip_serv, serv) == 0) {
			ultim = lista;
			penultim = NULL; 
		}
		else {
			penultim = lista;
		}
		lista = lista->next;
	}

	if (ultim != NULL) {
		if (penultim == NULL) {
			tabela.vect[poz] = ultim->next;
		}
		else {
			penultim->next = NULL;
		}
		free(ultim->info.tip_serv);
		free(ultim->info.nume_client);
		free(ultim);
	}
}




int main() {
	int nr;
	hashT tabela;
	char buff[50];
	char sep[] = ",\n";
	tabela.nrElem = 30;
	tabela.vect = (nodls**)malloc(sizeof(nodls*) * tabela.nrElem);
	for (int i = 0; i < tabela.nrElem; i++) {
		tabela.vect[i] = NULL;
	}
	FILE* f = fopen("serv.txt", "r");
	while(fgets(buff, sizeof(buff), f)) {
		Serviciu* s = (Serviciu*)malloc(sizeof(Serviciu));
		char* token;
		token = strtok(buff, sep);
		s->id_serv = atoi(token);

		token = strtok(NULL, sep);
		s->tip_serv = (char*)malloc(strlen(token) + 1);
		strcpy(s->tip_serv, token);
		token = strtok(NULL, sep);
		s->nume_client = (char*)malloc(strlen(token) + 1);
		strcpy(s->nume_client, token);
		
		token = strtok(NULL, sep);
		s->suma_p = (float)atof(token);

		token = strtok(NULL, sep);
		s->timp_est = atoi(token);

		insertHash(tabela, s);
	}
	fclose(f);
	sterge_ultim(tabela, "");
	traversHash(tabela);
	det_nr_serv(tabela, "Dorel");
	dezalocHash(tabela);
}