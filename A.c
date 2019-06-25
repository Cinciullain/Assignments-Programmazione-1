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
                                  
    for( ; ;){
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
    int maschera[NUMERO_BIT] = {};
	for(int i = 0; numero > 0; i++) {
		maschera[i] = numero % 2;
		numero = numero / 2;
	}

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
    /* printf("La rappresentazione dell\'opposto di %d in complemento a due (MSB) è: ", numero);
    if(numero != 0)
        numero*= -1;
    int maschera[NUMERO_BIT] = {};
	for(int i = 0; numero > 0; i++) {
		maschera[i] = numero % 2;
		numero = numero / 2;
	}
    if(numero != 0){
        for(int i = NUMERO_BIT; i > 0; i--){
            if(i%4 == 0 && i != NUMERO_BIT)
                printf(" ");
            if(maschera[i - 1] == 0)
                printf("1");
            else
                printf("0");

        }
    }
    else{
        for(int i = NUMERO_BIT; i > 0; i--){
            if(i%4 == 0 && i != NUMERO_BIT)
                printf(" ");
            
            printf("0");
        }
    }

	printf(" \n\n");
    */
    int j = 1;              //CONTATORE PER SEPARARE I BIT IN GRUPPI DI 4
    int temp = numero * -1;     //CALCOLO DEL NUMERO OPPOSTO

    //CONSULTARE LA FUNZIONE complementoA2 (riga 99)
    unsigned int maschera = 1 << (NUMERO_BIT-1);
    printf("La rappresentazione dell\'opposto di %d in complemento a due (MSB) è: ", numero);
    for(int i = 0; i < NUMERO_BIT; i++)
    {
        putchar(temp & maschera ? '1' : '0');
        temp <<= 1;
        if(0 == j % 4)// SE J E' DIVISIBILE PER 4 ALLORA METTE UNO SPAZIO
        {
            printf(" ");
        }
        j++;
    }
    printf("\n\n");

	return 0;
}

int esci(int numero){
    printf("Bye bye!\n");
    exit(1);
    return 0;
}