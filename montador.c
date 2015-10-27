#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct
{
    char rot[10];
    char mn[4];
    int ope1,ope2;
    char coment[30];
} LINHA;

typedef struct
{
    char equ[10];
    int valor;
} EQU;

typedef struct
{
    char rot[10];
    int lin;
} ROTULO;

EQU eq[10];

ROTULO rotulo[10];

LINHA linha[50];

void printaCodigo(FILE *cod, int n)
{
    char texto[n+1];
    while(fgets(texto,n,cod))
    {
        printf("%s",texto);

    }
}

void converte(char aux[], FILE *ling,int lin)
{
    int i=0;
    char rot[10];

}

int rotulos(FILE *cod)
{
    int max=20,l=0,j=0,k=0;
    int temEQU=0;
    char *aux;
    char quebra[]=" \n",lin[max],valor[4];
    long int pos=0;

    while(!feof(cod))
    {
        fgets(lin,max,cod);
        aux=strtok(lin,quebra);
        temEQU=0;
        while(aux!= NULL)
        {
            if(!strcmp(aux,"EQU"))
            {
                strcpy(eq[j].equ,lin);
                aux=strtok(NULL,quebra);
                eq[j].valor=atoi(aux);
                temEQU=1;
                j++;
            }
            //printf("%s\n",aux);
            aux=strtok(NULL,quebra);
        }
        if(!temEQU)
        {
            l+=2;
            aux=strtok(lin,"\n");
            while(aux!= NULL)
            {
                //
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
                else
                {
                    //printf("%d %d %s\n",j,l,aux);
                    strcpy(rotulo[k].rot,aux);
                    rotulo[k].lin=l;
                    k++;
                }
                aux=strtok(NULL,"\n");
            }
        }
    }
}

void leCodigo(FILE *cod,FILE *ling)
{
    int max=30,l=0;
    char lin[max], *aux, quebra[]=" \n", mne[]="],\n;";

    while(!feof(cod))
    {
        fgets(lin,max,cod);
        aux=strtok(lin,quebra);
        while(aux!= NULL)
        {
            //printf("%s\n",aux);
            if(!strcmp(lin,"MOV"))
            {
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

int main()
{
    int i;
    char lin[20];
    FILE *ASM, *OPC;

    if ((ASM = fopen("teste2.asm","rt")) == NULL)
    {
        printf("\nErro ao abrir o arquivo original.\n\n");
        exit(1);
    }

    if ((OPC = fopen("teste1.opc","w+t")) == NULL)
    {
        printf("\nErro ao abrir o arquivo original.\n\n");
        exit(1);
    }

    rotulos(ASM);

    //fgets(lin,21,ASM);
    //printf("%s\n",lin);

    //fseek(ASM,0,SEEK_SET);

    //leCodigo(ASM,OPC);

    for(i=0; i<5; i++)
        printf("%s %d\n",eq[i].equ,eq[i].valor);
    for(i=0; i<5; i++)
        printf("%s %d\n",rotulo[i].rot,rotulo[i].lin);
    //printf("%d [%s]\t[%s [%s[, %s]]][;%s]\n",i, linha[i].rot, linha[i].mn, linha[i].ope1, linha[i].ope2, linha[i].coment);


    fclose(ASM);
    fclose(OPC);

}
