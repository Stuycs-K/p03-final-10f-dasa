#include "networking.h"
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <fcntl.h>

static void sighandler(int signo) {
  if (signo == SIGINT) {
    printf("Interruption");
    exit(0);
  }
}


void clientLogic(int server_socket){
  char buff[256];
  printf("Please input the first letter of the first name of the artist: ");
  if(fgets(buff,sizeof(buff),stdin) == NULL){
    printf("Client done.\n");
    close(server_socket);
    return;
  }
  if(buff[0] == '\n' || buff[0] == '\0'){
    printf("No input provided.\n");
    close(server_socket);
    return;
  }

  char letter = buff[0];
  if(send(server_socket, &letter, 1, 0) <=0 ){
    printf("Failed to send letter.\n");
    close(server_socket);
    return;
  }

  int count = 0;
  int bytes = recv(server_socket, &count, sizeof(int), 0);
  if(bytes <= 0){
    printf("Failed to recieve songs. \n");
    close(server_socket);
    return;
  }

  printf("Recieving %d songs from server...\n", count);

  struct song_node *local_library = NULL;
  struct song_node *tail = NULL;

  
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
