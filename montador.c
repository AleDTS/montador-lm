#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct{
    int valor;
    char rot[10];
    char mn[4];
    char ope1[4];
    char ope2[4];
    char coment[30];
} LINHA;

LINHA linha[50];

int rotulos(FILE *cod){
    int max=20,l=0,j=0,k=0;
    int temEQU=0, linhas=0;
    char *aux;
    char quebra[]=" \n",lin[max],valor[4];

    while(!feof(cod))    {
        fgets(lin,max,cod); //LE LINHA
        aux=strtok(lin,quebra); //QUEBRA LINHA quando encontrar quebra[]
        temEQU=0;

        while(aux!= NULL){ //PROCURA EQU
            if(!strcmp(aux,"EQU")){
                strcpy(linha[j].rot,lin);
                aux=strtok(NULL,quebra);
                linha[j].valor=atoi(aux);
                strcpy(linha[k+j].mn,"xxx");
                strcpy(linha[k+j].ope1,"xxx");
                strcpy(linha[k+j].ope2,"xxx");
                temEQU=1;
                j++;
                linhas++;
            }
            aux=strtok(NULL,quebra);
        }

        if(!temEQU){ //SE NAO TEM EQU, PROCURA ROTULO
            aux=strtok(lin,"\n");
            while(aux!= NULL){
                if(!strcmp(aux,"MOV")) break;
                else if(!strcmp(aux,"ADD")) break;
                else if(!strcmp(aux,"SUB")) break;
                else if(!strcmp(aux,"CMP")) break;
                else if(!strcmp(aux,"JMP")) break;
                else if(!strcmp(aux,"JC")) break;
                else if(!strcmp(aux,"JNC")) break;
                else if(!strcmp(aux,"JZ")) break;
                else if(!strcmp(aux,"JNZ")) break;
                else if(!strcmp(aux,"JBE")) break;
                else if(!strcmp(aux,"JA")) break;
                else if(!strcmp(aux,"CALL")) break;
                else if(!strcmp(aux,"RET")) break;
                else if(!strcmp(aux,"HLT")) break;
                else{ //ROTULO
                    strcpy(linha[k+j].rot,aux);
                    strcpy(linha[k+j].mn,"xxx");
                    strcpy(linha[k+j].ope1,"xxx");
                    strcpy(linha[k+j].ope2,"xxx");
                    linha[k+j].valor=l;
                    k++;
                    linhas++;
                }
                aux=strtok(NULL,"\n");
            }
            l+=2;
        }
    }
    return (linhas);
}

void leCodigo(FILE *cod,FILE *ling){
    int max=30,l=0;
    char lin[max], *aux, quebra[]=" \n", mne[]="],\n;";

    while(!feof(cod))    {
        fgets(lin,max,cod);
        aux=strtok(lin,quebra);
        while(aux!= NULL)        {
            //printf("%s\n",aux);
            if(!strcmp(lin,"MOV"))            {
                strcpy(linha[l].mn,lin);
                aux=strtok(lin,mne);
                printf("%s\n",aux);
            }


            aux=strtok(NULL,quebra);
        }
        printf("\n");
        l++;
    }
}

int main(){
    int i,linhas;
    char lin[20];
    FILE *ASM, *OPC;
    
    //commit

    if ((ASM = fopen("teste2.asm","rt")) == NULL)    {
        printf("\nErro ao abrir o arquivo original.\n\n");
        exit(1);
    }

    if ((OPC = fopen("teste1.opc","w+t")) == NULL)    {
        printf("\nErro ao abrir o arquivo original.\n\n");
        exit(1);
    }

    linhas=rotulos(ASM);

    printf("VALOR\tROTULO\tMNE\tOP 1\tOP 2\t\tCOMENTARIO\n\n");
    for(i=0; i<linhas; i++)
        printf("[%d]\t[%s]\t[%s\t[%s\t[, %s]]]\t[;%s]\n",
        linha[i].valor, linha[i].rot, linha[i].mn,
        linha[i].ope1, linha[i].ope2, linha[i].coment);

    fclose(ASM);
    fclose(OPC);

}
