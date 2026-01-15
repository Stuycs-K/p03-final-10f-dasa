
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "node.h"
#include "library.h"
#include "server.h"
#include "client.h"
#include <string.h>
#include <sys/stat.h>
#include <signal.h>
#include <sys/wait.h>

int play_song(char * song_path){
  int pid = fork();
  if(pid == 0){
    char * args[] = {"mpg123", song_path, NULL};
    execvp("mpg123", args);
    exit(1);
  }

  return pid;
}

void stop_song(int pid){
  if(pid > 0){
    kill(pid, SIGTERM);
    waitpid(pid, NULL, 0);
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
