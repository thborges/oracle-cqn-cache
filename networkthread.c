
#include "config.h"

pthread_t network_thread = NULL;

void *listen_network_requests(void *ptr) {
	int sockfd, newsockfd, portno, optval;
	unsigned int clilen;
	struct sockaddr_in serv_addr, cli_addr;
	requisicao_estoque *req;
	double saldo;
	int received;
	char buffer[sizeof(req)];

	sockfd = socket(AF_INET, SOCK_STREAM, 0);

	optval = 1;
	setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof optval);

	bzero((char*)&serv_addr, sizeof(serv_addr));
	portno = 9753;
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	serv_addr.sin_port = htons(portno);

	if (bind(sockfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
		perror("Erro ao listar na porta. Bind error.");
		exit(1);
	}

	listen(sockfd, 100);
	clilen = sizeof(cli_addr);
	
	fprintf(stdout, "Aguardando requisicoes.\n");  
	while(1) {
		newsockfd = accept(sockfd, (struct sockaddr*) &cli_addr, &clilen);
		if (newsockfd < 0) {
			perror("Erro ao atender requisicao. Accept error.\n");
			exit(1);
		}

		//fprintf(stdout, "Conexao aceita.\n");

		received = 0;
		do {	
		  received += recv(newsockfd, (char*)&buffer[received], sizeof(requisicao_estoque) - received, 0);
		  //fprintf(stdout, "Obtidos %d bytes\n", received);
		} while (received < sizeof(requisicao_estoque));
		req = (requisicao_estoque*)&buffer[0];


		//printf("GetEstoque %d %s %d/%d/%d %d: ",
		//	req->empresa, req->produto, req->data_dia, req->data_mes, req->data_ano, req->controle);

		saldo = get_cache_estoque(req);
		//fprintf(stdout, "I received %f\n", saldo);

		memcpy(buffer, &saldo, sizeof(double));
		send(newsockfd, buffer, sizeof(double), 0);
		//close(newsockfd);

		//fprintf(stdout, "Requisicao finalizada.\n");

	}

}

void start_network_thread() {
	pthread_create(&network_thread, NULL, listen_network_requests, NULL);
}

