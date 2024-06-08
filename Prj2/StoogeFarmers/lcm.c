#include "lcm.h"

#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


// semaphores
// sem_t shovel;
sem_t shovel,unplanted_hole,unfilled_hole,holes;

void *larry() {
    // some code goes here
    int id = 0;
    while (1) {
        int hole;
        sem_getvalue(&holes,&hole);
        sleep(1);
        usleep(rand() % 1000000);
        if (hole<MAX)
        {
            sem_wait(&shovel);
            get_shovel(LARRY);
            
            dig(LARRY, ++id);
            drop_shovel(LARRY);
            sem_post(&shovel);
            
            sem_post(&unplanted_hole);
            sem_post(&holes);
            if (id==N) pthread_exit(0);
        }
    }
}

void *moe() {
    // some code goes here
    int id = 0;
    while (1) {
        sem_wait(&unplanted_hole);
        sleep(1);
        usleep(rand() % 1000000);
        plant(MOE, ++id);
        sem_post(&unfilled_hole);
        if (id==N) pthread_exit(0);
    }
}

void *curly() {
    // some code goes here
    int id = 0;
    while (1) {
        int hole;
        sem_getvalue(&unfilled_hole,&hole);
        sleep(1);
        usleep(rand() % 1000000);
        if (hole>0)
        {
            
            sem_wait(&shovel);
            get_shovel(CURLY);
            
            fill(CURLY, ++id);
            drop_shovel(CURLY);
            sem_post(&shovel);
            sem_wait(&unfilled_hole);
            sem_wait(&holes);
            
            if (id==N) pthread_exit(0);
        }
    }
}

void init() {
    // some code goes here
    sem_init(&shovel, 0, 1);
    sem_init(&unplanted_hole, 0, 0);
    sem_init(&unfilled_hole, 0, 0);
    sem_init(&holes, 0, 0);
    srand(time(NULL));
}

void destroy() {
    // some code goes here
    sem_destroy(&shovel);
    sem_destroy(&unplanted_hole);
    sem_destroy(&unfilled_hole);
    sem_destroy(&holes);
}
