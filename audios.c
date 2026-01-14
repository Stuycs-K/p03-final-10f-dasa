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

void playing(char * filename){
  mpg123_handle *mh;
  char *buffer;
  size_t buffer_size;
  size_t done;
  int err;

  if(mpg123_init() != MPG123_OK){
    exit(1);
  }

  mh = mpg123_new(NULL, &err);

}
