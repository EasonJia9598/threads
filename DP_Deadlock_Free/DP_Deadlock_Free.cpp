//
//  main.cpp
//  DP_Deadlock_Free
//
//  Created by Zesheng Jia on 2018-10-24.
//  Copyright © 2018 Zesheng Jia. All rights reserved.
//

/********************************************************************************
 
 Author:          Zesheng Jia (A00416452)
 
 Description:     Dining Philosophers Problem with monitor and condional variable
 
 ********************************************************************************/

#include <iostream>
#include <pthread.h>
#include <string.h>
#include <semaphore.h>
#include <unistd.h>
#include <ctime>

using namespace std;


pthread_mutex_t mutexLock;
pthread_cond_t self[5];

/************************************************************************
 
 Function:        randomTime
 
 Description:     get randomTime
 
 *************************************************************************/
int randomTime(){
    return rand() % 3 + 1;
}

/************************************************************************
 
 strcut:        DiningPhilosophers
 
 Description:   containing all functions of Dining Problem
 
 *************************************************************************/
struct DiningPhilosophers
{
    enum {THINKING, HUNGRY, EATING} state[5];
    string stateStr[3] = { "THINKING" , "HUNGRY" , "EATING"};
    
    /************************************************************************
     
     strcut:        think
     
     Description:   start thinking
     
     *************************************************************************/
    void think(int i ){
        printf("P %d is thinking\n" , i);
        int time = randomTime();
        sleep(time);
    }
    
    /************************************************************************
     
     strcut:        eat
     
     Description:   starting eating
     
     *************************************************************************/
    void eat(int i ){
        printf("P %d is eating\n" , i);
        int time = randomTime();
        sleep(time);
        
    }
    
    /************************************************************************
     
     strcut:        pickup
     
     Description:   pickup forks
     
     *************************************************************************/
    void pickup(int i) {
        state[i] = HUNGRY;
        test(i);
        
        pthread_mutex_lock(&mutexLock);
        
        if(state[i] != EATING){
            printf("P %d is Hungry\n" , i);
            pthread_cond_wait(&self[i],&mutexLock);
        }
        pthread_mutex_unlock(&mutexLock);

    }
    
    /************************************************************************
     
     strcut:        putdown
     
     Description:   putdown forks
     
     *************************************************************************/
    void putdown(int i) {
        state[i] = THINKING;
        test((i + 4) % 5);
        test((i + 1) % 5);
    }
    
    /************************************************************************
     
     strcut:        test
     
     Description:   test others condition
     
     *************************************************************************/
    void test(int i) {
        pthread_mutex_lock(&mutexLock);
        if ((state[(i + 4) % 5] != EATING) && (state[i] == HUNGRY) && (state[(i + 1) % 5] != EATING)) {
            state[i] = EATING;
            pthread_cond_signal(&self[i]);
        }
        pthread_mutex_unlock(&mutexLock);

    }
    
    /************************************************************************
     
     strcut:        initialize
     
     Description:   initialize state and locks
     
     *************************************************************************/
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

/************************************************************************
 
 strcut:        philosophers
 
 Description:   philosophers' main function
 
 *************************************************************************/
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

/************************************************************************
 
 strcut:        printState
 
 Description:   print every state
 
 *************************************************************************/
void *printState(void *param){
    while (1) {
        printf("[");
        for (int i = 0; i < 4; i++) {
            printf("P %d : %s , " , i + 1 ,  team.stateStr[(int)team.state[i]].c_str());
        }
        printf("P %d : %s ]\n" , (int)5 , team.stateStr[(int)team.state[4]].c_str());
        sleep(3);
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
