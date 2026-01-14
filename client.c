#include "networking.h"
#include "node.h"
#include "library.h"
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <fcntl.h>
//#include <mpg123.h>


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

  int count;
  int bytes = recv(server_socket, &count, sizeof(int), 0);
  char **songs = malloc(sizeof(char* )* count);
  printf("Songs for letter %c: \n", letter);
  for(int x = 0; x < count; x++){
    songs[x] = malloc(256);
    recv(server_socket, songs[x], 256,0);
    printf("%s\n", songs[x]);
  }
  for(int x = 0; x < count; x++){
    free(songs[x]);  
  }
  free(songs);

  printf("Enter artist: ");
  fgets(buff, sizeof(buff), stdin);
  if(buff[strlen(buff) - 1] == '\n'){
    buff[strlen(buff) - 1] = '\0';
  }
  send(server_socket, buff, strlen(buff) + 1, 0);

  recv(server_socket, &count, sizeof(int), 0);
  songs = malloc(count * sizeof(char*));
  for(int x = 0; x < count; x++){
    songs[x] = malloc(256);
    recv(server_socket, songs[x], 256,0);
    printf("%d: %s\n", x + 1, songs[x]);
  }

  printf("Choose song: ");
  fgets(buff,sizeof(buff), stdin);
  if(buff[strlen(buff) -1] == '\n'){
    buff[strlen(buff) - 1] = '\0';
  }
  send(server_socket, buff, strlen(buff) + 1, 0);
  close(server_socket);
  
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
