#include <stdio.h>  
#include <stdlib.h>

#define NUMERO_FUNZIONI 5
#define NUMERO_BIT 32 

int nuovoNumero(int *numero);
int stampaRappresentazioneC2(int numero);
int calcolaOpposto(int numero);
int stampaRappresentazioneC2Opposto(int numero);
int esci(int numero);


int main(){
    int numeroScelto;
    int funzioneScelta;
    printf("Inserisci un numero intero: ");
    nuovoNumero(&numeroScelto);

    int (*puntatore_funzioni[NUMERO_FUNZIONI])() = {nuovoNumero, stampaRappresentazioneC2, calcolaOpposto, stampaRappresentazioneC2Opposto, esci};
                                  
    for(;;){
        printf("Scegli un\'opzione: \n");
        printf("\t1) Inserisci un nuovo numero.\n");
        printf("\t2) Stampa rappresentazione in complemento a due.\n");
        printf("\t3) Calcola opposto.\n");
        printf("\t4) Stampa rappresentazione in complemento a due dell'opposto.\n");
        printf("\t5) Esci.\n");
        nuovoNumero(&funzioneScelta);
        if(funzioneScelta >= 1 && funzioneScelta <= 5){
            printf("Scelta:  ");
            switch(funzioneScelta){
                case 1:
                     printf("Inserisci un numero intero.\n");
                    (*puntatore_funzioni[funzioneScelta -1]) (&numeroScelto);
                    break;
                default:
                    (*puntatore_funzioni[funzioneScelta -1]) (numeroScelto);
            }
        }
    }
    return 0;
}

int nuovoNumero(int *numero){
    while(!scanf("%d", numero)){
        printf("Errore. Inserisci un numero intero.\n");
        scanf("%*[^\n]%*c");
    }
}

int stampaRappresentazioneC2(int numero){
    printf("La rappresentazione di %d in complemento a due (MSB) è: ", numero);
    int maschera[NUMERO_BIT] = {}; //Crea la maschera di 32 bit tutti a 0

    //Genero la rappresentazione
	for(int i = 0; numero > 0; i++) {
		maschera[i] = numero % 2;
		numero = numero / 2;
	}

    //Metto gli spazi ogni 4 bit
	for(int i = NUMERO_BIT; i > 0; i--){
		if(i%4 == 0 && i != NUMERO_BIT)
			printf(" ");
		printf("%d", maschera[i - 1]);
	}

	printf(" \n\n");

	return 0;
}

int calcolaOpposto(int numero){ 
    if (numero != 0)
        printf("L\'opposto di %d è: %d\n",numero ,numero * -1);
    else
        printf("L\'opposto di %d è: %d\n",numero ,numero);

    return 0;
}

int stampaRappresentazioneC2Opposto(int numero){
    int contatore = 1;     //Per separare i bit in gruppi di 4
    int temp = numero * -1;     //Calcola il numero opposto

    //Creo la maschera per fare il confronto bit a bit
    unsigned int maschera = 1 << (NUMERO_BIT-1); //La maschera diventa 1000 0000 0000 0000 0000 0000 0000 0000
    printf("La rappresentazione dell\'opposto di %d in complemento a due (MSB) è: ", numero);
    
    //Per ogni bit fa il confronto e a ogni iterazione li sposta di una posizione
    for(int i = 0; i < NUMERO_BIT; i++){
        putchar(temp & maschera ? '1' : '0');
        temp <<= 1;
        if(0 == contatore % 4)//Mette uno spazio ogni 4 bit
            printf(" ");

        contatore++;
    }
    printf("\n\n");
	return 0;
}

int esci(int numero){
    printf("Bye bye!\n");
    exit(1);
    return 0;
}
