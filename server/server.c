#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <errno.h>

#define DEFAULT_PORT 1980
#define MAXBUF 1024
#define MAXCLI 512

typedef struct client 
{
	int	fd;		// client socket
	char ip[16];
	unsigned short port;
} client_t;

typedef struct server 
{
	unsigned short port;	// port master will
	int fd;					// server socket, on which it accept conn
	int maxfd;				// use it in select/pool
	client_t **clients;		// clients array
	short clen;				// length of the clients array
	short num_clients;		// number of clients connected

} server_t;

client_t *
get_free_client(server_t *srv) 
{
	int i;
	// should we expand clients array ? //
	if(srv->num_clients == srv->clen) {
		srv->clen = srv->clen == 0 ? 4 : srv->clen * 2;
		srv->clients = realloc(srv->clients, srv->clen*sizeof(*srv->clients));
		if(!srv->clients) {
			perror("realloc");
			exit(1);
		}
		for(i = srv->num_clients; i < srv->clen; i++)
			srv->clients[i] = NULL;
	}
	// find free slot //
	for(i = 0; i < srv->clen; i++) {
		if(srv->clients[i] == NULL) {
			srv->clients[i] = calloc(1, sizeof(*srv->clients[i]));
			srv->num_clients++;
			return srv->clients[i];
		}
	}
	return NULL;
}

#define max(a, b) (a) > (b) ? a : b
void 
server_loop(server_t *srv) 
{
	client_t *client;
	int cfd;
	struct sockaddr_in caddr;
	socklen_t addr_len = sizeof(caddr);
	fd_set rfds;
	int maxfd, i, j;
	int sret, nb;
	struct timeval tv;
	char buf[1024];

	while(1) {
		tv.tv_sec = 0;
		tv.tv_usec = 10000;
		FD_ZERO(&rfds);
		FD_SET(srv->fd, &rfds);
		maxfd = srv->fd;
		for(i = 0; i < srv->num_clients; i++) {
			if(!srv->clients[i]) continue;
			FD_SET(srv->clients[i]->fd, &rfds);
			maxfd = max(maxfd, srv->clients[i]->fd);
		}
		if((sret = select(maxfd + 1, &rfds, NULL, NULL, &tv)) < 0) {
			if(errno == EINTR) 
				continue;
			perror("select");
			exit(1);
		} 
		else if(sret == 0) {
			continue;
		}
		
		if(FD_ISSET(srv->fd, &rfds)) {
			// accept_client
			cfd = accept(srv->fd, (struct sockaddr *)&caddr, &addr_len);
			if(cfd < 0) {
				perror("accept");
				exit(1);
			}
			if(!(client = get_free_client(srv))) {
				fprintf(stderr, "Error creating client\n");
				exit(1);
			}
			printf("%s:%d connected\n", inet_ntoa(caddr.sin_addr), 
				ntohs(caddr.sin_port));

			client->fd = cfd;
			client->port = ntohs(caddr.sin_port);
			strcpy(client->ip, inet_ntoa(caddr.sin_addr));
			for(i = 0; i < srv->clen; i++) {
				if(!srv->clients[i]) continue;
				printf("%s: %d: %i\n", client->ip, i, client->fd);
			}
		} 
		else {
			for(i = 0; i < srv->clen; i++) {
				if(!srv->clients[i]) continue;
				if(FD_ISSET(srv->clients[i]->fd, &rfds)) {
					if((nb = read(srv->clients[i]->fd, buf, sizeof(buf)-1)) == 0) {
						// client disconnected //
						printf("client %d: %s disconnected\n", i, srv->clients[i]->ip);
						close(srv->clients[i]->fd);
						free(srv->clients[i]);
						srv->clients[i] = NULL;
						srv->num_clients--;
					} 
					else {
						buf[nb] = 0;
						for(j = 0; j < srv->clen; j++) {
							if(!srv->clients[j]) 
								continue;
							if(srv->clients[j]->fd == srv->clients[i]->fd)
								continue;
							send(srv->clients[j]->fd, buf, strlen(buf), 0);
						}
					}
				}
			}
		}
	}
}


int 
main() 
{
	struct sockaddr_in saddr;
	server_t *srv;

	if(!(srv = malloc(sizeof(*srv)))) {
		perror("malloc");
		return 1;
	}
	srv->port = DEFAULT_PORT;
	if((srv->fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		perror("socket");
		return 1;
	}
	int en = 1;
	if(setsockopt(srv->fd, SOL_SOCKET, SO_REUSEADDR, &en, sizeof(en)) < 0) {
		perror("setsockopt");
		close(srv->fd);
		return 1;
	}
	memset(&saddr, 0, sizeof(saddr));
	saddr.sin_family = AF_INET;
	saddr.sin_port = htons(srv->port);
	saddr.sin_addr.s_addr = INADDR_ANY;
	if(bind(srv->fd, (struct sockaddr*)&saddr, sizeof(saddr)) < 0) {
		perror("bind");
		close(srv->fd);
		return 1;
	}
	if(listen(srv->fd, 32) < 0) {
		perror("listen");
		close(srv->fd);
		return 1;
	}
	printf("Listening on localhost:%d\n", srv->port);
	server_loop(srv);
	return 0;
}
