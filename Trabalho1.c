#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void Gauss(float **A, int N, float V[N]);
void ImprimeM(int N,float M[N][N]);
void ImprimeX(int N, float X[N]);
void FatoracaoLU(float **A, int N, float V[N]);
void DuplicaM(float **Mo, int N, float M[N][N]);
void DuplicaV(int N, float V[N], float Va[N]);

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
        M = (float **) malloc(dimension*sizeof(float));
        
        fscanf(file,"%d %d %f %c", &sistemas, &dimension, &precisao, &simetria);
        printf("sistemas:%d dimension:%d precisao:%f simetria:%c\n", sistemas, dimension, precisao, simetria);

        float V[dimension];

        //Inicializacao da matriz como matriz extensa
        for (int i = 0; i < dimension; i++){
            M[i] = (float*)malloc((dimension)*sizeof(float*));
            for (int j = 0; j < dimension; j++){
                fscanf(file,"%f", &M[i][j]);
            }
        }
        for(int i=0;i<dimension;i++){
            fscanf(file,"%f",&V[i]);
        }

        //Tecnicas de Sistemas Lineares
        printf("Tecnica de Gauss-");
        Gauss(M, dimension, V);
        //printf("Tecnica de Fatoracao LU-");
        //FatoracaoLU(M, dimension, V);
       
        //Liberando a matriz
        for (int i = 0; i < dimension; i++){
            free(M[i]);
        }
        free(M);
        
    }
    
    fclose(file);
}

void ImprimeM(int N, float M[N][N]){
    printf("\nMatriz: \n");
    for (int i = 0; i < N; i++){
            for (int j = 0; j < N; j++){
                printf("%f ", M[i][j]);
            }
            printf("\n");
    }
}

void ImprimeX(int N, float X[N]){
    printf("X:\n");
    for (int i = 0; i < N; i++){
        printf("|%f|\n",X[i]);
    }
}

void DuplicaM(float **Mo, int N, float Mc[N][N]){
    for (int i = 0; i < N; i++){
        for (int j = 0; j < N; j++){
            Mc[i][j] = Mo[i][j];
        }
    }
}

void DuplicaV(int N, float V[N], float Va[N]){
    for (int i = 0; i < N; i++){
        Va[i] = V[i];
    }
}

void Identidade(int N, float M[N][N]){
    for(int i=0; i<N; i++){
        for (int j = 0; j < N; j++){
            if(i==j){
                M[i][j]=1;
            }
            else{
                M[i][j]=0;
            }
        }
    }
}

void Gauss(float **A, int N, float V[N]){
    int k,i,j;
    float M, x[N], Mc[N][N],soma, Va[N];

    DuplicaM(A,N,Mc);
    DuplicaV(N, V, Va);

    //Matriz triangular superior
    for(k=0; k < N-1; k++){
        for(i=k+1; i<N; i++){
            M=Mc[i][k]/Mc[k][k];
            for(j=k; j<N; j++){   //Aplica o fator M em toda linha
                Mc[i][j]=Mc[i][j]-M*Mc[k][j];
            }
            Va[i]=Va[i]-M*Va[k];
        }
    }

    //Substituicao sucessiva
    for(i=N-1; i>=0; i--){
        for(j=i+1; j<N; j++){
            Va[i] = Va[i] - x[j]*Mc[i][j];
        }
        x[i]=Va[i]/Mc[i][i];
    }
    
    ImprimeX(N,x);
}

/*
Para fatoracao LU primeiro se faz:
    1-L.Y=B  -> Para achar Y
    2-U.X=Y  -> Para achar X
*/
void FatoracaoLU(float **A, int N, float V[N]){
    int k,i,j;
    float M, U[N][N], L[N][N];
    
    DuplicaM(A,N,U);
    Identidade(N, L);

    //Saido dese loop temos L e U
    for(k=0; k < N-1; k++){     //Refere-se a etapa
        for(i=k+1; i<N; i++){   //Refere-se a linha
            M=U[i][k]/U[k][k];  //Esse e o fator de divisao da linha
            L[i][k]=M;
            for(j=k; j<N; j++){   //Aplica o fator M em toda linha passa de coluna a coluna
                U[i][j]=U[i][j]-M*U[k][j];  //A matriz U ja esta pronta
            }
        }
    }
    
}