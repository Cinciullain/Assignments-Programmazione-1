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
void inizializzaListaDaFile(ProdottoPtr*, int, float, char[]);  
void cambiaQuantitaProdotto(ProdottoPtr*, int);      
void cambiaPrezzoProdotto(ProdottoPtr*, int);        
void nuovoProdotto(ProdottoPtr*, int);  //Inserisce un nuovo prodotto nella lista
void eliminaProdotto(ProdottoPtr*, int);    //Elimina un prodotto dalla lista          
void cercaProdotto(ProdottoPtr*, int);  //Cerca prodotto nella lista           
void stampaListaProdotti(ProdottoPtr*, int);               
int numeroProdottiDisponibili(ProdottoPtr, int);    //Restituisce il numero di prodotti disponibili         
int prodottiSottoSogliaMinima(ProdottoPtr, int, int);   //Restituisce il numero di prodotti sotto soglia
void ordinaProdottiSottoSoglia(ProdottoPtr, int soglia, FILE*);    //Scrive nel file i prodotti sotto soglia da ordinare
 
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
            inizializzaListaDaFile(&ProdottoPtr, codiceProdotto, prezzoProdotto, nomeProdotto); 
        }
        fclose(filePtr);
    }
    /*   FINE LETTURA      */

    printf("stampa tutti i prodotti\n");
    stampaLista(ProdottoPtr);

    /*   OPZIONI DELL'UTENTE   */
    int opzioneScelta = 0;                             
    int codiceProdotto = 0;                              
    int soglia = 10;    //soglia (default 10)
    void (*funzioniPtr[6])(ProdottoPtr*, int) = {cambiaQuantitaProdotto, cambiaPrezzoProdotto, nuovoProdotto, eliminaProdotto, cercaProdotto, stampaListaProdotti};    //array di puntatori a funzioni

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
            while(scanf("%d", &soglia)!= 1){
                printf("Inserisci la soglia:\n");
                while(getchar() != '\n');
            }
            (*funzioniPtr[opzioneScelta - 1])(&ProdottoPtr, soglia);
        }
    }while(opzioneScelta != 7);

    /*   UTENTE CHIEDE L'USCITA DAL PROGRAMMA      */
    int prodottiDisponibili = numeroProdottiDisponibili(ProdottoPtr, 0);
    int prodottiSottoSogliaMinimaNumber = prodottiSottoSogliaMinima(ProdottoPtr,soglia, 0);    //numero di prodotti sotto soglia
    printf("Il numero totale di prodotti disponibili in magazzino e\': %d\n", prodottiDisponibili); 
    printf("Il numero totale di prodotti sotto la soglia e\': %d\n", prodottiSottoSogliaMinimaNumber);

    /*   APERTURA DEL FILE numeroTotale.txt      */
    FILE *totalePtr;                        
    totalePtr = fopen("numeroTotale.txt", "w");
    if(totalePtr == NULL){                     
        printf("impossibile aprire il file.\n");
        exit(EXIT_FAILURE);
    }
    else{
        fprintf(totalePtr, "Il numero totale di prodotti disponibili in magazzino e\': %d\n", prodottiDisponibili);
        fprintf(totalePtr, "Il numero totale di prodotti sotto la soglia e\': %d\n", prodottiSottoSogliaMinimaNumber);
    }
    fclose(totalePtr);

    /*   APERTURA DEL FILE daOrdinare.bin      */
    FILE *daOrdinarePtr;                       
    daOrdinarePtr = fopen("daOrdinare.bin", "wb");
    if(daOrdinarePtr == NULL){
        printf("impossibile aprire il file.\n");
        exit(EXIT_FAILURE);
    }
    else
        ordinaProdottiSottoSoglia(ProdottoPtr, soglia, daOrdinarePtr);
    
    printf("Bye Bye\n");
    fclose(daOrdinarePtr);
}

//INSERIMENTO DEL PRODOTTO CON CODIDE CRESCENTE
void inizializzaListaDaFile(ProdottoPtr *prodotto, int codiceProdotto, float prezzoProdotto, char nomeProdotto[]){
    if(*prodotto == NULL)
        assegnaValoriProdotto(prodotto, codiceProdotto, prezzoProdotto, nomeProdotto);
    else if((*prodotto)->codiceProdotto > codiceProdotto)  
        assegnaValoriProdotto(prodotto, codiceProdotto, prezzoProdotto, nomeProdotto);
    else
        inizializzaListaDaFile(&((*prodotto)->prossimoProdotto),codiceProdotto, prezzoProdotto, nomeProdotto);
}

//STAMPA LA LISTA DEI PRODOTTI IN BASE SOTTO LA SOGLIA
//NON RICORSIVA IN CODA PER LA DECRESCENZA
//RICORSIVA IN CODA PER LA CRESCENZA
void stampaListaProdotti(ProdottoPtr *prodotto, int soglia)
{
    if (*prodotto == NULL)
        return;
    else{
        stampaListaProdotti(&((*prodotto)->prossimoProdotto), soglia);
        if((*prodotto)->quantitaProdotto < soglia)
            printf("%d, %s, %.2f, %d \n", (*prodotto)->codiceProdotto, (*prodotto)->nomeProdotto, (*prodotto)->prezzoProdotto, (*prodotto)->quantitaProdotto);
        
    }
}

void cambiaQuantitaProdotto(ProdottoPtr *prodotto, int codiceProdotto){
    if(*prodotto == NULL){ 
        printf("prodotto non trovato\n");
        return;
    }                           
    else if((*prodotto)->codiceProdotto == codiceProdotto){
        int nuovaQuantitaProdotto;   
        printf("Inserisci la nuova quantita\': ");
        while((scanf("%d", &nuovaQuantitaProdotto))!= 1){
            printf("inserire una quantita\': ");
            while(getchar()!= '\n');
        }
        (*prodotto)->quantitaProdotto = nuovaQuantitaProdotto;
        printf("Dettaglio prodotto\n");   
        printf("%d\t%s\t%.2f\t%d\n", (*prodotto)->codiceProdotto, (*prodotto)->nomeProdotto, (*prodotto)->prezzoProdotto, (*prodotto)->quantitaProdotto);
    }
    else    
        cambiaQuantitaProdotto(&(*prodotto)->prossimoProdotto, codiceProdotto);
}

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

//Inserimento nuovo prodotto nella lista; Se il codice gia' presente, esce
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

//Elimina un prodotto dalla lista
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
    else    
        eliminaProdotto(&(*prodotto)->prossimoProdotto, codiceProdotto);
}

void cercaProdotto(ProdottoPtr *prodotto, int codiceProdotto){
    if(*prodotto == NULL){
        printf("prodotto inesistente\n");
        return;
    }
    //Se trova il prodotto, stampa i dettagli
    else if((*prodotto)->codiceProdotto == codiceProdotto){ 
        printf("Dettaglio prodotto\n");
        printf("%d\t%s\t%.2f\t%d\n", (*prodotto)->codiceProdotto, (*prodotto)->nomeProdotto, (*prodotto)->prezzoProdotto, (*prodotto)->quantitaProdotto);
        return;
    }
    else    
        cercaProdotto(&(*prodotto)->prossimoProdotto, codiceProdotto);
}

//Restituisce il numero di prodotti disponibili
int numeroProdottiDisponibili(ProdottoPtr prodotto, int prodottiDisponibili)
{
    if(prodotto == NULL)
        return prodottiDisponibili;
    else
        numeroProdottiDisponibili(prodotto->prossimoProdotto, prodottiDisponibili++);
}

//Restituisce il numero di prodotti sotto soglia
int prodottiSottoSogliaMinima(ProdottoPtr prodotto, int soglia, int prodottiSottoSoglia)
{
    if(prodotto == NULL)
        return prodottiSottoSoglia;
    else{
        if((prodotto)->quantitaProdotto < soglia)
            prodottiSottoSogliaMinima(prodotto->prossimoProdotto, soglia, prodottiSottoSoglia++);
        else
            prodottiSottoSogliaMinima(prodotto->prossimoProdotto, soglia, prodottiSottoSoglia);
    }
}

//Scrive sul file daOrdinare.bin i prodotti da ordinare
void ordinaProdottiSottoSoglia(ProdottoPtr prodotto, int soglia, FILE *daOrdinare){
    if(prodotto == NULL)
        return;
    else{
        if(prodotto->quantitaProdotto < soglia)
            fprintf(daOrdinare, "%d\t%s\t%.2f\t%d /t da ordinare %d\n", prodotto->codiceProdotto, prodotto->nomeProdotto,
                 prodotto->prezzoProdotto, prodotto->quantitaProdotto, (soglia - prodotto->quantitaProdotto + 1));
        else    
            ordinaProdottiSottoSoglia(prodotto->prossimoProdotto, soglia, daOrdinare);
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