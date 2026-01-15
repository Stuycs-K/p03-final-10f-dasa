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


void subserver_logic(int client_socket, struct song_node ** library) {
  char letter;
  //reciecing letter
  int sizes = recv(client_socket, &letter, sizeof(letter), 0);
  if(sizes <= 0){
    printf("Socket closed.\n");
    close(client_socket);
    exit(1);
  }
  printf("Client requested letter: %c\n", letter);
  int count;
  char ** songs = by_letter(library, letter, &count);
  send(client_socket, &count, sizeof(int), 0);
  for(int x =0; x < count; x++){
    send(client_socket, songs[x], strlen(songs[x]) + 1, 0);
  }
  free_song_array(songs, count);

  //getting artist name
  char artist[256];
  sizes = recv(client_socket, artist, sizeof(artist), 0);
  if(sizes <= 0){
    close(client_socket);
    exit(1);
  }
  songs = by_artist(library, artist, &count);
  send(client_socket, &count, sizeof(int), 0);
  for(int x = 0; x < count; x++){
    send(client_socket, songs[x], strlen(songs[x]) + 1, 0);
  }
  free_song_array(songs, count);

  char title[256];
  sizes = recv(client_socket,title, sizeof(title),0);
  if(sizes <= 0){
    close(client_socket);
    exit(1);
  }
  struct song_node *song = search_song(library, artist, title);
  char buffer[256];
  sprintf(buffer, "%s - %s", song-> artist, song-> title);
  int len = strlen(buffer) + 1;


  int fd = open(song -> filepath, O_RDONLY);
  if(fd < 0){
    strerror(errno);
    close(client_socket);
    exit(1);
  }
  char buffs[1024];
  int bytes_read;
  bytes_read = read(fd, buffer, sizeof(buffer));
  while(bytes_read > 0){
    send(client_socket, buffs, bytes_read, 0);
    bytes_read = read(fd, buffs, sizeof(buffs));
  }
  close(fd);
  printf("Finished streaming: %s - %s\n", song -> artist, song -> title);
}

int main(int argc, char *argv[]) {

  printf("LINKED LIST TESTS\n====================================\n");

  struct song_node * list = NULL;
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
