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
    char ope[2][10];
} LINHA;

LINHA linha[100];

int rotulos(FILE *cod){
    int max=100,l=0;
    int linhaComentada,temQuebra, temB;
    int temEQU=0, bytes=0,rot=0,totalBytes=0;
    char *aux, lin[max];

    char quebra[]=" ,\n\t";
    char mnemonicos[]="/MOV/ADD/SUB/CMP/JMP/JC/JNC/JZ/JNZ/JBE/JA/CALL/RET/HLT/INC/DEC/";
    char doisBytes[]="/MOV/ADD/SUB/CMP/JMP/JC/JNC/JZ/JNZ/JBE/JA/CALL/";
    char umByte[]="/INC/DEC/RET/HLT/";

    //printf("%s\n",aux);
    while(!feof(cod))    {
        fgets(lin,max,cod); //LE LINHA

        aux=strtok(lin,quebra); //QUEBRA LINHA quando encontrar quebra[]

        while(aux!=NULL){

            if(!strcmp("EQU",aux)){ //SE AUX EH EQU
                    linha[l].temRot=1;
                    aux=strtok(NULL," ");
                    //printf("%s\n",aux);
                    linha[l].valor=atoi(aux);
                    strcpy(linha[l].mn,"EQU");
                    break;
            }

            else if (strstr(mnemonicos,aux)==NULL){ //SE AUX EH ROTULO
                strcpy(linha[l].rot,aux);
                linha[l].temRot=1;
            }

            else if (strstr(mnemonicos,aux)!=NULL){ //COMANDOS

                linha[l].valor=totalBytes;

                //LIMPA VARIAVEIS DE CONTROLE
                linhaComentada=0;
                temQuebra=0;
                temB=0;
                rot=0;
                bytes=0;

                //COPIA MNEMONICO E QUEBRA AUX
                strcpy(linha[l].mn,aux);
                aux=strtok(NULL,quebra);

                //printf("%s",linha[l].mn);

                while(aux!=NULL){ //ENQUANTO NAO EH FINAL DA LINHA
                    //printf("%s\n",aux);

                    if(strstr(aux,";")!=NULL){ //CHECA COMENTARIO ;
                        aux[strlen(aux)-1]='\0';
                        linhaComentada=1;
                    }

                         strcpy(linha[l].ope[rot],aux);
                         //printf("\t%s\t(%d)",linha[l].ope[rot],rot);

                        if (!strcmp("B",aux) || !strcmp("[B]",aux)){ //SE TEM B OU [B], 1 BYTE
                            bytes=1;
                            temB=1;
                        }
                        if(strstr(umByte,linha[l].mn)!=NULL) //SE NAO, 2 BYTES
                            bytes=1;
                        if(strstr(doisBytes,linha[l].mn)!=NULL && !temB)
                            bytes=2;

                    rot++;
                    if (!linhaComentada)
                        aux=strtok(NULL,quebra);
                        //printf("%s %d",aux,i);
                    else break;
                }
                totalBytes+=bytes;
                //printf(" %d\n",bytes);
                break;
            }
            //printf("%s\n",aux);
            aux=strtok(NULL,quebra); //quebra por partes ate o final da linha
        }
        if (!feof(cod)) l++;
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

    printf("\nVALOR\tROTULO\t\tMNE\t\OP 1\t\tOP 2\n\n");
    for(i=0; i<linhas; i++)
    //for(i=0; i<linhas&&strcmp(linha[i].rot,"\0"); i++)
        printf("[%d]\t%s\t\t%s\t%s\t\t%s\t\n",
        linha[i].valor, linha[i].rot, linha[i].mn,
        linha[i].ope[0], linha[i].ope[1]);

    fclose(ASM);
    fclose(OPC);
}
