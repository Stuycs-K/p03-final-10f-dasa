#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

struct song_node{char artist[100];char title[100]; struct song_node *next;};

int print_song_node(struct song_node m){
	printf("%s, %s, %p\n", m.artist, m.title, m.next);
	return 0;
}

struct song_node * createnode(char artist[100], char title[100], struct song_node *next){
    struct song_node *p = (struct song_node*) malloc(sizeof(struct song_node));

    strncpy(p->artist, artist, 99);
    p->artist[99] = '\0';

    strncpy(p->title, title, 99);
    p->title[99] = '\0';

    p->next = next;
    return p;
}

struct song_node * insert_front(struct song_node * list,char * artist, char * title){
    struct song_node * newSongNode = createnode(artist,title,list);
    return newSongNode;
}

int compare(struct song_node * a,struct song_node * b){

    int artistCmp = strcasecmp(a->artist,b->artist);// non case sensitive string compare
    if(artistCmp != 0){
        return artistCmp;//if artists are different compare
    }
    return strcasecmp(a->title,b->title);//if artists are the same compare titles
}

struct song_node * front_abc(struct song_node * list, char * artist, char * title){
    struct song_node * newSongNode = createnode(artist,title,NULL);//null because it doesnt have a next node yet

    if(list == NULL){
        return newSongNode;//if the list is null set it equal to the new node that is created
    }

    if(compare(newSongNode, list)< 0){
        newSongNode->next = list; //sets it to the front of the list if it is alphabeticaly in front
        return newSongNode;
    }

    struct song_node * current = list;
    while(current->next != NULL && compare(current->next, newSongNode) < 0){//keeps going down the list until it needs to be in front of another node
        current = current -> next;
    }

    newSongNode->next = current->next;//places the new song node in between current and the next node after current
    current->next = newSongNode;

    return list;
}

void print_song_list(struct song_node * list){
    struct song_node * current = list;
    while(current != NULL){
        printf("[%s,%s] | ",current->artist,current->title);//iterates through the list until therer is nothing left printing the artist followed by the title
        current = current->next;
    }
}

struct song_node * find_song(char * artist, char * title, struct song_node * list){
    struct song_node * current = list;
    while(current != NULL ){
      if(strcasecmp(current->artist,artist) == 0 && strcasecmp(current->title,title)== 0){//makes sure both the artist and title are correct
        return current;
      }
      else{
          current = current->next;
      }
    }
    return NULL;
}

struct song_node * find_first(char * artist, struct song_node * list){//just makes sure artist is correct, then prints first song
    struct song_node * current = list;
    while(current != NULL){
        if(strcasecmp(current->artist,artist) == 0){
            return current;
        }
        else{
            current = current->next;
        }
    }
    return NULL;
}

struct song_node * random_song(struct song_node * list){
    struct song_node * current = list;
    int count = 0;
    int random = 0;

    if(current == NULL){
        return NULL;
    }

    while(current != NULL){
        count ++;
        current = current->next;
    }

   random += (rand() % count);//random number with how many elemts are in the list

    for(int i = 0; i < random; i ++){
        list = list->next;
    }
    return list;
}


struct song_node * remove_song(struct song_node * list, char * artist, char * title){
    struct song_node * current = list;
    struct song_node * before = NULL;

    if(list == NULL){
        return NULL;
    }

    while(current != NULL){
        if(strcasecmp(current->artist, artist) == 0 && strcasecmp(current->title, title) == 0){

            if(before == NULL){
                struct song_node * front = current->next;
                free(current);
                return front;
            }

            before->next = current->next;
            free(current);
            return list;
        }

        before = current;
        current = current->next;
    }

    return list;
}


void freeing(struct song_node * list){
    struct song_node * current = list;
    struct song_node * tempNext;

    while(current != NULL){
        tempNext = current->next;
        free(current);
        current = tempNext;
    }
}
