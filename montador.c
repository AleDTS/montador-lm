#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    char rot[10];
    char mn[4];
    int ope1,ope2;
    char coment[30];
} LINHA;

typedef struct{
    char rot[10];
    int valor;
} ROTULO;

ROTULO rotulo[10];

LINHA linha[50];

void printaCodigo(FILE *cod, int n){
    char texto[n+1];
    while(fgets(texto,n,cod)){
        printf("%s",texto);
    }
}

void leCodigo(FILE *cod,FILE *ling){
    char ch,aux[20],c;
    int i=0,l=0;
    while(!feof(cod)){
        ch=fgetc(cod);
        if(!feof(cod)){
            for(i=0;(ch!=' ' && ch!=10&& ch!=',' && ch!=';');i++){
                aux[i]=ch;
                aux[i+1]='\0';
                ch=fgetc(cod);
                if(ch==10)l++;
                if(feof(cod)) break;
            }

            printf("%d %s\n",strlen(aux),aux);


        }
    }
    return;
}

int rotulos(FILE *cod){
    char ch,aux[4],valor[4];
    int i=0,j,l=0,pos=0;
    int fimCabecalho=0,temRotulo=0;

    while(!fimCabecalho){

            do{
                ch=fgetc(cod);
                rotulo[l].rot[i]=ch;
                i++;
                rotulo[l].rot[i]='\0';

                if(ch==' '){
                    ch=fgetc(cod);
                    for(j=0;j<3;j++){
                        aux[j]=ch;
                        ch=fgetc(cod);
                    }

                    aux[3]='\0';

                    if (!strcmp(aux,"EQU")){
                        temRotulo=1;
                        rotulo[l].rot[i-1]='\0';
                        for(j=0;j<4&&(ch=getc(cod))>=48&&ch<=57;j++)
                            valor[j]=ch;
                        rotulo[l].valor=atoi(valor);
                        //printf("%s %d\n",rotulo[l].rot,rotulo[l].valor);
                    }
                    else fimCabecalho=1;
                }
            }while(ch!=10);

            if (ch==10) l++;
            i=0;
    }
    return l;
}

int main(){
    int i;
    FILE *ASM, *OPC;

    if ((ASM = fopen("teste2.asm","rt")) == NULL) {
        printf("\nErro ao abrir o arquivo original.\n\n");
        exit(1);
    }

    if ((OPC = fopen("teste1.opc","w+t")) == NULL) {
        printf("\nErro ao abrir o arquivo original.\n\n");
        exit(1);
    }

    //printf("%d",linhas(ASM));

    rotulos(ASM);

    fseek(ASM,0,SEEK_SET);

    leCodigo(ASM,OPC);

    for(i=0;i<20;i+=2)
    //printf("%s %d\n",rotulo[i].rot,rotulo[i].valor);
    printf("%d [%s]\t[%s [%s[, %s]]][;%s]\n",i, linha[i].rot, linha[i].mn, linha[i].ope1, linha[i].ope2, linha[i].coment);


    fclose(ASM);
    fclose(OPC);

}
