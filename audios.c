#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>
#include <signal.h>
#include <sys/wait.h>
#include "networking.h"


int play_song_pipev(int *write_fd){
  int fd[2];
  if(pipe(fd) == -1){
    exit(1);
  }

  int pid = fork();
  if(pid == 0){
    dup2(fd[0], STDIN_FILENO);
    close(fd[0]);
    close(fd[1]);

    char *args[] = {"mpg123", "-", NULL};
    execvp("mpg123", args);
    exit(1);
  }
  close(fd[0]);
  *write_fd = fd[1];
  return pid;
}


void stream_song(int server_socket){
  int player_pid;
  int player_fd;
  player_pid = play_song_pipev(&player_fd);

  char buffs[4096];
  int bytes_read;
  while ((bytes_read = recv(server_socket, buffs, sizeof(buffs),0)) > 0) {
    int writing = 0;
    while(writing < bytes_read){
      int a = write(player_fd, buffs + writing , bytes_read - writing);
      if(a <= 0){
        strerror(errno);
        exit(1);
      }
      writing += a;
    }
  }
  close(player_fd);
  waitpid(player_pid, NULL, 0);

}
void send_client(int player_pid, int write_fd, char command){
  if(player_pid > 0){
    char buff[2];
    buff[0] = command;
    buff[1] = '\n';
    write(write_fd, buff, 2);
    if(command == 'q'){
      waitpid(player_pid, NULL,0);
    }
  }
}

void deleting_song(char * song_path){
  int pid = fork();
  if(pid == 0){
    char * args[] = {"rm", song_path, NULL};
    execvp("rm", args);
    exit(1);
  }
  waitpid(pid, NULL, 0);
}

void stop_song(int pid){
  if(pid > 0){
    kill(pid, SIGTERM);
    waitpid(pid, NULL, 0);
  }
}
