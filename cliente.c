#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

#include "networkthread.h"

#ifdef CLIENTE

int main(int argc, char* argv[]) {
	int sockfd, portno, n;
	struct sockaddr_in serv_addr;
	struct hostent *server;
	requisicao_estoque req;
	double saldo;
	int received;
	char buffer[sizeof(req)];

	if (argc < 4) {
		perror("cliente empresa produto data controle");
		exit(1);
	}

	portno = 9753;
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0) exit(1);

	server = gethostbyname("localhost");
	if (server == NULL) exit(1);

	bzero((char*)&serv_addr, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	bcopy((char*)server->h_addr, (char*)&serv_addr.sin_addr.s_addr, server->h_length);
	serv_addr.sin_port = htons(portno);
	if (connect(sockfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0)
		perror("Erro ao conectar.\n");

	req.empresa = atoi(argv[1]);
	strcpy(req.produto, argv[2]);
	req.data_dia = atoi(argv[3]);
	req.data_mes = atoi(argv[4]);
	req.data_ano = atoi(argv[5]);
	req.controle = atoi(argv[6]);

        printf("GetEstoque %d %s %d/%d/%d %d: ",
		req.empresa, req.produto, req.data_dia,
		req.data_mes, req.data_ano, req.controle);

	memcpy(buffer, &req, sizeof(req));
	send(sockfd, buffer, sizeof(buffer), 0);
	
	saldo = 0;
	received = 0;
	do {
		received += recv(sockfd, &buffer[received], sizeof(double)-received, 0);
	} while (received < sizeof(double));
	saldo = *(double*)buffer;
	
	close(sockfd);
	printf("%f\n", saldo);

}

#endif
