#include <stdio.h>
#include <stdlib.h>




int main(){
    char *nomeFile = malloc(256 * sizeof(char));
    int N, M, i, j;
    scanf("%s", nomeFile);
    scanf("%d", &N);
    scanf("%d", &M);

    int **matrice = malloc (N * sizeof(int));
    for( i = 0; i < N; i++){
        matrice[i] = malloc(M * sizeof(int));
    }

    FILE *file;
    file = fopen(nomeFile, "r");

    for(i = 0; i < N; i++){
      for(j = 0; j < M; j++){
       if (!fscanf(file, "%lf", &mat[i][j])) 
           break;
       printf("%lf\n",mat[i][j]); 
      }

  }


    fclose(file);
    return 0;
}