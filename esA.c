#include <stdio.h>  

#define NUMERO_FUNZIONI 5

void nuovoNumero(int *numero);
char *stampaRappresentazioneC2(int numero);
void calcolaOpposto(int numero);
char *stampaRappresentazioneC2Opposto(char *rappresentazioneC2);
void esci(int *numero);


int main(){
    int numeroScelto;
    int funzioneScelta;
    int exitProgramma = 0;
    printf("Inserire un numero intero:\n");
    nuovoNumero(&numeroScelto);
    
    int(*f[NUMERO_FUNZIONI]) (int) = {nuovoNumero, stampaRappresentazioneC2, calcolaOpposto, stampaRappresentazioneC2Opposto, esci};
                                  
    while(!exitProgramma){
        printf("Scegliere un\'opzione (1 - Nuovo numero; 2 - Stampa rappresentazione C2; 3 - Calcola opposto; 4 - Stampa rappresentazione C2 dell\'opposto; 5 - Esci):")
        nuovoNumero(&funzioneScelta);
        switch (funzioneScelta){
            case 1: 
                printf("Inserisci un numero intero:\n");
                nuovoNumero(&numeroScelto);
                break;
            case 2:
                printf("%s\n", stampaRappresentazioneC2(numeroScelto));
                break;
            case 3:
                calcolaOpposto(numeroScelto);
                break;
            case 4:
                printf("%s", stampaRappresentazioneC2Opposto(stampaRappresentazioneC2(numeroScelto)));
                break;
            case 5:
                esci();
            default:
                break;
        }
    }
    return 0;
}

void nuovoNumero(int *numero){
    //printf("Inserisci un numero intero:\n");
    while(!scanf("%d", numero)){
        printf("Errore. Inserisci un numero intero.\n");
        scanf("%*[^\n]%*c");
    }
}

void stampaRappresentazioneC2(int numero){

    char *rappresentazioneC2;
    rappresentazioneC2 = malloc(sizeof(char)*36 + 1);
    rappresentazioneC2[37] = '\0';
    int i = 36;

    while(){
        
    }


}

void calcolaOpposto(int numero){
    printf("%d\n", -numero);
}

void stampaRappresentazioneC2Opposto(char *rappresentazioneC2){
    int i = 0;

    while(rappresentazioneC2(i) != '\0'){
        if(rappresentazioneC2(i) == '0')
            rappresentazioneC2(i) = '1';
        else
            rappresentazioneC2(i) = '0';
        
        i++;
    }

    printf("%s", rappresentazioneC2);
    
}

void esci(int *numero){
    printf("Bye bye!\n");
    numero = 1;
}