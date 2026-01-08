#include "networking.h"
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void sighandler(int signo) {
  if (signo == SIGINT) {
    printf("Interruption");
    exit(0);
  }
}


void clientLogic(int server_socket){
  char buff[256];

  //while(1){
  printf("Please input the first letter of the first name of the artist: ");
  if(fgets(buff,sizeof(buff), stdin) == NULL){
    printf("Client done.\n");
    close(server_socket);
    return;
  }
  //prompted user for string
  int len = strlen(buff);
  int bytes_sent;
  bytes_sent = send(server_socket, buff, len, 0);
  //sending the first letter of artist to the server. the recieve should be the list of artists with first letter.

  int sizes = recv(server_socket, buff, sizeof(buff) -1, 0);
  if(sizes == 0){
    printf("Server closed\n");
    close(server_socket);
    return;
  }
  buff[sizes] = '\0';
  printf("Recieved string: %s\n", buff);
  //exit(1);
  }
}

int main(int argc, char *argv[] ) {
  char* IP = "127.0.0.1";
  if(argc>1){
    IP=argv[1];
  }
  signal(SIGINT, sighandler);
  int server_socket = client_tcp_handshake(IP);
  printf("client connected.\n");
  clientLogic(server_socket);

}
