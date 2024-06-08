#include "party.h"

#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

sem_t enter,leave,students_inside;

void *dean() {
    usleep((rand() % 500 + 200) * 1000);
    for (int i = 0; i < N; i++) {
       usleep((rand() % 500 + 200) * 1000);
        // some code goes here
        int students;
        sem_getvalue(&students_inside, &students);
        while (1) {
            if (students == 10 || students == 0) 
            {
                sem_wait(&leave);
                sem_wait(&enter);
                break;
            }
          //  usleep(1000);
            sem_getvalue(&students_inside, &students);
        }
       // sem_wait(&enter);
        dean_enter();
        if (students == 0) search();
        else if (students == 10) breakup();
        sem_post(&leave);
        while (students > 0) {
            sem_getvalue(&students_inside, &students);
        }
        dean_leave();
        sem_post(&enter);
    }
    pthread_exit(0);
}

void *student(void *arg) {
    int id = *(int *)arg;
    // some code goes here
    usleep((rand() % 500 + 200) * 1000);
    sem_wait(&enter);
    sem_post(&students_inside);
    student_enter(id);
    sem_post(&enter);

    party(id);

    sem_wait(&leave);
    student_leave(id);
    sem_wait(&students_inside);
    sem_post(&leave);

    pthread_exit(0);
}

void init() {
    srand(time(NULL)); 
    sem_init(&enter, 0, 1);
    sem_init(&leave, 0, 1);
    sem_init(&students_inside, 0, 0);
    
    // some code goes here
}

void destroy() {
    sem_destroy(&enter);
    sem_destroy(&leave);
    sem_destroy(&students_inside);
    
    // some code goes here
}
