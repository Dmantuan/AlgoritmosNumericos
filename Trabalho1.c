#include <stdio.h>
#include <stdlib.h>
#include <time.h>

float* Gauss(float **A, float *B, int N);

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
        fscanf(file,"%d %d %f %c", &sistemas, &dimension, &precisao, &simetria);
        printf("sistemas:%d dimension:%d precisao:%f simetria:%c\n", sistemas, dimension, precisao, simetria);

        /*
        Falta: 
                Leitura da ultima linha do arquivo
        */
       
        //inicializacao e implementacao da matriz 
        float M[dimension+1][dimension+1];
        char numeros[10];
        for (int i = 0; i < dimension; i++){
            for (int j = 0; j < dimension; j++){
                fscanf(file,"%f", &M[i][j]);
            }
        }
        for (int i = 0; i < dimension; i++){
            fscanf(file,"%f", &M[i][dimension]);
        }
        

        //Confirmacao da leitura da matriz
        for (int i = 0; i < dimension; i++){
            for (int j = 0; j < dimension+1; j++){
                printf("M[%d][%d]= %f\n",i, j, M[i][j]);
            }
        }
    }

    fclose(file);
}

/*
    Em Gauss o codigo nao esta usando a matriz ampliada para fazer as operacaoes, acredito que B signifique que e o vetor de resultados
        Quando voltar resolver isso
    Lembrar que a saida e um vetor do tamanho do numero de linhas 
*/

float* Gauss(float **A, float *B, int N){
    int k,i,j;
    float M, *x;
    x=(float*)malloc(N*sizeof(float));
    for(k=0; k < N-1; k++){
        for(i=k+1; i<N; i++){
            M=A[i][j]/A[k][k];
            for(j=k; j<N; j++){
                A[i][k]=A[i][j]-M*A[k][j];
            }
            B[i]=B[i]-M*B[k];
        }
    }
    for(i=N-1; i>=0; i--){
        for(j=i+1; j<N; j++){
            B[i] = B[i]-x[j]*A[i][j];
        }
        x[i]=B[i]/A[i][i];
    }
    return x;
}