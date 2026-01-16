#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include<string.h>
#include "node.h"

struct song_node ** init(){
  struct song_node** r = (struct song_node**)calloc(27, sizeof(struct song_node*));
  return r;
}


void add(struct song_node** library, char* artist, char* title, char * filepath) {
    if (!artist || !title || !filepath) return;

    int index = *artist - 'A';
    if (index < 0 || index > 25) index = 26;

    struct song_node *access = library[index];
    if(library == NULL){
        exit(0);
    }
    struct song_node *new_node = createnode(artist, title,filepath, NULL);

    if (!access || compare(new_node, access) < 0)
    {
        new_node->next = access;
        library[index] = new_node;
        return;
    }

    struct song_node *current = access;
    while (current->next && compare(current->next, new_node) < 0)
    {
        current = current->next;
    }

    new_node-> next = current-> next;
    current-> next = new_node;
}

struct song_node * search_song(struct song_node ** library, char* artist, char* title) {
    if (!artist || !title) return NULL;

    int index = *artist - 'A';
    if (index < 0 || index > 25) index = 26;

    struct song_node *current = library[index];
    while (current != NULL) {
        if (strcasecmp(current->artist, artist) == 0 && strcasecmp(current->title, title) == 0)
        {
            return current;
        }
        current = current->next;
    }
    return NULL;
}

struct song_node * search_artist(struct song_node**library, char*artist, char*title){
  int x = *artist - 'A';
  if(x <0 || x> 25){
    x=26;
  }
  struct song_node * access = library[x];
  return find_first(artist, access);
}


char** string_list(struct song_node * list, int *count){
    int n = 0;
    struct song_node * current = list;
    while (current != NULL) {
        n++;
        current = current-> next;
    }
    *count = n;
    if(n == 0){
        return NULL;
    }
    char **songs = malloc(sizeof(char*) * n);
    current = list;
    int i = 0;
    while(current){
        songs[i] = malloc(strlen(current->artist) + strlen(current->title) + 4); // " - " + \0
        sprintf(songs[i], "%s - %s", current->artist, current->title);
        i++;
        current =current->next;
    }
    return songs;

}

char ** by_letter(struct song_node ** library, char letter, int * count){
    int index = letter - 'A';
    if(index < 0 || index > 25){
        index = 26;
    }
    struct song_node * node = library[index];
    return string_list(node, count);
}


char ** by_artist(struct song_node ** library, char* artist, int *count){
    if(!artist){
        *count=0;
        return NULL;
    }

    int index = *artist - 'A';
    if(index < 0 || index > 25){
        index = 26;
    }


    struct song_node *node = search_artist(library, artist, NULL);
    if(!node){
        *count=0;
        return NULL;
    }
    struct song_node *cur = node;
    int n = 0;
    while(cur && strcmp(cur-> artist, artist) == 0){
        n++;
        cur = cur -> next;
    }

    *count = n;

    char ** songs = malloc(sizeof(char *) * n);

    cur = node;
    int x = 0;
    while(cur && strcmp(cur -> artist, artist) == 0){
        songs[x] = malloc(strlen(cur -> artist) + strlen(cur -> title) + 4);
        sprintf(songs[x], "%s - %s", cur -> artist, cur -> title);
        x++;
        cur = cur -> next;
    }
    return songs;
}

void print_library(struct song_node** library){
for (int i = 0; i < 26; i++) {
        struct song_node * current = library[i];
        if (current != NULL)
        {
            printf("%c:", 'A' + i);
            while (current != NULL) {
                printf("[%s, %s]", current->artist, current->title);
                if (current->next != NULL)
                {
                    printf("|");
                }
                current = current->next;
            }
            printf("\n");
        }
    }
    struct song_node * current = library[26];
    if (current != NULL)
    {
        printf("-:");
        while (current != NULL) {
            printf("[%s, %s]", current->artist, current->title);
            if (current->next != NULL)
            {
                printf("|");
            }
            current = current->next;
        }
        printf("\n");
    }
}


void shuffle(struct song_node **library, int n){
    int total_songs = 0;
    for(int x = 0; x <27; x++){
        struct song_node *current = library[x];
        while(current != NULL){
            total_songs++;
            current = current -> next;
        }
    }
    //total amount of songs
    if(total_songs == 0){
        return;
    }

    for(int x = 0; x < n; x++){
        unsigned int rand_num;
        int fd = open("/dev/urandom", O_RDONLY);
        read(fd, &rand_num, sizeof(rand_num));
        int rand_index = rand_num % total_songs;

        int counts = 0;
        int temp = 0;
        struct song_node * finder = library[temp];
        while(counts <= rand_index && temp < 27){
            if(finder != NULL){
                if(counts == rand_index){
                    break;
                }
                counts++;
                finder = finder -> next;
            } else{
                temp++;
                if(temp < 27){
                    finder = library[temp];
                }
            }
        }

        if(finder != NULL){
            print_song_node(*finder);
            //x++;
        }
    }
}


int delete_song(struct song_node ** library, char* artist, char* title){
    int index = *artist - 'A';
    struct song_node * current = library[index];
    struct song_node * prev = NULL;

    while (current != NULL) {
        if (strcasecmp(current->artist, artist) == 0 && strcasecmp(current->title, title) == 0)
        {
            if (prev == NULL)
            {
                library[index] = current->next;
            }
            else
            {
                prev->next = current->next;
            }
            free(current);
            return 0;
        }
        prev = current;
        current = current->next;
    }
    return 1;
}

void reset(struct song_node ** library){
    for(int i = 0; i < 27; i++){
        if(library[i] != NULL){
            freeing(library[i]);
            library[i] = NULL;
        }
    }
    free(library);
    library = NULL;
}

void free_song_array(char **arr, int count) {
    if(!arr) return;
    for(int i=0;i<count;i++){
        free(arr[i]);
    }
    free(arr);
}
