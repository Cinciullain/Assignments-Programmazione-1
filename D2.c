#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define LUNGHEZZA_MASSIMA 30

struct grigliaPartenza{
    char nomePilota[LUNGHEZZA_MASSIMA];        
    char costruttoreVeicolo[LUNGHEZZA_MASSIMA];   
    int posizionePilota;  //posizione della partenza
    struct grigliaPartenza *prossimaPartenza; 
};
typedef struct grigliaPartenza partenza;
typedef partenza* partenzaPtr;

struct listaOrdinata{
    char nomePilota[LUNGHEZZA_MASSIMA];  
    //I tempi impiegati in gara dal pilota       
    int ore;    
    int minuti;    
    int secondi;   
    int millisecondi;  
    struct listaOrdinata *prossimalista;   
};
typedef struct listaOrdinata lista;
typedef lista* listaPtr;

void riempiGrigliaPosizioniPartenza(partenzaPtr*, char[], char[], int);        
void listaOrdinataPiloti(listaPtr*, char[], int, int, int, int);    
void stampaPodio(listaPtr, int);                                   
void classificaFinale(partenzaPtr*, listaPtr*, FILE*);             
void ricercaPartenza(partenzaPtr*, char[], listaPtr*, FILE*);   //Cerca la posizione del pilota nella lista della grilgia di partenza

int main (int argc, const char *argv[]){
    //Lettura primo file
    FILE *fileGrigliaPartenza = fopen(argv[1], "r");    //Puntatore al file grigliaPartenza.txt
    partenza *grigliaPartenzaPtr = NULL;   
    char nomePilota[LUNGHEZZA_MASSIMA], costruttoreVeicolo[LUNGHEZZA_MASSIMA];

    if(fileGrigliaPartenza == NULL){
        printf("impossibile aprire il file %s.\n", argv[1]);
        exit(EXIT_FAILURE);
    }
    else{
        int posizionePilota = 1;                         

        //Lettura di nomi e costruttori
        fgets(costruttoreVeicolo, sizeof(costruttoreVeicolo)/sizeof(costruttoreVeicolo[0]), fileGrigliaPartenza);
        fgets(nomePilota, sizeof(nomePilota)/sizeof(nomePilota[0]), fileGrigliaPartenza); 
        while(!feof(fileGrigliaPartenza)){
            costruttoreVeicolo[strlen(costruttoreVeicolo)-1] = '\0';
            nomePilota[strlen(nomePilota)-1] = '\0';
            riempiGrigliaPosizioniPartenza(&grigliaPartenzaPtr, nomePilota, costruttoreVeicolo, posizionePilota);
            posizionePilota++;
            fgets(costruttoreVeicolo, sizeof(costruttoreVeicolo)/sizeof(costruttoreVeicolo[0]), fileGrigliaPartenza);
            fgets(nomePilota, sizeof(nomePilota)/sizeof(nomePilota[0]), fileGrigliaPartenza); 
        }
    }
    fclose(fileGrigliaPartenza);
    
    //Lettura secondo file
    FILE *tempiPtr = fopen(argv[2], "r");    //Puntatore al file tempi.csv
    char  nomeMigliorPilota[LUNGHEZZA_MASSIMA];                               
    float velocitaMassimaRaggiunta = 0;                                   
    
    if(tempiPtr == NULL){  
        printf("impossibile aprire il file %s.\n", argv[2]);
        exit(EXIT_FAILURE);
    }
    else{
        float intermediate1, intermediate2, finishLine, speedTrap;  //Tempi

        //Scarto la prima riga
        while(fgetc(tempiPtr)!= '\n');

        //Lettura dei tempi
        fscanf(tempiPtr, "%s%*f%*f%*f%f",nomePilota, &speedTrap);
        while(!feof(tempiPtr)){
            if(velocitaMassimaRaggiunta < speedTrap){ //Se la velocita' massima e' minore dell velocita' letta, sostituisce con i nuovi dati
                velocitaMassimaRaggiunta = speedTrap;
                strcpy(nomeMigliorPilota, nomePilota);
            }
            fscanf(tempiPtr, "%s%*f%*f%*f%f",nomePilota, &speedTrap);
        }
    }

    //Lettura terzo file
    FILE *risultatiPtr = fopen(argv[3], "r");   //Puntatore al file risultati.txt
    lista *listaPtr = NULL;  //Puntatore alla lista ordinata di piloti in ordine di arrivo 

    if(risultatiPtr == NULL){
        printf("impossibile aprire il file %s.\n", argv[3]);
        exit(EXIT_FAILURE);
    }
    else{
        char nomePilota[LUNGHEZZA_MASSIMA];         
        int ore, minuti, secondi, millisecondi; //Il suo tempo di comletamento 

        //Lettura dei nomi e tempi
        fscanf(risultatiPtr,"%s", nomePilota);
        fscanf(risultatiPtr,"%d:%d:%d.%d", &ore, &minuti, &secondi, &millisecondi);
        
        while(!feof(risultatiPtr)){
            //Lo salva nella lista ordinata
            listaOrdinataPiloti(&listaPtr, nomePilota, ore, minuti, secondi, millisecondi); 
            fscanf(risultatiPtr,"%s", nomePilota);
            fscanf(risultatiPtr,"%d:%d:%d.%d", &ore, &minuti, &secondi, &millisecondi);
        }   
    }

    //Creazione del file di output, la classifica finale
    FILE *fileClassifica = fopen("classifica.csv", "w");    //Puntatore al file di output classifica.csv
    if(fileClassifica == NULL){
        printf("impossibile creare il file classifica.csv.\n");
        exit(EXIT_FAILURE);
    }
    else{
        //Stampa la classifica finale
        printf("Pilota  Costruttore   Griglia Tempi\n");
        fprintf(fileClassifica, "Pilota  Costruttore   Griglia Tempi\n");
        classificaFinale(&grigliaPartenzaPtr, &listaPtr, fileClassifica);
    }
    fclose(fileClassifica);

    int sceltaUtente = 0;
    do{
        printf("metti la tua scelta");
        scanf("%d", &sceltaUtente);
        switch(sceltaUtente){
            case 1:
                printf("Podio:\n");
                stampaPodio(listaPtr, 0);
                break;
            case 2:
                printf("Miglir Pilota:\n%s\n", nomeMigliorPilota);
                break;
        }
    }while(sceltaUtente != 3);
}

void riempiGrigliaPosizioniPartenza(partenzaPtr *listaPtr, char nomePilota[], char costruttoreVeicolo[], int posizionePilota){
    partenzaPtr nuovaPartenza = malloc(sizeof(partenza)); 
    if(nuovaPartenza == NULL){
        printf("memoria insufficente1.\n");
        exit(EXIT_FAILURE);
    }
    else{
        //Copia i dati necessari nei campi della lista
        strcpy(nuovaPartenza->nomePilota, nomePilota);
        strcpy(nuovaPartenza->costruttoreVeicolo, costruttoreVeicolo);
        nuovaPartenza->posizionePilota = posizionePilota;
        nuovaPartenza->prossimaPartenza = *listaPtr;
        *listaPtr = nuovaPartenza;
    }
}

void listaOrdinataPiloti(listaPtr *listaPtr, char nomePilota[], int ore, int minuti, int secondi, int millisecondi){
    //Inserisco i dati nella lista se vuota
    if(*listaPtr == NULL){
        listaPtr nuovoPtr = malloc(sizeof(lista));
        if(nuovoPtr == NULL){
            printf("memoria insufficiente.\n");
            exit(EXIT_FAILURE);
        } 
        else{
            strcpy(nuovoPtr->nomePilota, nomePilota);   
            nuovoPtr->ore = ore;
            nuovoPtr->minuti = minuti;
            nuovoPtr->secondi = secondi;
            nuovoPtr->millisecondi = millisecondi;
            nuovoPtr->prossimalista = NULL;
            (*listaPtr) = nuovoPtr;
        }
    }
    //Altrimenti controllo se si tratta di tempi nulli
    else{
        //Se i pilota ha tempi 0:0:0.0 allora inserisco i dati 
        if((0 == (*listaPtr)->ore) && (0 == (*listaPtr)->minuti) && (0 == (*listaPtr)->secondi) && (0 == (*listaPtr)->millisecondi)){
            listaPtr nuovoPtr = malloc(sizeof(lista));
            if(nuovoPtr == NULL){
                printf("memoria insufficiente.\n");
                exit(EXIT_FAILURE);
            } 
            else{
                strcpy(nuovoPtr->nomePilota, nomePilota);
                nuovoPtr->ore = ore;
                nuovoPtr->minuti = minuti;
                nuovoPtr->secondi = secondi;
                nuovoPtr->millisecondi = millisecondi;
                nuovoPtr->prossimalista = *listaPtr;
                (*listaPtr) = nuovoPtr;
            }
        }  
        else
            listaOrdinataPiloti(&((*listaPtr)->prossimalista), nomePilota, ore, minuti, secondi, millisecondi);
    }
}

void stampaPodio(listaPtr listaPtr, int contatore){
    //Se la lista e' null o il contatore arriva a 3 esce
    if((listaPtr == NULL) || (contatore >= 3))
        return;
    //Altrimenti stampa il nome del pilota
    else{
        printf("%s\n", listaPtr->nomePilota);
        stampaPodio(listaPtr->prossimalista, contatore++);
    }
}

void classificaFinale(partenzaPtr *grigliaPartenzaPtr, listaPtr *listaPtr, FILE *filePtr){
    if(*listaPtr == NULL)
        return;   
    else{
        classificaFinale(&(*grigliaPartenzaPtr), &(*listaPtr)->prossimalista, &(*filePtr));
        ricercaPartenza(&(*grigliaPartenzaPtr), (*listaPtr)->nomePilota, &(*listaPtr), &(*filePtr));
    }
}

void ricercaPartenza(partenzaPtr *grigliaPartenzaPtr, char nomePilota[], listaPtr *listaPtr, FILE *filePtr){
    if((*grigliaPartenzaPtr) == NULL){
        printf("%s %s\n",(*grigliaPartenzaPtr)->nomePilota, nomePilota);
        printf("partenza del pilota non trovata.\n");
        exit(EXIT_FAILURE);
    }
    else{
        if(strcmp((*grigliaPartenzaPtr)->nomePilota, nomePilota) == 0){
            printf("%s\t%s\t%d\t%d:%d:%d.%d\n", (*listaPtr)->nomePilota, (*grigliaPartenzaPtr)->costruttoreVeicolo, (*grigliaPartenzaPtr)->posizionePilota, (*listaPtr)->ore, (*listaPtr)->minuti, (*listaPtr)->secondi, (*listaPtr)->millisecondi);
            fprintf(filePtr,"%s\t%s\t%d\t%d:%d:%d.%d\n", (*listaPtr)->nomePilota, (*grigliaPartenzaPtr)->costruttoreVeicolo, (*grigliaPartenzaPtr)->posizionePilota, (*listaPtr)->ore, (*listaPtr)->minuti, (*listaPtr)->secondi, (*listaPtr)->millisecondi);

            partenzaPtr tempPtr = *grigliaPartenzaPtr;
            *grigliaPartenzaPtr = tempPtr->prossimaPartenza;
            free(tempPtr);
        }
        else
            ricercaPartenza(&(*grigliaPartenzaPtr)->prossimaPartenza, nomePilota, &(*listaPtr), &(*filePtr));
    }
}
