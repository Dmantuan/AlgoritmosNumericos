#include <stdio.h>
#include <stdlib.h>
#include <time.h>

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
        Cuidado pois os números negativos estão escritos como strings nos arquivos
        Ainda nao sei como resolver e se tem que resolver

        Falta: 
                Leitura da ultima linha do arquivo
        */
       
        //inicializacao e implementacao da matriz 
        float M[dimension][dimension];
        char numeros[10];
        for (int i = 0; i < dimension; i++){
            for (int j = 0; j < dimension; j++){
                fscanf(file,"%f", &M[i][j]);
            }
        }

        //Confirmacao da leitura da matriz
        for (int i = 0; i < dimension; i++){
            for (int j = 0; j < dimension; j++){
                printf("M[%d][%d]= %f\n",i, j, M[i][j]);
            }
        }
    }

    fclose(file);
}