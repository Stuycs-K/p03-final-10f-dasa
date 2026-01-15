#ifndef LIBRARY_H
#define LIBRARY_H

#include "node.h"
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include<string.h>
#include "node.h"

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include<string.h>
#include "node.h"

struct song_node ** init();
void add(struct song_node** library, char* artist, char* title, char * filepath);
struct song_node * search_song(struct song_node ** library, char* artist, char* title);
struct song_node * search_artist(struct song_node**library, char*artist, char*title);
char** string_list(struct song_node * list, int *count);
char ** by_letter(struct song_node ** library, char letter, int * count);
char ** by_artist(struct song_node ** library, char* artist, int *count);
void print_library(struct song_node** library);
void shuffle(struct song_node **library, int n);
int delete_song(struct song_node ** library, char* artist, char* title);
void reset(struct song_node ** library);
void free_song_array(char **arr, int count);

#endif