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
void insertOccorrenze(occorrenzePtr*, int numeroOccorrenze, int posizione1, int posizione2, int valore, int colonna);   //INSERISCE NELLA LISTA OCCORRENZE 
void stampaOccorrenze(occorrenzePtr);                                                                       //STAMPA I RISULTATI 
void assegnaValori(occorrenzePtr*, int numeroOccorrenze, int posizione1, int posizione2, int valore, int colonna);      //ASSEGNA I DATI NECESSARI PER L'OUTPUT

int main(int argc, const char *argv[]){
    FILE *filePtr;             
    if((filePtr  = fopen(argv[1], "r")) == NULL){
        printf("Impossibile aprire il file %s.", argv[1]);
        exit(EXIT_FAILURE);
    }

    int cont = 0;           //contatore per il numero di elementi
    int temp = 0;           //variabile temporanea per salvare l'elemento letto
    int dimensioneMatrice = 0;      
    int i,j;                //contatori

    /*       CONTA QUANTI ELEMENTI SONO PRESENTI NEL FILE E NE CALCOLA LA RADICE    */
    while(fscanf(filePtr,"%d", &temp) == 1){
        cont++;
    }
    rewind(filePtr);

    //Se il file non ha radice perfetta o < 4 il programma termina
    dimensioneMatrice = radice(cont, 1); 
    if(dimensioneMatrice < 4){
        printf("File non valido.\n");
        exit(EXIT_FAILURE);
    }

    /*       LETTURA FILE E INSERIMENTO DATI NELLA MATRICE      */

    int **matriceInput = (int**)calloc(dimensioneMatrice, sizeof(int*));
    for(i = 0; i < dimensioneMatrice; i++)
    {
        matriceInput[i] = (int*)calloc(dimensioneMatrice, sizeof(int));
        for(j = 0; j < dimensioneMatrice; j++)
        {
            fscanf(filePtr,"%d", &matriceInput[i][j]);
            printf("%d ", matriceInput[i][j]);
        }
        printf("\n");
    }
    fclose(filePtr); 

    /*       FINE LETTURA       */
  
    
    printf("Inserire M: \n");
    int m = inputNumericoPositivo();       //LETTURA M
    while(m > dimensioneMatrice)              //SE MAGGIORE DELLA dimensioneMatriceENSIONE RILEGGE UNO NUOVO
    {
        printf("M deve essere minore di N");
        m = inputNumericoPositivo();
    }


    /*       RICERCA OCCORRENZE     */
    /*       PRIMA IN COLONNA E POI IN RIGA     */

    int valoreTemporaneoColonna = 0, valoreTemporaneoRiga = 0;    //Per contenere il valore della sequenza
    int riga, colonna;  //Indici
    int posizioneColonna = 0, posizioneRiga = 0;    //Dove Inizia l'occorrenza
    int contatoreOccorrenzeColonna, contatoreOccorrenzeRiga;                  
    Occorrenze *occPtr = NULL;    

    //tengo un indice che individua la posizione iniziale laddove inizia l'occorrenza
    //una volta trovato un numero diverso dalla posizione iniziale verifico se la consecutivita' supera l'M
    //se lo supera mette le informazioni necessari nella lista
    //altrimenti resetta l'indice alla posizione riga (usando il contatore)

    for(colonna = 0; colonna < dimensioneMatrice; colonna++){
        //A ogni iterazione resetto i contatori
        contatoreOccorrenzeColonna = 1;    
        contatoreOccorrenzeRiga = 1;

        for(riga = 1, posizioneColonna = 0, posizioneRiga = 0; riga < dimensioneMatrice; riga++){
            //Ricerca in colonna
            if(matriceInput[riga][colonna] == matriceInput[posizioneColonna][colonna]){
                contatoreOccorrenzeColonna += 1;
                valoreTemporaneoColonna = matriceInput[posizioneColonna][colonna];
                if((riga == (dimensioneMatrice - 1)) && (contatoreOccorrenzeColonna >= m)){   //SE L'INDICE ARRIVA ALL'ULTIMO ELEMENTO, contatoreOccorrenzeRigaOLLA SE IL CONTATORE SUPERA M, SE SI LO METTE NELLA LISTA, ALTRIMENTI NULLA
                    insertOccorrenze(&occPtr,contatoreOccorrenzeColonna, colonna, posizioneColonna, valoreTemporaneoColonna, 1);
                    contatoreOccorrenzeColonna = 1;
                }
            }
            else{
                if(contatoreOccorrenzeColonna >= m)
                    insertOccorrenze(&occPtr,contatoreOccorrenzeColonna, colonna, posizioneColonna, valoreTemporaneoColonna, 1);

                posizioneColonna = riga;
                contatoreOccorrenzeColonna = 1;
            }

            //Ricerca in riga, cambiano solo gli indici
            if(matriceInput[colonna][riga] == matriceInput[c][posizioneRiga]){
                contatoreOccorrenzeRiga += 1;
                valoreTemporaneoRiga = matriceInput[c][posizioneRiga];
                if((riga == (dimensioneMatrice - 1)) && (contatoreOccorrenzeRiga >= m)){
                    insertOccorrenze(&occPtr, contatoreOccorrenzeRiga, colonna, posizioneRiga, valoreTemporaneoRiga, 0);
                    contatoreOccorrenzeRiga = 1;
                }
            }
            else{
                if(contatoreOccorrenzeRiga >= m)
                    insertOccorrenze(&occPtr, contatoreOccorrenzeRiga, colonna, posizioneRiga, valoreTemporaneoRiga, 0);
                
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

//TANTI CASI DA ContrOLLARE

//SE LA LISTA VUOTA INSERISCE UN ELEMENTO
//ALTRIMENTI ContrOLLA SE IL NUMERO DI OCCORRENZE CHE STA LEGGENDO E' MINORE O MAGGIORE O UGUALE
    //SE MINORE LO INSERISCE NELLA LISTA
    //SE MAGGIORE VA AL PROSSIMO ELEMENTO E RICOMINCIA I ContrOLLI
    //SE UGUALE VA A ContrOLLARE SE L'ELEMENTO ENTRANTE E' DELLO STESSO TIPO DI QUELLO PRESENTE
        //SE L'ELEMENTO ENTRATE SI TRATTA DI UNA COLONNA E L'ELEMENTO PRESENTE NON LO E', VIENE INSERITO
        //SE SONO DELLO STESSO TIPO ControLLA LA POSIZIONE DI CHI INIZIA PRIMA
            //SE L'ELEMENTO ENTRATE INIZIA PRIMA VIENE INSERITO
            //ALTRIMENTI VA AL PROSSIMO ELEMENTO DELLA LISTA
        //SE L'ELEMENTO ENTRANTE SI TRATTA DI UNA RIGA E L'ELEMENTO PRESENTE NON LO E', VA AL PROSSIMO ELEMENTO DELLA LISTA
void insertOccorrenze(occorrenzePtr *oPtr, int numeroOccorrenze, int posizione1, int posizione2, int valore, int colonna){
    if(*oPtr == NULL)
        assegnaValori(oPtr, numeroOccorrenze, posizione1, posizione2, valore, colonna);

    else if((*oPtr)->numeroOccorrenza < numeroOccorrenze)
        assegnaValori(oPtr, numeroOccorrenze, posizione1, posizione2, valore, colonna);
    
    else if((*oPtr)->numeroOccorrenza == numeroOccorrenze){
        if((colonna > (*oPtr)->colonna))
            assegnaValori(oPtr, numeroOccorrenze, posizione1, posizione2, valore, colonna);

        else if(colonna == (*oPtr)->colonna){
            if(posizione1 > (*oPtr)->posizione1)
                assegnaValori(oPtr, numeroOccorrenze, posizione1, posizione2, valore, colonna);
            else
                insertOccorrenze((&(*oPtr)->prossimaOcc), numeroOccorrenze, posizione1, posizione2, valore, colonna);
        }
        else
            insertOccorrenze((&(*oPtr)->prossimaOcc), numeroOccorrenze, posizione1, posizione2, valore, colonna);
    }
    else
        insertOccorrenze((&(*oPtr)->prossimaOcc), numeroOccorrenze, posizione1, posizione2, valore, colonna);
}

//Inserisce i valore in lista
void assegnaValori(occorrenzePtr *oPtr, int numeroOccorrenze, int posizione1, int posizione2, int valore, int colonna){
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