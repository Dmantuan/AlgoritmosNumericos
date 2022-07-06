#include <stdio.h>
#include <stdlib.h>
#include <time.h>

float* Gauss(float **A, int N);

void main(){
    FILE *file;

    // Leitura de "Arquivo.txt"
    if((file = fopen("Arquivo.txt","r")) == NULL){
        printf("Problemas para abrir o arquivo\n");
    }
    else{
        int sistemas, dimension;
        float precisao;
        char simetria;
        float **M;
        float *X;
        X = (float *) malloc(dimension*sizeof(int));
        M = (float **) malloc(dimension*sizeof(int));
        
        fscanf(file,"%d %d %f %c", &sistemas, &dimension, &precisao, &simetria);
        printf("sistemas:%d dimension:%d precisao:%f simetria:%c\n", sistemas, dimension, precisao, simetria);

        //Inicializacao da matriz como matriz extensa
        for (int i = 0; i < dimension; i++){
            M[i] = (float*)malloc((dimension+1)*sizeof(int*));
            for (int j = 0; j < dimension; j++){
                fscanf(file,"%f", &M[i][j]);
            }
        }
        for(int i=0;i<dimension;i++){
            fscanf(file,"%f",&M[i][dimension]);
        }

        //Confirmacao da leitura da matriz
        for (int i = 0; i < dimension; i++){
            for (int j = 0; j < dimension+1; j++){
                printf("%f ", M[i][j]);
            }
            printf("\n");
        }

        X = Gauss(M, dimension);

        //Impressao de X
        printf("\nResultado: " );
            for(int i=0; i<dimension; i++)
        printf("\nX%d = %f\n", i+1, X[i]);

        //Liberando a matriz
        for (int i = 0; i < dimension+1; i++){
            free(M[i]);
        }
        free(M);
        
    }
    
    fclose(file);
}

float* Gauss(float **A, int N){
    int k,i,j,soma;
    float M, *x;
    x=(float*)malloc(N*sizeof(float));

    for(k=0; k < N-1; k++){
        for(i=k+1; i<N; i++){
            M=A[i][k]/A[k][k];
            for(j=k; j<=N; j++){
                A[i][j]=A[i][j]-M*A[k][j];
            }
        }
    }
    x[N-1] = A[N-1][N]/A[N-1][N-1];

    //Test
    printf("\n");
    for(i = 0; i<N; i++){
        for(j = 0; j<=N; j++){
            printf("|%.3f\t", A[i][j]);
        }
        printf("\n");
    }

    for(i=N-2; i>=0; i--){
        soma = 0;
        for(j=i+1; j<N; j++){
            soma = soma + x[j]*A[i][j];
        }
        x[i]=(A[i][N]-soma)/A[i][i];
    }
    return x;
}