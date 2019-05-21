#include <stdio.h>  

void nuovoNumero(int *numero);
void stampaRappresentazioneC2(int *numero);
void calcolaOpposto(int numero);
void stampaRappresentazioneC2Opposto(int *numero);
void esci(int *numero);


int main(){
    int numeroInput;
    printf("Inserire un numero intero:\n");
    while(!scanf("%d", &numeroInput)){
        printf("Errore. Inserisci un numero intero.\n");
        scanf("%*[^\n]%*c");
    }
    
    return 0;
}

void nuovoNumero(int *numero){
    printf("Inserisci un numero intero:\n");
    while(!scanf("%d", numero)){
        printf("Errore. Inserisci un numero intero.\n");
        scanf("%*[^\n]%*c");
    }
}

void stampaRappresentazioneC2(int *numero){
}

void calcolaOpposto(int numero){
    printf("%d\n", -numero);
}

void stampaRappresentazioneC2Opposto(int *numero){
}

void esci(int *numero){
    printf("Bye bye!\n");
    numero = 0;
}