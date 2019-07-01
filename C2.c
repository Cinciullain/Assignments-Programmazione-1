#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define LIMITE_LUNGHEZZA 30

typedef struct parola{
    char parolaEstratta[LIMITE_LUNGHEZZA];
    unsigned int ricorrenze;
    struct parola *nextPtr;
} Parola;
typedef Parola *ParolaPtr;

unsigned int numeroTotaleParole;

void getParole(FILE *fPtr, ParolaPtr *prima, unsigned int *percentuale);
int ordinaParole(char *testo, ParolaPtr *prima, unsigned int *percentuale);
void stampa(ParolaPtr *prima, unsigned int *percentuale);

int main(int argc, const char *argv[]){
    Parola *primaParola = NULL;
    FILE *testo = fopen(argv[1], "r");
    unsigned int percentuale[31] = {0};

    getParole(testo, &primaParola, percentuale);
    stampa(&primaParola, percentuale);

    return 0;
}

void getParole(FILE *fPtr, ParolaPtr *prima, unsigned int *percentuale){
    char stringaTemporanea[100];
    while (fscanf(fPtr, "%[A-zàèìòù]%*[^A-zàèìòù]", stringaTemporanea) >= 1){
        if(strlen(stringaTemporanea) == 0) 
            continue;
        for(unsigned int i = 0; i < strlen(stringaTemporanea); i++){ //I caratteri maiuscoli li rendo minuscoli
            if(stringaTemporanea[i] <= 90)
                stringaTemporanea[i] += 32;
        }
    }
    ordinaParole(stringaTemporanea, prima, percentuale);    
}

int ordinaParole(char *testo, ParolaPtr *prima, unsigned int *percentuale){
    ParolaPtr nuovaParola = malloc(sizeof(Parola));
    if(nuovaParola != NULL){
        strcpy(nuovaParola->parolaEstratta, testo);
        nuovaParola->ricorrenze = 1;

        ParolaPtr corrente = *prima;
        ParolaPtr precedente = NULL;

        while(corrente != NULL && strcmp(corrente->parolaEstratta, nuovaParola->parolaEstratta) < 0){
            precedente = corrente;
            corrente = corrente->nextPtr;
        }

        if(precedente == NULL){
            if(corrente != NULL && strcmp(corrente->parolaEstratta, nuovaParola->parolaEstratta) == 0)
                corrente->ricorrenze++;
            else{
                nuovaParola->nextPtr = *prima;
                *prima = nuovaParola;
            }
        }
        else if(corrente == NULL)
            precedente->nextPtr = nuovaParola;
        else{
            if(strcmp(corrente->parolaEstratta, nuovaParola->parolaEstratta) == 0)
                corrente->ricorrenze++;
            else{
                precedente->nextPtr = nuovaParola;
                nuovaParola->nextPtr = corrente;
            }
        }

        numeroTotaleParole++;
        percentuale[strlen(nuovaParola->parolaEstratta)]++;
    }
}

void stampa(ParolaPtr *prima, unsigned int *percentuale){
    printf("%30s%16s\n\n", "Lunghezza", "Frequenza (%)");
    for(size_t j = 1; j <= LIMITE_LUNGHEZZA; j++){
        if(percentuale[j] == 0)
            continue;
        printf("%30lu%16.2f\n", j, (float) (percentuale[j] * 100) / numeroTotaleParole);
    }

    puts("\n");

    ParolaPtr temp = *prima;
    printf("%30s%12s\n\n", "Parola", "Frequenza");
    while(temp != NULL){
        printf("%30s   ", temp->parolaEstratta);
        for(unsigned int t = 0; t < temp->ricorrenze; t++){
            printf("%c", '*');
        }
        printf("\n");
        temp = temp->nextPtr;
    }
}
