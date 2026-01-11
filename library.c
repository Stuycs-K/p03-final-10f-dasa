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


void add(struct song_node** library, char* artist, char* title) {
    if (!artist || !title) return;

    int index = *artist - 'A';
    if (index < 0 || index > 25) index = 26;

    struct song_node *access = library[index];
    struct song_node *new_node = createnode(artist, title, NULL);

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

    new_node->next = current->next;
    current->next = new_node;
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
  struct song_node * access = library[x];
  return find_first(artist, access);
}


void print_list(struct song_node * list){
    struct song_node * current = list;
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

void print_letter(struct song_node ** library, char letter){
    int index = letter - 'A';
    struct song_node * current = library[index];
    print_list(current);
}

void print_artist(struct song_node ** library, char* artist){
    int index = *artist - 'A';
    struct song_node * current = library[index];
    int first = 1;
    while (current != NULL) {
        if (strcasecmp(current->artist, artist) == 0)
        {
            if (!first)
            {
                printf("|");
            }
            printf("[%s, %s]", current->artist, current->title);
            first = 0;
        }
        current = current->next;
    }
    printf("\n");
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
