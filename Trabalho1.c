#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
/*
Grupo-
    Daniel Goncalves Mantuan
    Alexandre Melo Moulin Breda
    Vinicius de Mello Vieira
*/
void Gauss(float **A, int N, float V[N]);
void ImprimeM(int N,float M[N][N]);
void ImprimeX(int N, float X[N]);
void FatoracaoLU(float **A, int N, float V[N],float L[N][N], float U[N][N]);
void ResultadoLU(int N, float B[N], float L[N][N], float U[N][N]);
void Cholesky(float **A, int N, float V[N]);
void GaussSeidel(float **Matriz, int dimensao, float TermosIndepententes[dimensao], float precisao);
void GaussJacobi(float **Matriz, int dimensao, float TermosIndepententes[dimensao], float precisao);
float ValorMax(int dimensao, float Vetor[dimensao]);
void Identidade(int N, float M[N][N]);
void DuplicaM(float **Mo, int N, float M[N][N]);
void DuplicaV(int N, float V[N], float Va[N]);
void main(){
    FILE *file;
    // Leitura de "Arquivo.txt"
    if((file = fopen("Arquivo.txt","r")) == NULL){
        printf("Problemas para abrir o arquivo\n");
    }
    else{
        clock_t time;
        int sistemas, dimension;
        float precisao;
        char simetria;
        float **M;
        fscanf(file,"%d %d %f %c", &sistemas, &dimension, &precisao, &simetria);
        printf("\nsistemas:%d dimension:%d precisao:%f simetria:%c\n", sistemas, dimension, precisao, simetria);
        //Declaracao dos elementos dependentes
        M = (float **) malloc(dimension*sizeof(float*));
        float Mtime[5][sistemas];
        float V[dimension], L[dimension][dimension], U[dimension][dimension];
        //Inicializacao da matriz M
        for (int i = 0; i < dimension; i++){
            M[i] = (float*)malloc((dimension)*sizeof(float));
            for (int j = 0; j < dimension; j++){
                fscanf(file,"%f", &M[i][j]);
            }
        }
        //Para ler e executar todos os sistemas
        for(int k=0; k<sistemas; k++){
            //Inicializando o vertor V
            for(int i=0;i<dimension;i++){
                fscanf(file,"%f",&V[i]);
            }
            //Tecnicas de Sistemas Lineares
            printf("\n->Para o sistema linear %d temos:\n", k+1);
            printf("\nTecnica de Gauss-");
            time = clock();
            Gauss(M, dimension, V);
            time=clock()-time;
            Mtime[0][k]= ((float)time)/CLOCKS_PER_SEC;
            printf("\nTecnica de Fatoracao LU-");
            if(k==0){
                time=clock();
                FatoracaoLU(M, dimension, V, L, U);
                ResultadoLU(dimension,V ,L, U);
                time=clock()-time;
                Mtime[1][k]=((float)time)/CLOCKS_PER_SEC;
            }
            else{
                time=clock();
                ResultadoLU(dimension,V ,L, U);
                time=clock()-time;
                Mtime[1][k]=((float)time)/CLOCKS_PER_SEC;
            }
            printf("\nTecnica de Gauss Jacobi-");
            time=clock();
            GaussJacobi(M,dimension, V, precisao);
            time=clock()-time;
            Mtime[2][k]=((float)time)/CLOCKS_PER_SEC;
            printf("\nTecnica de Gauss Seidel-");
            time=clock();
            GaussSeidel(M,dimension,V,precisao);
            time=clock()-time;
            Mtime[3][k]=((float)time)/CLOCKS_PER_SEC;
            printf("\nTecnica de Cholesky-");
            if(simetria == 'S'||simetria=='s'){
                time=clock();
                Cholesky(M,dimension,V);
                time=clock()-time;
                Mtime[4][k]=((float)time)/CLOCKS_PER_SEC;
            }
            else{
                printf("\nA matriz informada nao e simetrica logo nao e possivel aplicar o metodo de Cholesky\n");
                Mtime[4][k]=-1;
            }
        }
        //Imprimir MMatriz de comparacao de tempos
        printf("\n\t#### Comparacao de tempo entre os metodos ####\n");
        for(int i=0; i<5; i++){
            switch (i){
                case 0:
                    printf("Elimin.Gauss: ");
                    break;
                case 1:
                    printf("Fatoracao LU: ");
                    break;
                case 2:
                    printf("Gauss-Jacobi: ");
                    break;
                case 3:
                    printf("Gauss-Seidel: ");
                    break;
                case 4:
                    printf("Deco.Cholesky:");
                    break;
            }
            for(int j=0; j<sistemas; j++){
                if(Mtime[i][j] != -1){
                    printf("%f  ",Mtime[i][j]);
                }
                else{
                    printf("--------  ");
                }
            }
            printf("\n");
        }
        printf("\n\n*Grupo-\nDaniel Goncalves Mantuan\nAlexandre Melo Moulin Breda\nVinicius de Mello Vieira\n");
        //Liberando a matriz M
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
float ValorMax(int dimensao, float Vetor[dimensao]){
    float temp[dimensao], aux;

    DuplicaV(dimensao, Vetor, temp);

    for(int i = 1; i < dimensao; i++){
        if(fabs(temp[0]) < fabs(temp[i])){
            aux = temp[i];
            temp[i] = temp[0];
            temp[0] = aux;
        }
    }
    return fabs(temp[0]);
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
Para fatoracao LU se faz:
    1-L.Y=B  -> Para achar Y
    2-U.X=Y  -> Para achar X
*/
void FatoracaoLU(float **A, int N, float V[N],float L[N][N],float U[N][N]){

    int k,i,j;
    float M;
    
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
void ResultadoLU(int N, float B[N], float L[N][N], float U[N][N]){
    int i,j;
    float Y[N];
    float X[N];
    float Ba[N];

    DuplicaV(N, B, Ba);

    //Substituicao sucessiva para L.Y=B
    for(i=0; i<N; i++){
        for(j=i-1; j>=0; j--){
            Ba[i] = Ba[i] - Y[j]*L[i][j];
        }
        Y[i]=Ba[i]/L[i][i];
    }

    //Substituicao sucessiva para U.X=Y
    for(i=N-1; i>=0; i--){
        for(j=i+1; j<N; j++){
            Y[i] = Y[i] - X[j]*U[i][j];
        }
        X[i]=Y[i]/U[i][i];
    }
    ImprimeX(N, X);
}
void Cholesky(float **A, int N, float V[N]){
    int i, j, k;
    float s, x[N], y[N];
    float L[N][N], LT[N][N], Va[N];
    DuplicaM(A, N, L);
    DuplicaV(N, V, Va);

    //zerar triangular superior
    for (i = 0; i < N; i++){
        for(j = 0; j < N; j++){
            if (j > i){
                L[i][j] = 0;
            }
        }
    }

    //metodo cholesky
    for (i = 0; i < N; i++){
        s = 0;

        if(i == 0){
            if (A[i][i] < 0){
                printf("Matriz não positiva definida\n");
                break;
            }

            L[i][i] = sqrt(A[i][i]);

            for (k = (i + 1); k < N; k++){
                L[k][i] = A[k][i] / L[i][i];
            }
        }
        else {
            for (k = 0; k <= (i - 1); k++){
                s = s + pow(L[i][k], 2);
            }
            s = A[i][i] - s;
                
            if (s < 0){
                printf("Matriz não positiva definida\n");
                break;
            }

            L[i][i] = sqrt(s);
            for(j = (i + 1); j < N; j++){
                s = 0;
                for (k = 0; k <= (j - 1); k++){
                    s = s + (L[k][i] * L[k][j]);
                }
                L[j][i] = (A[j][i] - s) / L[i][i];
            }
        }
    }

    //calculo da transposta de L
    for(i = 0; i < N; i++){
        for(j = 0; j < N; j++){
            LT[i][j] = L[j][i];
        }
    }

    //Achando o y da matriz normal
    for(i = 0; i <= (N - 1); i++){
        for(j = 0; j < i; j++){
            Va[i] = Va[i] - (y[j] * L[i][j]);
        }
        y[i] = Va[i] / L[i][i];
    }

    //Achando o x da matriz transposta
    for(i = (N - 1); i >= 0; i--){
        for(j = (i + 1); j < N; j++){
            y[i] = y[i] - (x[j] * LT[i][j]);
        }
        x[i] = y[i] / LT[i][i];
    }

    ImprimeX(N, x);
}
void GaussJacobi(float **Matriz, int dimensao, float TermosIndepententes[dimensao], float precisao){

    float Incognita[dimensao][dimensao],Ti[dimensao], G[dimensao], NextG[dimensao], DifNextGeG[dimensao];
    for(int i=0;i<dimensao;i++){
        for(int j=0;j<dimensao;j++){
            if(i==j){
                G[i]=TermosIndepententes[i]/Matriz[i][j];
                break;
            }
        }
    }

    for (int i = 0; i < dimensao; i++){
        NextG[i]=G[i];
    }

    DuplicaM(Matriz,dimensao,Incognita);
    DuplicaV(dimensao, TermosIndepententes,Ti );

    float D=1, AuxG, AuxG2[dimensao];

    while(D >= precisao){
        for(int i = 0; i < dimensao; i++){
            AuxG = G[i];
            for(int j = 0; j < dimensao; j++){
                if (i != j){
                    AuxG -= (Incognita[i][j]*AuxG2[j])/Incognita[i][i];
                }
            }
            NextG[i] = AuxG;
            DifNextGeG[i] = fabs(NextG[i] - AuxG2[i]);
        }

        D = (ValorMax(dimensao, DifNextGeG) / ValorMax(dimensao, NextG));
        for(int a = 0; a < dimensao; a++){
            AuxG2[a] = NextG[a];
        }
    }
    ImprimeX(dimensao,NextG);
}
void GaussSeidel(float **Matriz, int dimensao, float TermosIndepententes[dimensao], float precisao){
    float Incognita[dimensao][dimensao],Ti[dimensao], G[dimensao], NextG[dimensao], DifNextGeG[dimensao];

    for(int i=0;i<dimensao;i++){
        for(int j=0;j<dimensao;j++){
            if(i==j){
                G[i]=TermosIndepententes[i]/Matriz[i][j];
                break;
            }
        }
    }

    for (int i = 0; i < dimensao; i++){
        NextG[i]=G[i];
    }

    DuplicaM(Matriz,dimensao,Incognita);
    DuplicaV(dimensao, TermosIndepententes,Ti );

    double D=1, AuxG, OldG[dimensao];

    for (int i = 0; i < dimensao; i++){
        OldG[i]=G[i];
    }

    while(D >= precisao){
        for(int i = 0; i < dimensao; i++){
            AuxG = G[i];
            for(int j = 0; j < dimensao; j++){
                if (i != j){
                    AuxG -= (Incognita[i][j]*NextG[j])/Incognita[i][i];
                }
            }
            NextG[i] = AuxG;
            DifNextGeG[i] = fabs(NextG[i] - OldG[i]);
            OldG[i] = NextG[i];
        }

        D = (ValorMax(dimensao, DifNextGeG) / ValorMax(dimensao, NextG));
    }
    ImprimeX(dimensao,NextG);
}
