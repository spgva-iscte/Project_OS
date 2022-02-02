#include "defines.h"
#include "consulta.h"
#include "mensagem.h"
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>
#define CAPACIDADE 10
#define DURACAO 10

typedef struct{
    Consulta lista_consultas[CAPACIDADE];
    int lista_contadores[4];
}Memoria;

Memoria* mem;
int n;

void handler_SIGALRM(int sinal){
    n=1;
}

void limpar_lista(){
    for(int i=0;i<CAPACIDADE;i++)
        mem->lista_consultas[i].tipo=-1;
    for(int i=0;i<4;i++)
        mem->lista_contadores[i]=0;
}

int iniciar_lista(){

    int sem_id= semget( IPC_KEY, 1, IPC_CREAT | 0600 );        //cria um semáforo para sincronização
    exit_on_error(sem_id, "semget");

    int status= semctl(sem_id, 0, SETVAL, 0);              //inicializa o semáforo a 0 para impedir que outro processo possa aceder à zona de memória partilhada antes da inicialização terminar
    exit_on_error(status, "semctl(SETVAL)");

    int id_memoria=shmget(IPC_KEY,sizeof(Memoria), IPC_CREAT | IPC_EXCL | 0600);

    if(id_memoria>0){
        mem=(Memoria *)shmat(id_memoria,NULL,0);
        exit_on_null(mem,"Attach");
        if (mem==NULL) { perror("erro no attach"); exit(1);}
        limpar_lista();
    }else{
        id_memoria=shmget(IPC_KEY,sizeof(Memoria),0);
        exit_on_error(id_memoria,"shmget");
        mem=(Memoria *)shmat(id_memoria,NULL,0);
        exit_on_null(mem,"Attach");
    }

    status = semctl(sem_id, 0, SETVAL, 1);                  // A inicialização terminou, coloca o semáforo a 1 para que outros processos possam aceder à zona de memória partilhada
    exit_on_error(status, "semctl(SETVAL)");

    printf("Memória Inicializada\n");
    return(0);
}

int ver_vaga(){
    for(int i=0;i<CAPACIDADE;i++)
        if(mem->lista_consultas[i].tipo==-1)
            return(1);
    return(0);
}

int sala_livre(){
    for(int i=0;i<CAPACIDADE;i++)
        if(mem->lista_consultas[i].tipo==-1)
            return i;
    return(0);
}

int adicionar_consulta(Consulta c,int sala){
    int sem_id = semget(IPC_KEY, 1, 0 );                             // Obter identificador do semáforo
    exit_on_error(sem_id, "semget");

    struct sembuf DOWN = { .sem_op = -1 };                       // Baixar o valor do semáforo (caso o semáforo esteja a zero o processo fica em espera)
    int status_sem = semop(sem_id, &DOWN, 1);
    exit_on_error(status_sem, "DOWN");

    mem->lista_consultas[sala]=c;
    printf("Consulta agendada para a sala %d\n",sala+1);
    switch(c.tipo){
        case 1: mem->lista_contadores[1]++; break;
        case 2: mem->lista_contadores[2]++; break;
        case 3: mem->lista_contadores[3]++; break;
        default:printf("tipo consulta inválido\n");
    }
        
    struct sembuf UP = { .sem_op = +1 };     // Sobe o valor do semáforo
    status_sem= semop(sem_id, &UP, 1);
    exit_on_error(status_sem, "UP");
    
    return(0);
}

int remover_consulta(int sala){
    mem->lista_consultas[sala].tipo=-1;
    return(0);
}

int servidor_dedicado(Consulta c){
    Mensagem m;
    int status, stat;

    int id_mensagem=msgget(IPC_KEY,0);                                     //liga-se à fila de mensagens
    exit_on_error ( id_mensagem, "Acesso fila de mensagens");

    if(ver_vaga()==0){
        printf("Lista de consultas cheia\n");
        c.status=4;
        m.tipo=c.pid_consulta;
        m.conteudo=c;

        status=msgsnd(id_mensagem, &m, sizeof(m.conteudo), 0);
        exit_on_error(status, "erro ao enviar");

        mem->lista_contadores[0]++;

    }else{
        int sala=sala_livre();
        adicionar_consulta(c,sala);
        c.status=2;
        m.tipo=c.pid_consulta;
        m.conteudo=c;

        status=msgsnd(id_mensagem,&m,sizeof(m.conteudo),0);
        exit_on_error(status, "erro ao enviar");

        signal(SIGALRM,handler_SIGALRM);
        alarm(DURACAO);
        while(n==0){           
            stat=msgrcv(id_mensagem, &m, sizeof(m.conteudo), c.pid_consulta, 0);
            if(stat>0){
                printf("Consulta cancelada pelo utilizador %d\n",m.conteudo.pid_consulta);
                remover_consulta(sala);
                exit(0);
            }
        }

        printf("Consulta terminada na sala %d\n",sala+1);
        remover_consulta(sala);
        c.status=3;
        m.conteudo=c;
        status=msgsnd(id_mensagem,&m,sizeof(m.conteudo),0);
        exit_on_error(status, "erro ao enviar");
    }
}

void handler_SIGINT(int sinal){
    printf("Estatisticas\n");
    printf("%d Consultas do tipo 1\n", mem->lista_contadores[1]);
    printf("%d Consultas do tipo 2\n", mem->lista_contadores[2]);
    printf("%d Consultas do tipo 3\n", mem->lista_contadores[3]);
    printf("%d Consultas perdidas\n", mem->lista_contadores[0]);
    exit(0);
}

int main(){
    int id_mensagem=msgget(IPC_KEY, IPC_CREAT | IPC_EXCL |0600);                 
    if(id_mensagem<0){
        id_mensagem=msgget(IPC_KEY, IPC_CREAT|0600); 
        exit_on_error(id_mensagem,"Erro no msgget");
    }

    iniciar_lista();                                                                                    

    int sem_id = semget(IPC_KEY, 1, 0 );                             // Obter identificador do semáforo
    exit_on_error(sem_id, "semget");

    while(1){
        Mensagem m;

        int stat=msgrcv(id_mensagem, &m, sizeof(m.conteudo), 1, 0);  //recebe mensagem
	    exit_on_error(stat, "erro ao receber");

        signal(SIGINT,handler_SIGINT);

        printf("Chegou novo pedido de consulta do tipo %d, descrição %s e PID %d\n", m.conteudo.tipo,m.conteudo.descricao,m.conteudo.pid_consulta);

        pid_t parent=fork();
        if(!parent){
            signal(SIGINT,SIG_IGN); 
            servidor_dedicado(m.conteudo);
            exit(0);
        }
        signal(SIGCHLD,SIG_IGN);                    //mata zombies

    }

}