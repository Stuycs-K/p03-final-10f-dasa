#include "networking.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "node.h"
#include "library.h"



static void sighandler(int signo) {
  if (signo == SIGINT) {
    //remove(WKP);
    printf("Interruption");
    exit(0);
  }
}

void sub_libr_send(int client_socket, struct song_node * list){
  struct song_node *current = list;
  int count = 0;

  while(current != NULL){
    count++;
    current = current -> next;
  }

  send(client_socket, &count, sizeof(int),0);

  current = list;
  while(current != NULL){
    send(client_socket, current -> artist, sizeof(current -> artist),0);
    send(client_socket, current -> title, sizeof(current -> title), 0);
    current = current -> next;
  }
}


void subserver_logic(int client_socket, struct song_node ** library) {
  char letter;
  int sizes = recv(client_socket, &letter, sizeof(letter), 0);
  if(sizes <= 0){
    printf("Socket closed.\n");
    close(client_socket);
    exit(1);
  }
  printf("Client requested letter: %s\n", letter);
  int index = letter - 'a';
  if(index < 0 || index > 25){
    int zero = 0;
    send(client_socket, &zero, sizeof(int), 0);
    return;
  }
  sub_libr_send(client_socket, library[index]);
}

int main(int argc, char *argv[]) {
  struct song_node ** library = init();
  signal(SIGINT, sighandler);
  printf("bind complete\n");
  int listen_socket = server_setup();
  printf("server listening for connections.\n");
  while (1) {
    int client_socket = server_tcp_handshake(listen_socket);
    pid_t subserver = fork();
    if (subserver == 0) {
      close(listen_socket);
      while (1) {
        subserver_logic(client_socket, library);
        //sleep(1);
      }
      //close(listen_socket);
      close(client_socket);
      exit(0);
    } else {
      // int num;
      // wait(&num);
      //close(listen_socket);
      close(client_socket);
      printf("Waiting for next client...\n");
    }
  }
  return 0;
}
