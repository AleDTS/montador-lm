#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct{ //STRUCT DOS ELEMENTOS DE CADA LINHA
    int valor;
    int temRot;
    int bytes;
    char rot[10];
    char mn[5];
    char ope[2][10];
} LINHA;

typedef struct{ //STRUCT DE ROTULOS
    char rot[10];
    int valor;
} ROTULO;

LINHA linha[100];
ROTULO rotulo[20];

void maiuscula(char *str){
    char aux[strlen(str)+1],ch;
    int i;

    for(i=0;i<strlen(str);i++){
        aux[i]=str[i];
        if (str[i]=='\n') {
            aux[i]=='\0';
            break;
        }
    }
    aux[i]='\0';
    strcpy(str,aux);
}

void quebraColchete(char str[]){ //QUEBRA STRING DENTRO DE []
    int tam=strlen(str)-1;
    int i;
    char aux[tam];
    for(i=0;i<tam-1;i++)
        aux[i]=str[i+1];
    aux[i]='\0';
    //printf("%s\n",str);
    strcpy(str,aux);
}

int valorRotulo(char byte[], int tam){ //RETURN VALOR RELATIVO AO ROTULO
    int i;

    for(i=0;i<tam;i++){
        //printf("%s %s\n",byte,rotulo[i].rot);
        if (!strcmp(byte,rotulo[i].rot)){
            //printf("%s %d",byte,rotulo[i].valor);
            return (rotulo[i].valor);
        }
    }
    return -1;
}

void imprimeLinhas(int tam){ //IMPRIME TODAS AS LINHAS COM ELEMENTOS SEPARADOS NA STRUCT
    int i;
    printf("\nVALOR\tROTULO\t\tMNE\tOP 1\t\tOP 2\n\n");
    for(i=0; i<tam; i++)
        printf("[%d]\t%s\t\t%s\t%s\t\t%s\t\t(%d)\n",
        linha[i].valor, linha[i].rot, linha[i].mn,
        linha[i].ope[0], linha[i].ope[1], linha[i].bytes);
}

void imprimeRotulos(int tam){ //FUNCAO QUE IMPRIME TABELA DE ROTULOS
    int i;
    printf("\nROTULO\t\tVALOR\n\n");
    for (i=0;i<tam;i++)
        printf("%s\t\t%d\n",rotulo[i].rot,rotulo[i].valor);
    printf("\n");
}

int vetorRotulos(int linhas){ //JOGA ROTULOS NO VETOR DE OUTRA STRUCUT
    int i,j=0;
    for(i=0;i<linhas;i++)
        if (linha[i].temRot==1){
            strcpy(rotulo[j].rot,linha[i].rot);
            rotulo[j].valor=linha[i].valor;
            j++;
        }
    return j;
}

int rotulaCodigo(FILE *cod){ //RETORNA No DE LINHAS, FUNCAO SEPARA COMANDOS NA STRUCT
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
        //maiuscula(lin);

        if(strlen(lin)>1){
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
                    //printf("%s\n",linha[l].mn);
                    aux=strtok(NULL,quebra);


                    if(strstr(umByte,linha[l].mn)!=NULL) //1 BYTE
                        bytes=1;
                    else if(strstr(doisBytes,linha[l].mn)!=NULL && !temB) //2 BYTES
                        bytes=2;
                    
                    //printf("%s %d\n",linha[l].mn,bytes);

                    while(aux!=NULL){ //ENQUANTO NAO EH FINAL DA LINHA
                        ///printf("%s\n",aux);
                        //printf("%s\n",linha[l].mn);

                        if(strstr(aux,";")!=NULL){ //CHECA COMENTARIO ;
                            aux[strlen(aux)-1]='\0';
                            linhaComentada=1;
                        }

                        if (!strcmp("B",aux) || !strcmp("[B]",aux)){ //SE TEM B OU [B], 1 BYTE
                            bytes=1;
                            temB=1;
                        }
                             strcpy(linha[l].ope[rot],aux);
                             //printf("\t%s\t(%d)",linha[l].ope[rot],rot);

                        rot++;
                        if (!linhaComentada)
                            aux=strtok(NULL,quebra);
                            //printf("%s %d",aux,i);
                        else break;
                    }
                    linha[l].bytes=bytes;
                    totalBytes+=bytes;
                    //printf(" %d\n",bytes);
                    break;
                }
                //printf("%s\n",aux);
                aux=strtok(NULL,quebra); //quebra por partes ate o final da linha
            }
            if (!feof(cod)) l++;
        }
    }
    return (l);
}

int criaCodigo(int linhas, int rotulos, FILE *opcode){ //CRIA ARQUIVO DE SAIDA (OPCODES)
    int i,j,ehEndereco[2];
    int sucesso=1;
    int bytes, byte[2];
    char rot[10], mne[5], ope[2][10];

    for(i=0;i<linhas;i++){ //LINHA A LINHA
        if(strcmp(linha[i].mn,"EQU")){ //PULA EQU

            //AUXILIARES
            bytes=linha[i].bytes;
            strcpy(rot,linha[i].rot);
            strcpy(mne,linha[i].mn);

            for(j=0;j<2;j++){ //LE OS 2 BYTES
                strcpy(ope[j],linha[i].ope[j]); //AUXILIAR
                ehEndereco[j]=0;

                if (!strcmp(ope[j],"A") || !strcmp(ope[j],"B"))
                    byte[j]=-1;
                else if (ope[j][0]=='['){ //SE EH ENDERECO []

                    ehEndereco[j]=1;
                    quebraColchete(ope[j]); //FUNCAO PARA QUEBRAR []
                    //printf("%s\n",ope[j]);
                    if (strcmp("B",ope[j])){ //SE NAO EH B
                        //strcpy(ope[j],"\0");
                        byte[j]=valorRotulo(ope[j],rotulos); //BYTE RECEBE O VALOR RELATIVO AO ROTULO
                        if (byte[j]<0){ //SE O ROTULO NAO FOI DECLARADO
                            sucesso=0;
                            printf("\nErro na linha %d, rotulo \"%s\" nao econtrado!\n",i,ope[j]);
                        }
                        //printf("%s - %d\n",ope[j],byte[j]);
                    }

                }
                else if((byte[j]=valorRotulo(ope[j],rotulos))<0){ //SE NAO EH ENDEREDO NEM B
                    byte[j]=atoi(linha[i].ope[j]); //ENTAO EH VALOR NUMERICO, BYTE RECEBE O VALOR
                    //printf("%s - %d\n",ope[0],byte[0]);
                }
            }

            //printf("%s\t%s\t%s\t%d %d\t%s\t%d %d\n",rot,mne,ope[0],byte[0],ehEndereco[0],ope[1],byte[1],ehEndereco[1]);

            //COMPARA MNEMONICOS
            if(!strcmp(mne,"ADD")){
                if(bytes==1)
                    fprintf(opcode,"03h\n");
                else if(ehEndereco[1]){
                    fprintf(opcode,"02h\n");
                    fprintf(opcode,"%d\n",byte[1]);
                }
                else {
                    fprintf(opcode,"04h\n");
                    fprintf(opcode,"%d\n",byte[1]);
                }
            }

            else if(!strcmp(mne,"SUB")){
                if(bytes==1)
                    fprintf(opcode,"2bh\n");
                else if(ehEndereco[1]){
                    fprintf(opcode,"2ah\n");
                    fprintf(opcode,"%d\n",byte[1]);
                }
                else {
                    fprintf(opcode,"2ch\n");
                    fprintf(opcode,"%d\n",byte[1]);
                }
            }

            else if(!strcmp(mne,"CMP")){
                if(bytes==1)
                    fprintf(opcode,"3bh\n");
                else if(ehEndereco[1]){
                    fprintf(opcode,"3ah\n");
                    fprintf(opcode,"%d\n",byte[1]);
                }
                else {
                    fprintf(opcode,"3ch\n");
                    fprintf(opcode,"%d\n",byte[1]);
                }
            }

            else if(!strcmp(mne,"INC")){
                if (!strcmp(ope[0],"A"))
                    fprintf(opcode,"40h\n");
                else if (!strcmp(ope[0],"B"))
                    fprintf(opcode,"41h\n");
            }

            else if(!strcmp(mne,"DEC")){
                if (!strcmp(ope[0],"A"))
                    fprintf(opcode,"42h\n");
                else if (!strcmp(ope[0],"B"))
                    fprintf(opcode,"43h\n");
            }

            else if(!strcmp(mne,"JC")){
                fprintf(opcode,"72h\n");
                fprintf(opcode,"%d\n",byte[0]);
            }

            else if(!strcmp(mne,"JNC")){
                fprintf(opcode,"73h\n");
                fprintf(opcode,"%d\n",byte[0]);
            }

            else if(!strcmp(mne,"JZ")){
                fprintf(opcode,"74h\n");
                fprintf(opcode,"%d\n",byte[0]);
            }

            else if(!strcmp(mne,"JNZ")){
                fprintf(opcode,"75h\n");
                fprintf(opcode,"%d\n",byte[0]);
            }

            else if(!strcmp(mne,"JBE")){
                fprintf(opcode,"76h\n");
                fprintf(opcode,"%d\n",byte[0]);
            }

            else if(!strcmp(mne,"JA")){
                fprintf(opcode,"77h\n");
                fprintf(opcode,"%d\n",byte[0]);
            }

            else if(!strcmp(mne,"MOV")){

                //printf("%s  %s %d   %s %d - ",mne,ope[0],ehEndereco[0],ope[1],ehEndereco[1]);

                if (byte[0]<0 && ehEndereco[1]){
                    if (strcmp(ope[1],"B")){ //MOV A,[]
                        fprintf(opcode,"a0h\n");
                        fprintf(opcode,"%d\n",byte[1]);
                        //printf("a0h ");
                        //printf("%d",byte[1]);
                    }
                    else //MOV A,[B]
                        fprintf(opcode,"a1h\n");
                        //printf("a1h");
                }

                if (ehEndereco[0]){
                    if (!strcmp(ope[0],"B")) //MOV [B],A
                        fprintf(opcode,"a3h\n");
                        //printf("a3h");
                    else {
                        fprintf(opcode,"a2h\n"); //MOV [],A
                        fprintf(opcode,"%d\n",byte[0]);
                        //printf("a2h ");
                        //printf("%d",byte[0]);
                    }
                }

                if (byte[0]<0 && byte[1]<0){
                    if (!strcmp(ope[0],"A")) //MOV A,B
                        fprintf(opcode,"88h\n");
                        //printf("88h");
                    else //MOV B,A
                        fprintf(opcode,"8ah\n");
                        //printf("8ah");
                }

                if (!strcmp(ope[0],"A") && !ehEndereco[1] && strcmp(ope[1],"B")){ //MOV A,
                    fprintf(opcode,"b0h\n");
                    fprintf(opcode,"%d\n",byte[1]);
                    //printf("b0h");
                    //printf("%d",byte[1]);
                }
                //printf("\n");
            }

            else if(!strcmp(mne,"JMP")){
                fprintf(opcode,"ebh\n");
                fprintf(opcode,"%d\n",byte[0]);
            }

            else if(!strcmp(mne,"CALL")){
                fprintf(opcode,"e8h\n");
                fprintf(opcode,"%d\n",byte[0]);
            }

            else if(!strcmp(mne,"RET"))
                fprintf(opcode,"c3h\n");

            else if(!strcmp(mne,"HLT"))
                fprintf(opcode,"f4h\n");
        }
    }
    if (sucesso) return 1;
        else return 0;
}

int main(int argc, char *nome[]){
    int linhas,rotulos;
    FILE *ASM, *OPC;

    if (argc!=3){
        printf("\nUtilize ./montador <nome_entrada> <nome_saida>\n");
        exit(1);
    }

    if ((ASM = fopen(nome[1],"rt")) == NULL)    {
        printf("\nNao foi possivel achar o arquivo.\n\n");
        exit(1);
    }

    if ((OPC = fopen(nome[2],"w+t")) == NULL)    {
        printf("\nErro ao criar o arquivo.\n\n");
        exit(1);
    }

    linhas=rotulaCodigo(ASM);
    rotulos=vetorRotulos(linhas);
    imprimeRotulos(rotulos);
    //imprimeLinhas(linhas);
    if(!criaCodigo(linhas,rotulos,OPC)){
        remove(nome[2]);
        printf("\nO arquivo %s foi removido\n",nome[2]);
    }
    else printf("Arquivo \"%s\" gerado com sucesso!\n",nome[2]);

    fclose(ASM);
    fclose(OPC);
    exit(1);
}
