#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


typedef struct Magazin{
    char* numeProdus;
    float pret;
    char categorie;
}Magazin;

typedef struct nodls{
    struct Magazin* inf;
    struct nodls* next, *prev;
}nodls;

nodls* insertSF(nodls* cap, nodls** coada,Magazin* magazinNou){
    nodls* nou = (nodls*)malloc(sizeof(nodls));
    nou->inf = (Magazin*)malloc(sizeof(Magazin));

    nou->inf->numeProdus = (char*)malloc((strlen(magazinNou->numeProdus) + 1) * sizeof(char));
    strcpy(nou->inf->numeProdus, magazinNou->numeProdus);
    nou->inf->pret = magazinNou->pret;
    nou->inf->categorie = magazinNou->categorie;

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
        printf("%s %0.2f %c\n", temp->inf->numeProdus, temp->inf->pret, temp->inf->categorie);
        temp = temp->next;
    }
}

void traversInvers(nodls* coada){
    nodls* temp = coada;
    while(temp){
        printf("%s %0.2f %c\n", temp->inf->numeProdus, temp->inf->pret, temp->inf->categorie);
        temp = temp->prev;
    }
}

void dezalocare(nodls* cap){
    nodls* temp = cap;
    while(temp){
        nodls* temp2 = temp->next;
        free(temp->inf->numeProdus);
        free(temp->inf);
        free(temp);

        temp = temp2;
    }
}

float produsPretMinim(nodls* cap){
    nodls* temp = cap;
    float min = temp->inf->pret;
    while(temp){
        if(temp->inf->pret < min){
            min = temp->inf->pret;
        }
        temp = temp->next;
    }
    return min;
}

float mediePreturiProduse(nodls* cap){
    nodls* temp = cap;
    int nrProduse = 0;
    float preturiProduse = 0;
    while(temp){
        preturiProduse += temp->inf->pret;
        nrProduse++;
        temp = temp->next;
    }
    return preturiProduse / nrProduse;
}


int nrPestePrag(nodls* cap, int prag){
    nodls* temp = cap;
    int counter = 0;
    while(temp){
        if(temp->inf->pret > prag){
            counter++;
        }
        temp = temp->next;
    }
    return counter;
}

nodls* afisareProdusePestePrag(nodls* cap, int prag){
    nodls* temp = cap;
    nodls* produse = temp;
    while(temp){
        if(temp->inf->pret > prag){
            produse = temp;
        }
        temp = temp->next;
    }
    return produse;
}

int main(){
    nodls* cap = NULL;
    nodls *coada = NULL;
    char* token;
    char buff[300];
    char sep[3] = ",\n";
    
    FILE* f = fopen("magazin.txt", "r");
    while(fgets(buff, sizeof(buff),f)){
        Magazin* m = (Magazin*)malloc(sizeof(Magazin));

        token = strtok(buff, sep);
        m->numeProdus = (char*)malloc(sizeof(char)*strlen(token)+1);
        strcpy(m->numeProdus, token);

        token = strtok(NULL, sep);
        m->pret = atof(token);

        token = strtok(NULL, sep);
        m->categorie = *token;

        cap = insertSF(cap, &coada, m);
    }
    traversare(cap);
    printf("\n");
    //traversInvers(coada);

    float minim = produsPretMinim(cap);
    printf("\nPret produs minim: %0.2f", minim);
    
    float medie = mediePreturiProduse(cap);
    printf("\nMedie preturi produse: %0.2f", medie);

    int contorPrag = nrPestePrag(cap, 10);
    printf("\nProduse peste prag: %d", contorPrag);

    nodls* produsePrag = afisareProdusePestePrag(cap, 40);
    printf("\n*********** PESTE PRAG ***********\n");
    traversare(produsePrag);

    dezalocare(cap);
}