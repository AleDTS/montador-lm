#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define EQU 0
#define ROT 1

typedef struct{
    int valor;
    int temRot;
    char rot[10];
    char mn[4];
    char ope[2][4];
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

int temNaString(char ch, char str[]){
    int i;
    for(i=0;i<strlen(str);i++)
        if (str[i]==ch) return 1;
    return 0;
}

int rotulos(FILE *cod){
    int max=100,l=0,j=0,k=0,i=0;
    int com=0;
    int linhaComentada,temQuebra, temB;
    int temEQU=0, bytes=0,rots=0, totalBytes=0;
    char *aux;
    char quebra[]=" ,\n\t",lin[max],linAux[max];

    char mnemonicos[]="/MOV/ADD/SUB/CMP/JMP/JC/JNC/JZ/JNZ/JBE/JA/CALL/RET/HLT/INC/DEC/";
    char doisBytes[]="/MOV/ADD/SUB/CMP/JMP/JC/JNC/JZ/JNZ/JBE/JA/CALL/";
    char doisBytesCTZ[]="/JMP/JC/JNC/JZ/JNZ/JBE/JA/CALL/";
    char umByte[]="/INC/DEC/RET/HLT/";

    //printf("%s\n",aux);
    while(!feof(cod))    {
        fgets(lin,max,cod); //LE LINHA

        aux=strtok(lin,quebra); //QUEBRA LINHA quando encontrar quebra[]

        while(aux!=NULL){

            if(!strcmp("EQU",aux)){
                    linha[l].temRot=1;
                    aux=strtok(NULL," ");
                    //printf("%s\n",aux);
                    linha[l].valor=atoi(aux);
                    break;
            }

            else if (strstr(mnemonicos,aux)==NULL){
                strcpy(linha[l].rot,aux);
                linha[l].temRot=1;
            }

            else if (strstr(mnemonicos,aux)!=NULL){

                linha[l].valor=totalBytes;

                linhaComentada=0;
                temQuebra=0;
                temB=0;
                i=0;
                bytes=0;


                strcpy(linha[l].mn,aux);
                aux=strtok(NULL,quebra);

                printf("%s",linha[l].mn);

                while(aux!=NULL){
                    //printf("%s\n",aux);

                    if(strstr(aux,";")!=NULL){
                        aux[strlen(aux)-1]='\0';
                        linhaComentada=1;
                    }


                        printf("\t%s\t(%d)",aux,i);
                        //strcpy(linha[l].ope[i],aux);

                        if (!strcmp("B",aux) || !strcmp("[B]",aux)){
                            bytes=1;
                            temB=1;
                        }

                        if(strstr(umByte,linha[l].mn)!=NULL)
                            bytes=1;

                        if(strstr(doisBytes,linha[l].mn)!=NULL && !temB)
                            bytes=2;

                    i++;
                    if (!linhaComentada){
                        aux=strtok(NULL,quebra);
                        //printf("%s %d",aux,i);
                    }
                    else break;
                }

                totalBytes+=bytes;
                printf(" %d\n",bytes);

                break;
            }
            //printf("%s\n",aux);

            aux=strtok(NULL,quebra); //quebra por partes ate o final da linha

        }
        l++;
    }
    return (l);
}



int main(){
    int i,linhas;
    char lin[20];
    char nome[20];
    FILE *ASM, *OPC;
    char quebra[]=" [],;";
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

    printf("\nVALOR\tROTULO\t\tMNE\tOP 1\t\tOP 2\n");
    for(i=0; i<linhas; i++)
    //for(i=0; i<linhas&&strcmp(linha[i].rot,"\0"); i++)
        printf("[%d]\t(%s\t)\t(%s)\t(%s\t)\t(%s\t)\n",
        linha[i].valor, linha[i].rot, linha[i].mn,
        linha[i].ope[0], linha[i].ope[1]);

    fclose(ASM);
    fclose(OPC);
}
