#include <stdio.h>
#include <pthread.h>

#include "config.h"

int main(int argc, char* argv[]) {
	
        // Inicia o banco em memoria sqlite
	start_sqlite_db();

	// Inicia conexao com oracle
	start_oracle_db();
	start_oracle_cqn();

	// Inicia o atendimento a requisicoes
	start_network_thread();
	pthread_join(network_thread, NULL);

	return 0;
}
