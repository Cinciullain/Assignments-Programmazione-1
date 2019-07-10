#include <stdio.h>
#include <stdlib.h>
#include <math.h>

struct occorrenze{
    int valoreOccorrenza;          
    int numeroOccorrenza;   //Quante volte occorre il numero
    int posizione1;    //Quale riga/colonna appartiene
    int posizione2;    //Da quale riga/colonna inizia
    int colonna;       //Flag per indicare se colonna o riga
    struct occorrenze *prossimaOcc;    
};
typedef struct occorrenze Occorrenze;
typedef Occorrenze *occorrenzePtr;

int inputNumericoPositivo();      
int radice(int, int);   //Calcola radice perfetta
void inserisciOccorrenzeInLista(occorrenzePtr*, int numeroOccorrenze, int posizione1, int posizione2, int valore, int colonna);   
void stampaOccorrenze(occorrenzePtr);   //Stampa Output                                                                
void assegnaValoriOccorrenza(occorrenzePtr*, int numeroOccorrenze, int posizione1, int posizione2, int valore, int colonna);  

int main(int argc, const char *argv[]){
    FILE *filePtr;             
    if((filePtr  = fopen(argv[1], "r")) == NULL){
        printf("Impossibile aprire il file %s.", argv[1]);
        exit(EXIT_FAILURE);
    }

    int numeroElementi = 0;           
    int elementoLetto = 0;      
    int dimensioneMatrice = 0;      
    int i, j;             

    //Conta quanti elementi sono presenti e ne calcola la radice, e una volta contati torno all'inizio del file
    while(fscanf(filePtr,"%d", &elementoLetto) == 1){
        numeroElementi++;
    }
    rewind(filePtr);

    //Se il file non ha radice perfetta o < 4, il programma termina
    dimensioneMatrice = radice(numeroElementi, 1); 
    if(dimensioneMatrice < 4){
        printf("File non valido.\n");
        exit(EXIT_FAILURE);
    }

    //Lettura file e inserimento dati nella matrice
    int **matriceInput = (int**)calloc(dimensioneMatrice, sizeof(int*));
    for(i = 0; i < dimensioneMatrice; i++){
        matriceInput[i] = (int*)calloc(dimensioneMatrice, sizeof(int));
        for(j = 0; j < dimensioneMatrice; j++){
            fscanf(filePtr,"%d", &matriceInput[i][j]);
            printf("%d ", matriceInput[i][j]);
        }
        printf("\n");
    }
    fclose(filePtr); 
  
    printf("Inserire M: \n");
    int m = inputNumericoPositivo();  
    while(m > dimensioneMatrice){ 
        printf("M deve essere minore di N");
        m = inputNumericoPositivo();
    }

    //Ricerca occorrenze, prima in colonna e poi in riga
    int valoreTemporaneoColonna = 0, valoreTemporaneoRiga = 0;    //Per contenere il valore della sequenza
    int riga, colonna;  //Indici
    int posizioneColonna = 0, posizioneRiga = 0;    //Dove Inizia l'occorrenza
    int contatoreOccorrenzeColonna, contatoreOccorrenzeRiga;                  
    Occorrenze *occPtr = NULL;    

    /*
        Tengo un indice che individua la posizione iniziale laddove inizia l'occorrenza.
        Una volta trovato un numero diverso dalla posizione iniziale verifico se la consecutivita' supera M:
            -Se lo supera mette le informazioni necessarie nella lista
            -Altrimenti resetta l'indice alla posizione riga

    */
    for(colonna = 0; colonna < dimensioneMatrice; colonna++){
        //A ogni iterazione resetto i contatori
        contatoreOccorrenzeColonna = 1;    
        contatoreOccorrenzeRiga = 1;

        for(riga = 1, posizioneColonna = 0, posizioneRiga = 0; riga < dimensioneMatrice; riga++){
            //Ricerca in colonna
            if(matriceInput[riga][colonna] == matriceInput[posizioneColonna][colonna]){
                contatoreOccorrenzeColonna += 1;
                valoreTemporaneoColonna = matriceInput[posizioneColonna][colonna];
                //Se l-indice arriva all'ultimo elemento, controlla se il contatore supera M. Se si mette in lista, altrimenti non fa nulla
                if((riga == (dimensioneMatrice - 1)) && (contatoreOccorrenzeColonna >= m)){ 
                    inserisciOccorrenzeInLista(&occPtr,contatoreOccorrenzeColonna, colonna, posizioneColonna, valoreTemporaneoColonna, 1);
                    contatoreOccorrenzeColonna = 1;
                }
            }
            else{
                if(contatoreOccorrenzeColonna >= m)
                    inserisciOccorrenzeInLista(&occPtr,contatoreOccorrenzeColonna, colonna, posizioneColonna, valoreTemporaneoColonna, 1);

                posizioneColonna = riga;
                contatoreOccorrenzeColonna = 1;
            }

            //Ricerca in riga, cambiano solo gli indici
            if(matriceInput[colonna][riga] == matriceInput[colonna][posizioneRiga]){
                contatoreOccorrenzeRiga += 1;
                valoreTemporaneoRiga = matriceInput[colonna][posizioneRiga];
                if((riga == (dimensioneMatrice - 1)) && (contatoreOccorrenzeRiga >= m)){
                    inserisciOccorrenzeInLista(&occPtr, contatoreOccorrenzeRiga, colonna, posizioneRiga, valoreTemporaneoRiga, 0);
                    contatoreOccorrenzeRiga = 1;
                }
            }
            else{
                if(contatoreOccorrenzeRiga >= m)
                    inserisciOccorrenzeInLista(&occPtr, contatoreOccorrenzeRiga, colonna, posizioneRiga, valoreTemporaneoRiga, 0);
                
                posizioneRiga = riga;
                contatoreOccorrenzeRiga = 1;
            }
        }
    }
    stampaOccorrenze(occPtr);
}

//Stampa output
void stampaOccorrenze(occorrenzePtr oPtr)     {
    if(oPtr == NULL)
        return;
    else{
        if(oPtr->colonna == 0)  // Il flag della struttura indica quale frase utilizzare
            printf("Ci sono %d occorrenze consecutive di %d in riga %d a partire della colonna %d\n", oPtr->numeroOccorrenza, oPtr->valoreOccorrenza, oPtr->posizione1, oPtr->posizione2);
        else
            printf("Ci sono %d occorrenze consecutive di %d in colonna %d a partire della riga %d\n", oPtr->numeroOccorrenza, oPtr->valoreOccorrenza, oPtr->posizione1, oPtr->posizione2);
        stampaOccorrenze(oPtr->prossimaOcc);
    }
}

/*
    Molteplici casi da controllare:
        -Se la lista e' vuota, inserisce un elemento. Altrimenti controlla se il numero di occorrenze che sta leggendo e' minore, maggiore o uguale.
            -Se minore inserisce nella lista
            -Se maggiore va al prossimo elemento e ricomincia i controlli
            -Se uguale va a controllare se l'elemento entrante e' dello stesso tipo di quello presente
        -Se l'elemento entrante si tratta di una colonna e l'elemento presente non lo e', viene inserito
        -Se sono dello stesso tipo controlla la posizione di chi inizia prima
            -Se l'elemento entrante inizia prima, viene inserito, altrimenti va al prossimo elemento della lista
            -Se l'elemento entrante si tratta di una riga e l'elemento presente non lo e', va al prossimo elemento della lista

 */
void inserisciOccorrenzeInLista(occorrenzePtr *oPtr, int numeroOccorrenze, int posizione1, int posizione2, int valore, int colonna){
    if(*oPtr == NULL)
        assegnaValoriOccorrenza(oPtr, numeroOccorrenze, posizione1, posizione2, valore, colonna);

    else if((*oPtr)->numeroOccorrenza < numeroOccorrenze)
        assegnaValoriOccorrenza(oPtr, numeroOccorrenze, posizione1, posizione2, valore, colonna);
    
    else if((*oPtr)->numeroOccorrenza == numeroOccorrenze){
        if((colonna > (*oPtr)->colonna))
            assegnaValoriOccorrenza(oPtr, numeroOccorrenze, posizione1, posizione2, valore, colonna);

        else if(colonna == (*oPtr)->colonna){
            if(posizione1 > (*oPtr)->posizione1)
                assegnaValoriOccorrenza(oPtr, numeroOccorrenze, posizione1, posizione2, valore, colonna);
            else
                inserisciOccorrenzeInLista((&(*oPtr)->prossimaOcc), numeroOccorrenze, posizione1, posizione2, valore, colonna);
        }
        else
            inserisciOccorrenzeInLista((&(*oPtr)->prossimaOcc), numeroOccorrenze, posizione1, posizione2, valore, colonna);
    }
    else
        inserisciOccorrenzeInLista((&(*oPtr)->prossimaOcc), numeroOccorrenze, posizione1, posizione2, valore, colonna);
}

//Inserisce i valore in lista
void assegnaValoriOccorrenza(occorrenzePtr *oPtr, int numeroOccorrenze, int posizione1, int posizione2, int valore, int colonna){
    occorrenzePtr nuovoPtr = malloc(sizeof(Occorrenze));
    if(nuovoPtr != NULL){
        nuovoPtr->numeroOccorrenza = numeroOccorrenze;
        nuovoPtr->posizione1 = posizione1;
        nuovoPtr->posizione2 = posizione2;
        nuovoPtr->colonna = colonna;
        nuovoPtr->valoreOccorrenza = valore;
        nuovoPtr->prossimaOcc = *oPtr;
        *oPtr = nuovoPtr;
    }
    else{
        printf("memoria insufficiente\n");
        exit(EXIT_FAILURE);
    }
}

int radice(int i, int j){
    if(j*j == i)
        return j;
    else{
        if(j*j < i)
            radice(i, j + 1);
        else{
            printf("File non valido.\n");
            exit(EXIT_FAILURE);
        }
    }
}

int inputNumericoPositivo(){
    int n;
    while((scanf("%d", &n) < 1) || (n < 1)){
        printf("inserire un numero intero");
        while(getchar() != '\n');
    }
    return n;
}