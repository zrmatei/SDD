#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Masina{
    char* nume;
    float pret;
    int viteza;
}Masina;

typedef struct nodls{
    Masina* info;
    struct nodls* next;
}nodls;

typedef struct hash{
    nodls** vect;
    int nrElem;
}hash;

nodls* insertSF(nodls* cap, Masina* m){
    nodls* nou = (nodls*)malloc(sizeof(nodls));
    nou->info = (Masina*)malloc(sizeof(Masina));

    nou->info->nume = (char*)malloc(sizeof(char) * (strlen(m->nume)+1));
    strcpy(nou->info->nume, m->nume);
    nou->info->pret = m->pret;
    nou->info->viteza = m->viteza;

    nou->next = NULL;
    if(cap == NULL){
        cap = nou;
    }else{
        nodls* temp = cap;
        while(temp->next){
            temp = temp->next;
        }
        temp->next = nou;
    }
    return cap;
}

void traversare(nodls* cap){
    nodls* temp = cap;
    while(temp){
        printf("%s %0.2f %d\n", temp->info->nume, temp->info->pret, temp->info->viteza);
        temp = temp->next;
    }
}

void dezaloc(nodls* cap){
    nodls* temp = cap;
    while(temp){
        nodls* temp2 = temp->next;
        free(temp->info->nume);
        free(temp->info);
        free(temp);
        temp = temp2;
    }
}

int functieHash(hash h, char cheie[50]){
    return cheie[0] % h.nrElem;
}

void insertHash(hash h, Masina* m){
    int poz = functieHash(h, m->nume);
    if(h.vect != NULL){
        h.vect[poz] = insertSF(h.vect[poz], m);
    }
}

void traversareHash(hash h){
    for(int i = 0; i < h.nrElem; i++){
        if(h.vect[i] != NULL){
            printf("Pozitia %d\n", i);
            traversare(h.vect[i]);
        }
    }
}

void dezalocHash(hash h){
    for(int i = 0 ; i < h.nrElem; i++){
        if(h.vect[i] != NULL){
            dezaloc(h.vect[i]);
        }
    }
    free(h.vect);
}

Masina* getMasinaByNume(hash h, char nume[50]){
    //calcul pozitie in hash
    int poz = functieHash(h, nume);
    //parcurg lista simpla unde am acel hash
    nodls* temp = h.vect[poz];
    while(temp){
        if(strcmp(temp->info->nume, nume) == 0){
            return temp->info;
        }
        temp = temp->next;
    }
    return NULL;
}

void stergeMasina(hash* h, char nume[50]){
    int poz = functieHash(*h, nume);
    nodls* temp = h->vect[poz];
    nodls* prev = NULL;
    while(temp){
        if(strcmp(temp->info->nume, nume) == 0){
            if(prev == NULL){
                h->vect[poz] = temp->next;
            }else{
                prev->next = temp->next;
            }
            free(temp->info->nume);
            free(temp->info);
            free(temp);
        }
        prev = temp;
        temp = temp->next;
    }
}

int main(){
    char* token;
    char buff[100];
    char sep[3] = "\n,";
    nodls* cap = NULL;

    hash h;
    h.nrElem = 6;
    h.vect = (nodls**)malloc(sizeof(nodls*) * h.nrElem);
    for(int i = 0; i < h.nrElem; i++){
        h.vect[i] = NULL;
    }

    FILE* f = fopen("masini.txt", "r");
    while(fgets(buff, sizeof(buff),f)){
        Masina* m = (Masina*)malloc(sizeof(Masina));
        
        token = strtok(buff, sep);
        m->nume = (char*)malloc(sizeof(char) * (strlen(token)+1));
        strcpy(m->nume, token);
        token = strtok(NULL, sep);
        m->pret = atof(token);
        token = strtok(NULL, sep);
        m->viteza = atoi(token);

        insertHash(h, m);
    }
    traversareHash(h);

    Masina* cautata = getMasinaByNume(h, "Ford");
    printf("\nMasina gasita: %s %0.2f %d\n", cautata->nume, cautata->pret, cautata->viteza);

    stergeMasina(&h, "Ford");
    Masina* cautata2 = getMasinaByNume(h, "Ford");
    printf("\nMasina gasita: %s %0.2f %d\n", cautata->nume, cautata->pret, cautata->viteza);

    dezalocHash(h);
}