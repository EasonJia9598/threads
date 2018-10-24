//
//  main.cpp
//  DP_Deadlock_Free
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

int randomTime(){
    return rand() % 3 + 1;
}

pthread_mutex_t mutexLock;
pthread_cond_t self[5];

struct DiningPhilosophers
{
    enum {THINKING, HUNGRY, EATING} state[5];
    string stateStr[3] = { "THINKING" , "HUNGRY" , "EATING"};
    
    void think(int i ){
        printf("P %d is thinking\n" , i);
        int time = randomTime();
        sleep(time);
    }
    
    void eat(int i ){
        printf("P %d is eating\n" , i);
        int time = randomTime();
        sleep(time);
        
    }
    
    void pickup(int i) {
        state[i] = HUNGRY;
        test(i);
        
        pthread_mutex_lock(&mutexLock);
        
        while(state[i] != EATING){
            pthread_cond_wait(&self[i],&mutexLock);
        }
        pthread_mutex_unlock(&mutexLock);

    }
    void putdown(int i) {
        state[i] = THINKING;
        test((i + 4) % 5);
        test((i + 1) % 5);
    }
    
    void test(int i) {
        if ((state[(i + 4) % 5] != EATING) && (state[i] == HUNGRY) && (state[(i + 1) % 5] != EATING)) {
            pthread_mutex_lock(&mutexLock);
            state[i] = EATING;
            pthread_cond_signal(&self[i]);
            pthread_mutex_unlock(&mutexLock);
        }
    }
    
    void initalize() {
        pthread_mutex_init(&mutexLock,NULL);
        for(int i = 0 ; i < 5 ; i++){
            pthread_cond_init(&self[i],NULL);
        }
        for (int i = 0; i < 5; i++)
            state[i] = THINKING;
    }
};

DiningPhilosophers team;


void *philosophers(void* param){
    int *a = (int*)param;
    int i = *a;
    while (1) {
        team.think(i);
        team.pickup(i);
        team.eat(i);
        team.putdown(i);
    }
}

void *printState(void *param){
    while (1) {
        printf("[");
        for (int i = 0; i < 4; i++) {
            printf("P %d : %s , " , i + 1 ,  team.stateStr[(int)team.state[i]].c_str());
        }
        printf("P %d : %s ]\n" , (int)5 , team.stateStr[(int)team.state[4]].c_str());
        sleep(2);
    }
}

int main(int argc, const char * argv[]) {
    srand((int)time(NULL));
    team.initalize();
    
    // initialize 5 pthread
    pthread_t tid[6];
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    // indicate id 5
    int id[5] = {1, 2, 3, 4, 5};
    // start to create 5 pthread with specific functions
    for (int i = 0; i < 5; i++) {
        pthread_create(&tid[i], &attr, philosophers, &id[i]);
    }
    // create new pthread for print
    pthread_create(&tid[5], &attr, printState, NULL);
    // join threads
    for (int i = 0; i < 6 ; i++) {
        pthread_join(tid[i], NULL);
    }
    
    return 0;
}
