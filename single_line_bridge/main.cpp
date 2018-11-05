//
//  main.cpp
//  single_line_brige
//
//  Created by WillJia on 2018-10-31.
//  Copyright © 2018 WillJia. All rights reserved.
//

#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <unistd.h>
#include <iostream>
#include <unistd.h>

// semaphore
sem_t bridge_avaliabe;

// framers
int north_counter = 12;
int south_counter = 12;

using namespace std;

/************************************************************************
 
 strcut:        south
 
 Description:   south village framers' action
 
 *************************************************************************/
void *south(void *param){
    
    while(1){
        //lock bridge
        sem_wait(&bridge_avaliabe);
        south_counter -= 1;
        //unlock bridge
        sem_post(&bridge_avaliabe);
        sleep(rand() % 3 + 1);
        if(south_counter == 0) break;
    }
    pthread_exit(NULL);
}

/************************************************************************
 
 strcut:        north
 
 Description:   north village framers' action
 
 *************************************************************************/
void *north(void *param){
    while(1){
        //lock bridge
        sem_wait(&bridge_avaliabe);
        north_counter -= 1;
        //unlock bridge
        sem_post(&bridge_avaliabe);
        sleep(rand() % 3 + 1);
        if(north_counter == 0) break;
    }
    pthread_exit(NULL);
}

/************************************************************************
 
 strcut:        printState
 
 Description:   print framers state
 
 *************************************************************************/
void *printState(void *param){
    while (1) {
        sem_wait(&bridge_avaliabe);
        printf("[South: %d left, North: %d left] \n" , south_counter , north_counter);
        sem_post(&bridge_avaliabe);
        sleep(1);
        if (north_counter == 0 && south_counter == 0) {
            printf("[South: %d left, North: %d left] \n" , south_counter , north_counter);
            break;
        }
    }
    pthread_exit(NULL);
}


int main(int argc, const char * argv[]) {
    srand((int)time(NULL));

    // initialize semaphore
    sem_init(&bridge_avaliabe, 0, 0);
    
    // initialize threads
    pthread_t tid[3];
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    
    pthread_create(&tid[0], &attr, south, NULL);
    pthread_create(&tid[0], &attr, north, NULL);
    pthread_create(&tid[0], &attr, printState, NULL);
    
    
    for (int i = 0; i < 3; i++) {
        pthread_join(tid[i],NULL);
    }
    return 0;
}
