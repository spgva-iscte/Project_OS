#ifndef __CONSULTA_H__
#define __CONSULTA_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    int tipo;               // Tipo de Consulta: 1-Normal, 2-COVID19, 3-Urgente
    char descricao[100];    // Descrição da Consulta
    int pid_consulta;       // PID do processo que quer fazer a consulta
    int status;             // Estado da consulta: 1-Pedido, 2-Iniciada,3-Terminada, 4-Recusada, 5-Cancelada
} Consulta;                 
#endif