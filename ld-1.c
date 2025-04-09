#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Persoana{
    char* nume;
    int varsta;
    float balantaCont;
    unsigned char valoare; //unsigned char ul e practic un int, doar ca il citesti ca %hhu, nu cu %d
}Persoana;

typedef struct nodls{
   struct Persoana* inf;
   struct nodls* next, *prev;
}nodls;

nodls* insertSF(nodls* cap, nodls** coada, Persoana* p){
    nodls* nou = (nodls*)malloc(sizeof(nodls));
    nou->inf = (Persoana*)malloc(sizeof(Persoana));

    nou->inf->nume = (char*)malloc(sizeof(char) * (strlen(p->nume)+1));
    strcpy(nou->inf->nume, p->nume);
    nou->inf->varsta = p->varsta;
    nou->inf->balantaCont = p->balantaCont;

    nou->inf->valoare = p->valoare; // ca la int normal

    nou->next = NULL;
    nou->prev = NULL;
    if(cap == NULL){
        cap = nou;
        *coada = nou;
    }else{
        nodls* temp = cap;
        while(temp->next){
            temp = temp->next;
        }
        temp->next = nou;
        nou->prev = temp;
        *coada = nou;
    }
    return cap;
}

void traversare(nodls* cap){
    nodls* temp = cap;
    while(temp){
        printf("%s %d %.2f %hhu\n", temp->inf->nume, temp->inf->varsta, temp->inf->balantaCont, temp->inf->valoare);
        temp = temp->next;
    }
}

void traversInvers(nodls* coada){
    nodls* temp = coada;
    while(temp){
        printf("%s %d %.2f %hhu\n", temp->inf->nume, temp->inf->varsta, temp->inf->balantaCont, temp->inf->valoare);
        temp = temp->prev;
    }
}

void dezalocare(nodls* cap){
    nodls* temp = cap;
    while(temp){
        nodls* temp2 = temp->next;
        free(temp->inf->nume);
        free(temp->inf);
        free(temp);
        temp = temp2;
    }
}

int main(){
    nodls* cap = NULL;
    nodls* coada = NULL;
    char* token;
    char buff[100];
    char sep[3] = "\n,";
    FILE* f = fopen("pers.txt", "r");
    
    while(fgets(buff, sizeof(buff),f)){
        Persoana* p = (Persoana*)malloc(sizeof(Persoana));
        
        token = strtok(buff, sep);
        p->nume = (char*)malloc(sizeof(char) * (strlen(token)+1));
        strcpy(p->nume, token);

        token = strtok(NULL, sep);
        p->varsta = atoi(token);
        token = strtok(NULL, sep);
        p->balantaCont = atof(token);

        token = strtok(NULL, sep);
        p->valoare = atoi(token); //ca la int

        cap = insertSF(cap, &coada, p);
    }
    traversare(cap);
    dezalocare(cap);
}