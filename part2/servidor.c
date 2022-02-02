#include <stdio.h>
#include <unistd.h>
#include "consulta.h"
#include <string.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#define CAPACIDADE 10

Consulta lista_consultas[CAPACIDADE];
int consultas_tipo_1;
int consultas_tipo_2;
int consultas_tipo_3;
int consultas_perdidas;
int n;

void handler_SIGALRM(int sinal){
    n=1;
}

int ver_vagas(){
    int a=0;
    for(int i=0;i<CAPACIDADE;i++)
        if(lista_consultas[i].tipo==-1){
            a=1;
            break;
        }
    return a;
}

int sala_livre(){
    int position;
    for(int i=0;i<CAPACIDADE;i++)
         if(lista_consultas[i].tipo=-1){
             position=i;
             break;
         }
    return position;
}

void handler_SIGUSR1(int sinal){
    Consulta c;
    FILE* ft=fopen("PedidoConsulta.txt","r");
    if(ft){
        fscanf(ft, "%d,%100[^,]%*c%d",&c.tipo, &c.descricao, &c.pid_consulta);
        printf("Chegou novo pedido de consulta do tipo %d, descrição %s e PID %d\n",
            c.tipo,c.descricao,c.pid_consulta);
        if(ver_vagas()==0){
            printf("Lista de consultas cheia\n");
            kill(c.pid_consulta,SIGUSR2);
            consultas_perdidas++;
        }else{
            int sala=sala_livre();
            lista_consultas[sala]=c;
            printf("Consulta agendada para a sala %d\n",sala+1);
            switch(c.tipo){
                case 1: consultas_tipo_1++; break;
                case 2:consultas_tipo_2++; break;
                case 3:consultas_tipo_3++; break;
                default:printf("opção inválida\n");
            }
            pid_t parent =fork();
            if(!parent){
                kill(c.pid_consulta,SIGHUP);
                signal(SIGALRM,handler_SIGALRM);
                alarm(10);
                while(n==0)
                    pause();
                printf("Consulta terminada na sala %d\n",sala+1);
                kill(c.pid_consulta,SIGTERM);
                exit(0);
            }
            wait(NULL);
            lista_consultas[sala].tipo=-1;
            fclose(ft);
        }
    }else
        fprintf(stderr,"Erro, não foi possível abrir o ficheiro\n");
}

void atualizar_stats(){
    FILE * fp=fopen("StatsConsultas.dat","wb");
    if(fp){
        fwrite(&consultas_perdidas,sizeof(int),1,fp);
        fwrite(&consultas_tipo_1,sizeof(int),1,fp);
        fwrite(&consultas_tipo_2,sizeof(int),1,fp);
        fwrite(&consultas_tipo_3,sizeof(int),1,fp);
        fclose(fp);
    }else
        fprintf(stderr,"Erro, não foi possivel abrir o ficheiro");
}

void handler_SIGINT(int sinal){
    if(remove("./SrvConsultas.pid")==0)
        printf("Ficheiro apagado\n");
    else
        printf("Não foi possivel apagar o ficheiro\n");
    atualizar_stats();
    exit(0);
}

void registar_PID(char c[]){
    FILE * ft=fopen(c,"w");
    if(ft){
        fprintf(ft,"%d\n",getpid());
        fclose(ft);
    }else
        fprintf(stderr,"Erro, não foi possivel abrir o ficheiro");
}

void iniciar_lista_consultas(){
    for(int i=0;i<CAPACIDADE;i++)
        lista_consultas[i].tipo=-1;
    consultas_tipo_1=0;
    consultas_tipo_2=0;
    consultas_tipo_3=0;
    consultas_perdidas=0;
    registar_PID("SrvConsultas.pid");
}

int main(){
    iniciar_lista_consultas();
    char s[100];
    printf("Se desejar sair escreva sair\n");
    signal(SIGUSR1,handler_SIGUSR1);
    signal(SIGINT,handler_SIGINT);
    while(1){
        pause();
        fgets(s,100,stdin);
        s[strlen(s) -1]=0;
        if(strcmp(s,"sair")==0)
            exit(0);
    }
}
