#include "networking.h"
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <unistd.h>

void err(int i, char*message){
  if(i < 0){
	  printf("Error: %s - %s\n",message, strerror(errno));
  	exit(1);
  }
}


/*Create and bind a socket.
* Place the socket in a listening state.
* returns the socket descriptor
*/
int server_setup() {
  //setup structs for getaddrinfo
  struct addrinfo *hints, * results;
  hints = calloc(1,sizeof(struct addrinfo));
  hints->ai_family = AF_INET;
  hints->ai_socktype = SOCK_STREAM;
  hints->ai_flags = AI_PASSIVE;
  int stats = getaddrinfo(NULL, "4840", hints, &results);

  if(stats != 0){
    err(stats,"getaddrinfo not working");
  }
  //printf("getaddrinfo\n");
  //create the socket
  int clientd = socket(results ->ai_family, results -> ai_socktype, results -> ai_protocol );//store the socket descriptor here
  if(clientd == -1){
    err(clientd, "socket not working");
  }
  //printf("socket\n");
  //this code should get around the address in use error
  int yes = 1;
  int sockOpt =  setsockopt(clientd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes));
  err(sockOpt,"sockopt  error");

  //bind the socket to address and port
  int binder =  bind(clientd, results-> ai_addr, results-> ai_addrlen);
  if(binder == -1){
    err(binder, "bind not working");
  }
  //printf("bind\n");
  //set socket to listen state
  int listening = listen(clientd, 10);
  if(listening == -1){
    err(listening, "listen not working");
  }
  //printf("listening\n");
  //server_tcp_handshake(clientd);
  //printf("handshake\n");
      /*
      server_tcp_handshake(clientd);
      server_tcp_handshake(clientd);
      server_tcp_handshake(clientd);
      server_tcp_handshake(clientd);
      server_tcp_handshake(clientd);
      server_tcp_handshake(clientd);
      */

  //free the structs used by getaddrinfo
  free(hints);
  freeaddrinfo(results);

  return clientd;
}

/*Accept a connection from a client
 *return the socket descriptor for the new socket connected to the client
 *blocks until connection is made.
 */
int server_tcp_handshake(int listen_socket){
    int client_socket;
    struct sockaddr_storage their_addr;
    socklen_t addr_size = sizeof(their_addr);
    //accept() the client connection
    client_socket = accept(listen_socket, (struct sockaddr*) &their_addr, &addr_size);
    if(client_socket == -1){
      err(client_socket, "accept not working");
    }
    return client_socket;
}


/*Connect to the server
 *return the to_server socket descriptor
 *blocks until connection is made.*/
int client_tcp_handshake(char * server_address) {
  struct addrinfo *hints;
  struct addrinfo* res;
  hints = calloc(1,sizeof(struct addrinfo));
  hints->ai_family = AF_INET;
  hints->ai_socktype = SOCK_STREAM;
  int stats = getaddrinfo(server_address, "4840", hints, &res);
  if(stats != 0){
    err(stats,"getaddrinfo not working");
  }
  //getaddrinfo

  int serverd;//store the socket descriptor here
  //create the socket
  serverd = socket(res -> ai_family, res -> ai_socktype, res-> ai_protocol);
  if(serverd == -1){
    err(serverd,"getaddrinfo not working");
  }
  //connect() to the server
  int connection = connect(serverd, res-> ai_addr, res -> ai_addrlen);
  if(connection == -1){
    err(connection,"getaddrinfo not working");
  }
  free(hints);
  freeaddrinfo(res);
  return serverd;
}
