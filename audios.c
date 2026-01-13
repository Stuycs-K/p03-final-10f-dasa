#include <mpg123.h>
#include "networking.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "node.h"
#include "library.h"
#include "server.h"
#include "client.h"

void redirect_stdin(char ** args, int len){
  char * after_redirect[len];
  char * source = malloc(BUFFER_SIZE);
  
}
