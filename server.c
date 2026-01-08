#include "networking.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void rotX(char *s, int x) {
  int len = strlen(s);
  for (int a = 0; a < len; a++) {
    int temp = *(s + a);
    if (temp >= 'A' && temp <= 'Z') {
      int z = temp + x;
      if (z <= 'Z') {
        *(s + a) = (char)z;
      } else if (z >= 'Z') {
        int sub = z - 'Z';
        *(s + a) = (char)('A' + sub - 1);
      }
    } else if (temp >= 'a' && temp <= 'z') {
      if (temp <= 'z') {
        int o = temp + x;
        if (o <= 'z') {
          *(s + a) = (char)o;
        } else if (o >= 'z') {
          int sub = o - 'z';
          *(s + a) = (char)('a' + sub - 1);
        }
      }
    }
  }
}

static void sighandler(int signo) {
  if (signo == SIGINT) {
    //remove(WKP);
    printf("Interruption");
    exit(0);
  }
}

void subserver_logic(int client_socket) {
  char buff[256];
  int sizes = recv(client_socket, buff, sizeof(buff) - 1, 0);
  if(sizes <= 0){
    printf("Socket closed.\n");
    exit(1);
  }
  buff[sizes] = '\0';
  char original[256];
  strcpy(original, buff);
  rotX(buff, 13);
  printf("%s becomes: %s\n",original, buff);
  send(client_socket, buff, strlen(buff), 0);
}

int main(int argc, char *argv[]) {
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
        subserver_logic(client_socket);
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
