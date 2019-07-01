#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct prodotto{
    int codiceProdotto; 
    int quantitaProdotto;        
    float prezzoProdotto; 
    char nomeProdotto[20];  
    struct prodotto *prossimoProdotto;
};

typedef struct prodotto Prodotto;
typedef Prodotto *ProdottoPtr;

void assegnaValoriProdotto(ProdottoPtr*, int codiceProdotto, float prezzoProdotto, char nomeProdotto[]);     
void inizialize(ProdottoPtr*, int, float, char[]);   //inizializza la lista con elementi presenti nel file
void cambiaQuantitaProdotto(ProdottoPtr*, int);      
void cambiaPrezzoProdotto(ProdottoPtr*, int);        
void nuovoProdotto(ProdottoPtr*, int);               //inserisce un nuovo prodotto nella lista
void eliminaProdotto(ProdottoPtr*, int);             
void cercaProdotto(ProdottoPtr*, int);               
void listaProdotti(ProdottoPtr*, int);               
int numeroProdottiDisponibili(ProdottoPtr, int);             
int underThreshold(ProdottoPtr, int, int);           //stmpa il numero di prodotti sotto soglia
void order(ProdottoPtr, int threshold, FILE*);       //scrive nel file i prodotti sotto soglia da ordinare
 
int main(){
    srand(10);                                  //seme per la pseudocasualita'
    FILE *filePtr;                                 //puntatore al file di input elenProdotti.csv
    filePtr = fopen("elencoProdotti.csv", "r");    
    Prodotto *ProdottoPtr = NULL;           

    /*   LETTURA DAL FILE    */
    if(filePtr == NULL){   
        printf("impossibile aprire il file");
        exit(EXIT_FAILURE);
    }
    else{
        int codiceProdotto = 0;                           
        float prezzoProdotto = 0;                        
        char nomeProdotto[11];                          
        while(!feof(filePtr)){
            if(fscanf(filePtr,"%d %s %f\n", &codiceProdotto, nomeProdotto, &prezzoProdotto) < 3){  //se il file non ha campi formattati bene esce dal programma
                printf("file non formattato bene.\n");
                printf("%d,%s,%f\n", codiceProdotto, nomeProdotto, prezzoProdotto);
                exit(EXIT_FAILURE);
            }
            //inserisce il prodotto
            inizialize(&ProdottoPtr, codiceProdotto, prezzoProdotto, nomeProdotto); 
        }
        fclose(filePtr);
    }
    /*   FINE LETTURA      */

    printf("stampa tutti i prodotti\n");
    stampaLista(ProdottoPtr);

    /*   OPZIONI DELL'UTENTE   */
    int opzioneScelta = 0;                             
    int codiceProdotto = 0;                              
    int threshold = 10;                         //soglia (default 10)
    void (*funzioniPtr[6])(ProdottoPtr*, int) = {cambiaQuantitaProdotto, cambiaPrezzoProdotto, nuovoProdotto, eliminaProdotto, cercaProdotto, listaProdotti};    //array di puntatori a funzioni

    do{
        //MENU' TESTUALE 
        printf("Scegliere un\'opzione(1 - variazione della quantita\' di un prodotto;");
        printf("2 - variazione del prezzo di un prodotto;");
        printf("3 - inserimento di un nuovo prodotto;");
        printf("4 - eliminazione di un prodotto;");
        printf("5 - Ricerca di un prodotto;");
        printf("6 - Stampa prodotti sotto al soglia;");
        printf("7 - Esci)\n");

        //lettura dell'opzione dell'utente
        while((scanf("%d",&opzioneScelta) != 1) || (opzioneScelta < 1) || (opzioneScelta > 7)){
            printf("scelta non valida riprovare\n");
            while(getchar()!= '\n');
        }

        //in base alla lettura chiama la funzione corrispondente
        if((opzioneScelta > 0) && (opzioneScelta < 6)){
            printf("Inserisci il codice del prodotto:\n");
            while(scanf("%d", &codiceProdotto) != 1){
                printf("Inserisci il codice del prodotto:\n");
                while(getchar() != '\n');
            }
            (*funzioniPtr[opzioneScelta - 1])(&ProdottoPtr, codiceProdotto);
        }
        else if(6 == opzioneScelta){
            printf("Inserisci la soglia:\n");
            while(scanf("%d", &threshold)!= 1){
                printf("Inserisci la soglia:\n");
                while(getchar() != '\n');
            }
            (*funzioniPtr[opzioneScelta - 1])(&ProdottoPtr, threshold);
        }
    }while(opzioneScelta != 7);

    /*   UTENTE CHIEDE L'USCITA DAL PROGRAMMA      */

    int prodottiDisponibili = numeroProdottiDisponibili(ProdottoPtr, 0);    //numero di prodotti differenti disponibili
    int underThresholdNumber = underThreshold(ProdottoPtr,threshold, 0);    //numero di prodotti sotto soglia
    printf("Il numero totale di prodotti disponibili in magazzino e\': %d\n", prodottiDisponibili); 
    printf("Il numero totale di prodotti sotto la soglia e\': %d\n", underThresholdNumber);

    /*   APERTURA DEL FILE totale.txt      */
    FILE *totalPtr;                             //puntatore al file di output totale.txt
    totalPtr = fopen("numeroTotale.txt", "w");
    if(totalPtr == NULL){                        //stampa il numero dei prodotti se il file viene aperto 
        printf("impossibile aprire il file.\n");
        exit(EXIT_FAILURE);
    }
    else{
        fprintf(totalPtr, "Il numero totale di prodotti disponibili in magazzino e\': %d\n", prodottiDisponibili);
        fprintf(totalPtr, "Il numero totale di prodotti sotto la soglia e\': %d\n", underThresholdNumber);
    }
    fclose(totalPtr);

    /*   APERTURA DEL FILE daOrdinare.bin      */
    FILE *daOrdinarePtr;                               //puntatore al file di output daOrdinare.bin
    daOrdinarePtr = fopen("daOrdinare.bin", "wb");
    if(daOrdinarePtr == NULL){
        printf("impossibile aprire il file.\n");
        exit(EXIT_FAILURE);
    }
    else
        order(ProdottoPtr, threshold, daOrdinarePtr);
    
    printf("Bye Bye\n");
    fclose(daOrdinarePtr);
}

//INSERIMENTO DEL PRODOTTO CON CODIDE CRESCENTE
void inizialize(ProdottoPtr *prodotto, int codiceProdotto, float prezzoProdotto, char nomeProdotto[]){
    if(*prodotto == NULL)
        assegnaValoriProdotto(prodotto, codiceProdotto, prezzoProdotto, nomeProdotto);
    else if((*prodotto)->codiceProdotto > codiceProdotto)  
        assegnaValoriProdotto(prodotto, codiceProdotto, prezzoProdotto, nomeProdotto);
    else
        inizialize(&((*prodotto)->prossimoProdotto),codiceProdotto, prezzoProdotto, nomeProdotto);
}

//STAMPA LA LISTA DEI PRODOTTI IN BASE SOTTO LA SOGLIA
//NON RICORSIVA IN CODA PER LA DECRESCENZA
//RICORSIVA IN CODA PER LA CRESCENZA
void listaProdotti(ProdottoPtr *prodotto, int soglia)
{
    if (*prodotto == NULL)
        return;
    else{
        listaProdotti(&((*prodotto)->prossimoProdotto), soglia);
        if((*prodotto)->quantitaProdotto < soglia)
            printf("%d, %s, %.2f, %d \n", (*prodotto)->codiceProdotto, (*prodotto)->nomeProdotto, (*prodotto)->prezzoProdotto, (*prodotto)->quantitaProdotto);
        
    }
}

void cambiaQuantitaProdotto(ProdottoPtr *prodotto, int codiceProdotto){
    if(*prodotto == NULL){ 
        printf("prodotto non trovato\n");
        return;
    }                           
    //SE TROVA IL CODICE CHIEDE LA NUOVA QUANTIA' E CAMBIA LA QUANTITA' DEL PRODOTTO
    else if((*prodotto)->codiceProdotto == codiceProdotto){
        int nuovaQuantitaProdotto;   
        printf("Inserisci la nuova quantita\': ");
        while((scanf("%d", &nuovaQuantitaProdotto))!= 1){
            printf("inserire una quantita\': ");
            while(getchar()!= '\n');
        }
        (*prodotto)->quantitaProdotto = nuovaQuantitaProdotto;
        printf("Dettaglio prodotto\n");         //STAMPA LE INFORMZIONI DEL PRODOTTO CON LA NUOVA QUANTITA'
        printf("%d\t%s\t%.2f\t%d\n", (*prodotto)->codiceProdotto, (*prodotto)->nomeProdotto, (*prodotto)->prezzoProdotto, (*prodotto)->quantitaProdotto);
    }
    else    //Se non trova passa all'elemento successivo
        cambiaQuantitaProdotto(&(*prodotto)->prossimoProdotto, codiceProdotto);
}

//VARIAZIONE PREZZO
//PROCEDURA SIMILE ALLA PRECEDENTE
void cambiaPrezzoProdotto(ProdottoPtr *prodotto, int codiceProdotto){
    if(*prodotto == NULL){
        printf("prodotto non trovato\n");
        return;
    }
    else if((*prodotto)->codiceProdotto == codiceProdotto){
        float nuovoPrezzoProdotto;
        printf("Inserisci il nuovo prezzo: ");
        while((scanf("%f", &nuovoPrezzoProdotto)) != 1){
            printf("inserire il nuovo prezzo prezzo\': ");
            while(getchar()!= '\n');
        }
        (*prodotto)->prezzoProdotto = nuovoPrezzoProdotto;
        printf("Dettaglio prodotto\n");
        printf("%d\t%s\t%.2f\t%d\n", (*prodotto)->codiceProdotto, (*prodotto)->nomeProdotto, (*prodotto)->prezzoProdotto, (*prodotto)->quantitaProdotto);

    }
    else
        cambiaPrezzoProdotto(&(*prodotto)->prossimoProdotto, codiceProdotto);
}


//INSERIMENTO NUOVO PRODOTTO
//SE IL CODICE E' GIA' PRESENTE ESCE
//ALTRIMENTI CONTINUA A CHIEDERE INFORMAZIONI NECESSARIE ALL'INSERIMENTO DEL PRODOTTO
void nuovoProdotto(ProdottoPtr *prodotto, int codiceProdotto){
    if(*prodotto != NULL){
        if((*prodotto)->codiceProdotto == codiceProdotto){
            printf("prodotto gia\' presente nella lista\n");
            printf("%d\t%s\t%.2f\t%d\n", (*prodotto)->codiceProdotto, (*prodotto)->nomeProdotto, (*prodotto)->prezzoProdotto, (*prodotto)->quantitaProdotto);
            return;
        }
        else if((*prodotto)->codiceProdotto > codiceProdotto){
            float prezzoProdotto;
            char nomeProdotto[10];

            printf("Inserisci il nome del prodotto(max 10 caratteri): \n");
            scanf("%10s", nomeProdotto);
            printf("Inserisci il prezzo del prodotto:\n");
            while((scanf("%f",&prezzoProdotto))!= 1){
                printf("Inserisci il prezzo del prodotto:\n");
                while(getchar() != '\n');
            }
            assegnaValoriProdotto(prodotto, codiceProdotto, prezzoProdotto, nomeProdotto);

            printf("prodotto inserito: ");
            printf("%d\t%s\t%.2f\t%d\n", (*prodotto)->codiceProdotto, (*prodotto)->nomeProdotto, (*prodotto)->prezzoProdotto, (*prodotto)->quantitaProdotto);
            
        }
        else
            nuovoProdotto(&(*prodotto)->prossimoProdotto, codiceProdotto);
    }
    else{
        ProdottoPtr nuovoPtr = malloc(sizeof(Prodotto));
        if(nuovoPtr == NULL)
            printf("memoria insuficiente");
        else{
            float prezzoProdotto;
            char nomeProdotto[10];

            printf("Inserisci il nome del prodotto(max 10 caratteri): \n");
            scanf("%10s", nomeProdotto);
            printf("Inserisci il prezzo del prodotto:\n");
            while((scanf("%f",&prezzoProdotto))!= 1){
                printf("Inserisci il prezzo del prodotto:\n");
                while(getchar() != '\n');
            }
            assegnaValoriProdotto(prodotto, codiceProdotto, prezzoProdotto, nomeProdotto);

            printf("prodotto inserito: ");
            printf("%d\t%s\t%.2f\t%d\n", (*prodotto)->codiceProdotto, (*prodotto)->nomeProdotto, (*prodotto)->prezzoProdotto, (*prodotto)->quantitaProdotto);
        }
        return;
    }
}

//CANCELLA UN PRODOTTO DALLA LISTA
void eliminaProdotto(ProdottoPtr *prodotto, int codiceProdotto){
    if(*prodotto == NULL){ 
        printf("prodotto inesistente\n");
        return;
    }
    //Se trova il prodotto lo elimina dalla lista
    else if((*prodotto)->codiceProdotto == codiceProdotto){
        printf("cancellazione di :");
        printf("%d\t%s\t%.2f\t%d\n", (*prodotto)->codiceProdotto, (*prodotto)->nomeProdotto, (*prodotto)->prezzoProdotto, (*prodotto)->quantitaProdotto);
        ProdottoPtr tempPtr = *prodotto;
        *prodotto = tempPtr->prossimoProdotto;
        free(tempPtr);
        printf("prodotto cancellato\n");
        return;
    }
    //PASSA ALL'ELEMENTO SUCCESSIVO
    else
        eliminaProdotto(&(*prodotto)->prossimoProdotto, codiceProdotto);
}

//RICERCA UN PRODOTTO NELLA LISTA
void cercaProdotto(ProdottoPtr *prodotto, int codiceProdotto)
{
    if(*prodotto == NULL)    //SE NON TROVA STAMPA LA FRASE
    {
        printf("prodotto inesistente\n");
        return;
    }
    //SE TROVA STAMPA I DETTAGLI DEL PRODOTTO
    else if((*prodotto)->codiceProdotto == codiceProdotto)
    {
        printf("Dettaglio prodotto\n");
        printf("%d\t%s\t%.2f\t%d\n", (*prodotto)->codiceProdotto, (*prodotto)->name, (*prodotto)->prezzoProdotto, (*prodotto)->quantitaProdotto);
        return;
    }
    //ALTRIMENTI PASSA ALL'ELEMENTO SUCCESSIVO
    else
        cercaProdotto(&(*prodotto)->prossimoProdotto, codiceProdotto);
}

//RITORNA IL NUMERO DI PRODOTTI DISPONIBILI
int numeroProdottiDisponibili(ProdottoPtr prodotto, int i)
{
    if(prodotto == NULL)
        return i;
    else
        numeroProdottiDisponibili(prodotto->prossimoProdotto, i+1);
}

//RITORNA IL NUMERO DI PRODOTTI SOTTO SOGLIA
int underThreshold(ProdottoPtr prodotto, int threshold, int i)
{
    if(prodotto == NULL)
        return i;
    else{
        if((prodotto)->quantitaProdotto < threshold)
            underThreshold(prodotto->prossimoProdotto, threshold, i+1);
        else
            underThreshold(prodotto->prossimoProdotto, threshold, i);
    }
}

//SCRIVE SUL FILE daOrdinare.bin I PRODOTTI DA ORDINARE
void order(ProdottoPtr prodotto, int threshold, FILE *toOrder){
    if(prodotto == NULL)
        return;
    else{
        if(prodotto->quantitaProdotto < threshold)
            fprintf(toOrder, "%d\t%s\t%.2f\t%d /t da ordinare %d\n", prodotto->codiceProdotto, prodotto->nomeProdotto,
                 prodotto->prezzoProdotto, prodotto->quantitaProdotto, (threshold - prodotto->quantitaProdotto + 1));
        else
            order(prodotto->prossimoProdotto, threshold, daOrdinare);
    }
}

void assegnaValoriProdotto(ProdottoPtr *prodotto, int codiceProdotto, float prezzoProdotto, char nomeProdotto[]){
    ProdottoPtr nuovoPtr = malloc(sizeof(Prodotto));
    if(nuovoPtr == NULL)
        printf("memoria insufficiente");
    else{
        nuovoPtr->codiceProdotto = codiceProdotto;
        nuovoPtr->prezzoProdotto = prezzoProdotto;
        strcpy(nuovoPtr->nomeProdotto, nomeProdotto);
        nuovoPtr->quantitaProdotto = rand() % 100001;
        nuovoPtr->prossimoProdotto = *prodotto;
        *prodotto = nuovoPtr;
    }
}