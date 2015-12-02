#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define EQU 0
#define ROT 1

typedef struct{
    int valor;
    char rot[10];
    char mn[4];
    char ope1[4];
    char ope2[4];
} LINHA;

LINHA linha[100];

void copy_string(char *target, char *source) {
   while (*source) {
      *target = *source;
      source++;
      target++;
   }
   *target = '\0';
}

int Rotulos(FILE *cod){
    int max=20,l=0,j=0,k=0;
    int temEQU=0, linhas=0;
    char *aux;
    char quebra[]=" \n\t;[],",lin[max],valor[4];

    while(!feof(cod))    {
        fgets(lin,max,cod); //LE LINHA

        aux=strtok(lin,quebra); //QUEBRA LINHA quando encontrar quebra[]
        temEQU=0;

        while(aux!= NULL){ //PROCURA EQU
            if(!strcmp(aux,"EQU")){
                strcpy(linha[j].rot,lin);
                aux=strtok(NULL,quebra);
                printf("\n%s\n",aux);
                linha[j].valor=atoi(aux);
                strcpy(linha[j].mn,"xxx");
                strcpy(linha[j].ope1,"xxx");
                strcpy(linha[j].ope2,"xxx");
                temEQU=1;
                j++;
                linhas++;
            }

            aux=strtok(NULL,quebra);


        }

        if(!temEQU){ //SE NAO TEM EQU, PROCURA ROTULO
            aux=strtok(lin,quebra);
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
                else if(!strcmp(aux,"INC")) break;
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

int rotulos(FILE *cod){
    int max=100,l=0,j=0,k=0;
    int com=0;
    int temEQU=0, bytes=0,rots=0,temROT;
    char *aux;
    char quebra[]=" [],\n;",lin[max],linAux[max];

    char mnemonicos[]="/MOV/ADD/SUB/CMP/JMP/JC/JNC/JZ/JNZ/JBE/JA/CALL/RET/HLT/INC/";

    //if(strstr(comandos,"/");
    //printf("%s\n",aux);
    while(!feof(cod))    {
        fgets(lin,max,cod); //LE LINHA

        aux=strtok(lin,quebra); //QUEBRA LINHA quando encontrar quebra[]
        //com=0;
        while(aux!=NULL){
            //com++;
            //

            if(!strcmp("EQU",aux)){
                    aux=strtok(NULL," ");
                    //printf("%s\n",aux);
                    linha[l].valor=atoi(aux);
                    break;
            }



            else if (strstr(mnemonicos,aux)==NULL){
                strcpy(linha[l].rot,aux);
                //break;
            }

            else if (strstr(mnemonicos,aux)!=NULL){
                strcpy(linha[l].mn,aux);

                aux=strtok(NULL,quebra);
                printf("%s\n",aux);


                break;
            }
            //printf("%s\n",aux);


            aux=strtok(NULL,quebra); //quebra por partes ate o final da linha

        }

        //printf("%d - %s\n",com,lin);
        //if (temROT) rots++;
        l++;
    }
    return (l);
}



int main(){
    int i,linhas;
    char lin[20];
    char nome[20];
    FILE *ASM, *OPC;

    gets(nome);

    if ((ASM = fopen(nome,"rt")) == NULL)    {
        printf("\nErro ao abrir o arquivo original.\n\n");
        exit(1);
    }

    if ((OPC = fopen("teste1.opc","w+t")) == NULL)    {
        printf("\nErro ao abrir o arquivo original.\n\n");
        exit(1);
    }

    printf("\n%d\n",linhas=rotulos(ASM));

    printf("\nVALOR\tROTULO\t\tMNE\tOP 1\tOP 2\n");
    for(i=0; i<linhas; i++)
    //for(i=0; i<linhas&&strcmp(linha[i].rot,"\0"); i++)
        printf("[%d]\t[%s\t]\t[%s\t[%s\t[, %s]]]\n",
        linha[i].valor, linha[i].rot, linha[i].mn,
        linha[i].ope1, linha[i].ope2);

    fclose(ASM);
    fclose(OPC);

}
