#ifndef NETWORKTHREAD_H
#define NETWORKTHREAD_H

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

typedef struct {
	short empresa;
	char produto[17];
	short controle;
	short data_dia;
	short data_mes;
	short data_ano;
} requisicao_estoque;

extern pthread_t network_thread;

void start_network_thread();

#endif
