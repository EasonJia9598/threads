//
//  main.cpp
//  DP
//
//  Created by WillJia on 2018-10-24.
//  Copyright © 2018 WillJia. All rights reserved.
//
#include <iostream>
#include <pthread.h>
#include <algorithm>    // std::sort
#include <vector>
#include <cmath>
#include <iomanip>
#include <string.h>
#include <semaphore.h>
#include <unistd.h>
#include <ctime>


using namespace std;
sem_t sem[5];
string state[5];

int randomTime(){
    return rand() % 3 + 1;
}
void think(int i ){
    printf("P %d is thinking\n" , i);
    state[i - 1] = "thinking";
    int time = randomTime();
//    printf("   %d \n" , time);
    sleep(time);
}

void eat(int i ){
    printf("P %d is eating\n" , i);
    state[i - 1] = "eating";
    int time = randomTime();
//    printf("   %d \n" , time);
    sleep(time);
    
}

void *philosopher(void *param ){
    int* a = (int*)(param);
    int i = *a;
    while (1){
        sem_wait(&sem[i]);
        sem_wait(&sem[(i+1) % 5]);
        eat(i);
        sem_post(&sem[i]);
        sem_post(&sem[(i+1) % 5]);
        think(i);
    }

}

void *printState(void *param){
    while (1) {
        for (int i = 0 ; i < 5; i++) {
            sem_wait(&sem[i]);
        }
        printf("[");
        for (int i = 0; i < 4; i++) {
            printf("P %d : %s , " , i + 1 , state[i].c_str());
        }
        printf("P %d : %s ]\n" , (int)5 , state[4].c_str());
        sleep(5);
        for (int i = 0 ; i < 5; i++) {
            sem_post(&sem[i]);
        }
    }
}


int main(int argc, const char * argv[]) {
    srand((int)time(NULL));

    for (int i = 0; i < 5; i++) {
        sem_init(&sem[i], 0, 0);
    }
    
    
    // initialize 5 pthread
    pthread_t tid[6];
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    // indicate id 5
    int id[5] = {1, 2, 3, 4, 5};
    // start to create 5 pthread with specific functions
    for (int i = 0; i < 5; i++) {
        pthread_create(&tid[i], &attr, philosopher, &id[i]);
    }
    // create new pthread for print
    pthread_create(&tid[5], &attr, printState, NULL);
    // join threads
    for (int i = 0; i < 6 ; i++) {
        pthread_join(tid[i], NULL);
    }
    
    return 0;
}
