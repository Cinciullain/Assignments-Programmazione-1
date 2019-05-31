#include <stdio.h>  

#define NUMERO_FUNZIONI 5
#define NUMERO_BIT 32 

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
}

void calcolaOpposto(int *numero){
    printf("L\'opposto di %d e: -%d\n",*numero ,*numero);
}

void stampaRappresentazioneC2Opposto(int *numero){
}

void esci(int *numero){
    printf("Bye bye!\n");
    *numero = 1;
}