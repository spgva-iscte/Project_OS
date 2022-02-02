#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include "consulta.h"
#include <signal.h>

Consulta c;
int s;
int n;

void handler_SIGALRM(int sinal){
    n=1;
}

void pedir_Consulta(){
	n=0;
	if(access("./PedidoConsulta.txt",F_OK)==0){
		printf("Erro, Não foi possivel marcar consulta. Vou voltar a tentar passado 10 segundos\n");
		signal(SIGALRM,handler_SIGALRM);
        alarm(10);
        while(n==0)
             pause();
		pedir_Consulta();
	}else{
		FILE *ft=fopen("PedidoConsulta.txt","w");
		if(ft){
			fprintf(ft, "%d,%s,%d\n",c.tipo, c.descricao, c.pid_consulta);
			fclose(ft);
		}else
	    	fprintf(stderr,"Erro, não foi possivel abrir o ficheiro");
	}
}

Consulta criar_consulta(){
	s=0;
	printf("Nova Consulta\n");
	printf("Indique o tipo de consulta (1-Normal, 2-COVID19, 3-Urgente): ");
	scanf("%d",&c.tipo);
	printf("Indique a descrição da consulta: ");
	scanf("%s",&c.descricao);
	c.pid_consulta=getpid();
	pedir_Consulta();
	return c;
}

int ler_PID(){
	int pid;
	FILE* ft=fopen("SrvConsultas.pid","r");
	if(ft){
		fscanf(ft,"%d",&pid);
		kill(pid,SIGUSR1);
	}else
		fprintf(stderr,"Erro, não foi possivel abrir o ficheiro");
	return pid;
}

void apagar_pedido(){
if(remove("./PedidoConsulta.txt")==0)
		printf("O pedido de consulta foi apagado\n");
	else
		printf("Não foi possivel apagar o pedido de consulta\n");
}

void handler_SIGHUP(int sinal){
	printf("Consulta iniciada para o processo %d\n",c.pid_consulta);
	apagar_pedido();
	s=1;
}

void handler_SIGTERM(int sinal){
	if(s==1){
		printf("Consulta concluida para o processo %d\n",c.pid_consulta);
		exit(0);
	}
	else
		printf("Erro, não havia consulta a decorrer\n");
}

void handler_SIGUSR2(int sinal){
	printf("Consulta não é possivel para o processo %d\n",c.pid_consulta);
	apagar_pedido();
}

void handler_SIGINT(int sinal){
	printf("Paciente cancelou pedido\n");
	if(access("./PedidoConsulta.txt",F_OK)==0)
		apagar_pedido();
	exit(0);
}

int main(){
	criar_consulta();
	printf("%d\n",getpid());
	int i=ler_PID();
	printf("%d\n",i);
	printf("Se desejar sair escreva sair\n");
	signal(SIGHUP,handler_SIGHUP);
	signal(SIGTERM,handler_SIGTERM);
	signal(SIGUSR2,handler_SIGUSR2);
	signal(SIGINT,handler_SIGINT);
	char s[100];
	while(1){
		pause();
        fgets(s,100,stdin);
        s[strlen(s) -1]=0;
        if(strcmp(s,"sair")==0)
            exit(0);
    } 
}
