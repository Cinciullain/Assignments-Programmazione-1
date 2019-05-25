#include <stdio.h>  
#include <stdlib.h>

#define NUMERO_FUNZIONI 5

void nuovoNumero(int *numero);
void stampaRappresentazioneC2(int *numero);
void calcolaOpposto(int *numero);
void stampaRappresentazioneC2Opposto(int *numero);
void esci(int *numero);


int main(){
    int numeroScelto;
    int funzioneScelta;
    int exitProgramma = 0;
    printf("Inserire un numero intero:\n");
    nuovoNumero(&numeroScelto);
    
    void (*puntatore_funzioni[NUMERO_FUNZIONI])(int*) = {nuovoNumero, stampaRappresentazioneC2, calcolaOpposto, stampaRappresentazioneC2Opposto, esci};
                                  
    while(!exitProgramma){
        printf("Scegliere un\'opzione (1 - Nuovo numero; 2 - Stampa rappresentazione C2; 3 - Calcola opposto; 4 - Stampa rappresentazione C2 dell\'opposto; 5 - Esci):");
        nuovoNumero(&funzioneScelta);
        if(funzioneScelta >= 1 && funzioneScelta <= 5)
            puntatore_funzioni[funzioneScelta - 1];
    }
    return 0;
}

void nuovoNumero(int *numero){
    while(!scanf("%d", numero)){
        printf("Errore. Inserisci un numero intero.\n");
        scanf("%*[^\n]%*c");
    }
}

void stampaRappresentazioneC2(int *numero){

    char *rappresentazioneC2;
    rappresentazioneC2 = malloc(sizeof(char)*36 + 1);
    rappresentazioneC2[37] = '\0';
    int i = 36;
}

void calcolaOpposto(int *numero){
    printf("%d\n", - *numero);
}

void stampaRappresentazioneC2Opposto(int *numero){
    int i = 0;
    char *rappresentazioneC2;
    rappresentazioneC2 = malloc(sizeof(char)*36 + 1);
    rappresentazioneC2[37] = '\0';

    while(rappresentazioneC2[i] != '\0'){
        if(rappresentazioneC2[i] == '0')
            rappresentazioneC2[i] = '1';
        else
            rappresentazioneC2[i] = '0';
        
        i++;
    }

    printf("%s", rappresentazioneC2);
    
}

void esci(int *numero){
    printf("Bye bye!\n");
    *numero = 1;
}