#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define GRANDEZZA_INPUT 100

//Dichiaro una funzione per ogni stato dell'automa.
void stato0(char carattere);
void stato1(char carattere);
void stato2(char carattere);
void stato3(char carattere);
void stato4(char carattere);
void stato5(char carattere);
void stato6(char carattere);
void stato7(char carattere);
void stato8(char carattere);
void statoabDD0(char carattere);
void statoabDD1(char carattere);
void statoabDD2(char carattere);
void statoabDD3(char carattere);

void secondaVerifica(char *stringa);
void finale();


unsigned short int stato = 0;
unsigned short int stato_abDD = 0;

int main(){
    char stringaInput[GRANDEZZA_INPUT];
    printf("Stringa:");

	while(scanf("%s", stringaInput) != 1){
		printf("Input invalido, inserire un nuovo input.");
		while(getchar() != '\n');
	}

    for(unsigned int i = 0; i <= strlen(stringaInput); i++){
        switch(stato){
            case 0:
                stato0(stringaInput[i]);
                break;
            case 1:
                stato1(stringaInput[i]);
                break;
            case 2:
                stato2(stringaInput[i]);
                break;
            case 3:
                stato3(stringaInput[i]);
                break;
            case 4:
                stato4(stringaInput[i]);
                break;
            case 5:
                stato5(stringaInput[i]);
                break; 
            case 6:
                stato6(stringaInput[i]);
                break;
            case 7:
                stato7(stringaInput[i]);
                break;
            case 8:
                stato8(stringaInput[i]);
                break;
            case 9:
                secondaVerifica(stringaInput);
        }
    }

    printf("stringa non appartenente al linguaggio\n");
    printf("sottostringa abDD presente\n");

    return 0;
}

void stato0(char carattere){
	if(carattere >= 'A' && carattere <= 'Z')
		stato = 1;
}


void stato1(char carattere){
	if(carattere >= 'A' && carattere <= 'Z')
		stato = 2;
	else
		stato = 0;
}


void stato2(char carattere){
	if(carattere >= 'A' && carattere <= 'Z')
		stato = 3;
	else
		stato = 0;
}

void stato3(char carattere){
	if(carattere >= 'A' && carattere <= 'Z')
		stato = 4;
	else
		stato = 0;
}

void stato4(char carattere){
	if(carattere == 'a')
		stato = 5;
	else if(carattere >= 'A' && carattere <= 'Z')
		stato = 1;
	else
		stato = 0;
}

void stato5(char carattere){
	if(carattere == 'b')
		stato = 6;
	else if(carattere >= 'A' && carattere <= 'Z')
		stato = 1;
	else
		stato = 0;
}

void stato6(char carattere){
	if(carattere != 'd')
		stato = 7;
	else if(carattere == 'd')
		stato = 0;
}

void stato7(char carattere){
	if(carattere != 'd')
		stato = 8;
	else
		stato = 0;
}

void stato8(char carattere){
	if(carattere != 'd')
		stato = 9;
	else
		stato = 0;
}

void statoabDD0(char carattere){
	if(carattere == 'a')
		stato_abDD = 1;
}

void statoabDD1(char carattere){
	if(carattere == 'b')
		stato_abDD = 2;
	else if(carattere == 'a')
		stato_abDD = 1;
	else
		stato_abDD = 0;
}

void statoabDD2(char carattere){
	if(carattere == 'D')
		stato_abDD = 3;
	else if(carattere == 'a')
		stato_abDD = 1;
	else
		stato_abDD = 0;
}

void statoabDD3(char carattere){
	if(carattere == 'D')
		stato_abDD = 4;
	else if(carattere == 'a')
		stato_abDD = 1;
	else
	    stato_abDD = 0;
}

void secondaVerifica(char *stringa){
	for(unsigned int i = 0; i <= strlen(stringa); i++){
		switch(stato_abDD){
			case 0:
				statoabDD0(stringa[i]);
				break;
			case 1:
				statoabDD1(stringa[i]);
				break;
			case 2:
				statoabDD2(stringa[i]);
				break;
			case 3:
				statoabDD3(stringa[i]);
				break;
			case 4:
				finale();
		}
	}
	printf("stringa appartenente al linguaggio\n");
	printf("nessuna sottostringa abDD\n");
	exit(0);
}

void finale(){
	printf("stringa appartenente al linguaggio\nsottostringa abDD presente");
	exit(0);
}