#include "consulta.h"
#include "defines.h"
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include "mensagem.h"

int n;                           
Consulta c;

Consulta criar_consulta(){
	int n=0;
	printf("Nova Consulta\n");
	printf("Indique o tipo de consulta (1-Normal, 2-COVID19, 3-Urgente): ");
	scanf("%d",&c.tipo);
	if(c.tipo!=1 && c.tipo!=2 && c.tipo!=3){
		printf("Tipo de consulta inválido\n");
		exit(1);
	}
	printf("Indique a descrição da consulta: ");
	scanf("%s",&c.descricao);
	c.pid_consulta=getpid();
	c.status=1;
	Mensagem m;
	m.tipo=c.status;
	m.conteudo=c;
	int id_mensagem=msgget(IPC_KEY,0);
	exit_on_error(id_mensagem, "Erro no msgget o servidor está inativo");
	int status=msgsnd(id_mensagem,&m,sizeof(m.conteudo),0);
	exit_on_error(status, "erro ao enviar");
	return c;
}

int handler_Mensagem(Mensagem m){
	switch(m.conteudo.status){
		case 1: printf("Consulta em estado de pedido\n");
				break;
		case 2: printf("Consulta iniciada para o processo %d\n",getpid()); 
				n=1; 
				break;
		case 3: if(n==1)
					printf("Consulta concluída para o processo %d\n",getpid());
				else
					printf("Erro, a consulta não tinha sido iniciada\n");
				exit(0);
				break;
		case 4: printf("Consulta recusada para o processo %d\n",getpid());
				exit(0);
				break;
		case 5: printf("Consulta cancelada\n");
				break;
		default: printf("Status inválido\n");
	}
	return(0);
}

void handler_SIGINT(int sinal){
	Mensagem m;
	m.tipo=getpid();
	c.status=5;
	m.conteudo=c;
	printf("Paciente cancelou pedido\n");
	int id=msgget(IPC_KEY,0);
	exit_on_error(id, "Erro no msgget");
	int status=msgsnd(id,&m,sizeof(m.conteudo),0);
	exit_on_error(status, "erro ao enviar");
	exit(0);
}

int main(){
    c=criar_consulta();
	signal(SIGINT,handler_SIGINT);
	while(1){
		Mensagem m;
		int id_mensagem=msgget(IPC_KEY,0);
		exit_on_error(id_mensagem, "Erro no msgget");
		int stat= msgrcv(id_mensagem, &m, sizeof(m.conteudo), getpid(), 0);
		exit_on_error(stat, "erro ao receber");
		handler_Mensagem(m);
	}
}