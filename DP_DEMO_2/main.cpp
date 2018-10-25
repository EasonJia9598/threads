/************************************************************************
 
 Author:        Zesheng Jia
 
 Description:   Dining Philosophers Problem with semaphores
 
 Ref: https://www.geeksforgeeks.org/operating-system-dining-philosopher-problem-using-semaphores/
 *************************************************************************/

#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <unistd.h>
#include <iostream>
#define N 5
#define THINKING 2
#define HUNGRY 1
#define EATING 0
#define LEFT (phnum + 4) % N
#define RIGHT (phnum + 1) % N

using namespace std;

int state[N] = {2, 2, 2, 2, 2};  // intialize all to thinking state
int phil[N] = { 0, 1, 2, 3, 4 }; // philoshper ID

sem_t semLock;
sem_t S[N];                      // N semaphores
string stateStr[3] = {"EATING", "HUNGRY" , "THINKING"} ;

/************************************************************************
 
 Function:        test
 
 Description:     test left side and right side's condition
 
 *************************************************************************/
void test(int phnum)
{
    // checking state
    if (state[phnum] == HUNGRY
        && state[LEFT] != EATING
        && state[RIGHT] != EATING) {
        // state that eating
        state[phnum] = EATING;
        sleep(2);
        printf("Philosopher %d is Eating\n", phnum + 1);
        // signal semaphores
        sem_post(&S[phnum]);
    }
}

/************************************************************************
 
 Function:        take_fork
 
 Description:     take left and right fork, parepare to eat
 
 *************************************************************************/
void take_fork(int phnum)
{
    // wait
    sem_wait(&semLock);
    // state that hungry
    state[phnum] = HUNGRY;
    printf("Philosopher %d is Hungry\n", phnum + 1);
    // eat if neighbours are not eating
    test(phnum);
    sem_post(&semLock);
    // if unable to eat wait to be signalled
    if(state[phnum] != EATING){
        sem_wait(&S[phnum]);
    }
    sleep(1);
}

/************************************************************************
 
 Function:        put_fork
 
 Description:     putdown fork and begin thinking
 
 *************************************************************************/
void put_fork(int phnum)
{
    sem_wait(&semLock);
    // state that thinking
    state[phnum] = THINKING;
    test(LEFT);
    test(RIGHT);
    // signal lock
    sem_post(&semLock);
}

/************************************************************************
 
 Function:        philospher
 
 Description:     main funciton for philosphers
 
 *************************************************************************/
void* philospher(void* num)
{
    while (1) {
        int* i = (int*)num;
        sleep(1);
        take_fork(*i);
        sleep(0);
        put_fork(*i);
    }
}

/************************************************************************
 
 Function:        printState
 
 Description:     print philosphers' states
 
 *************************************************************************/

void *printState(void *param){
    while (1) {
        for (int i = 0 ; i < 5; i++) {
            sem_wait(&S[i]);
        }
        printf("[");
        for (int i = 0; i < 4; i++) {
            printf("P %d : %s , " , i + 1 , stateStr[state[i]].c_str());
        }
        
        printf("P %d : %s ]\n" , (int)5 , stateStr[state[4]].c_str());
        sleep(2);
        for (int i = 0 ; i < 5; i++) {
            sem_post(&S[i]);
        }
    }
}

int main()
{
    
    int i;
    pthread_t thread_id[N + 1];
    // initialize the semaphores
    sem_init(&semLock, 0, 1);
    for (i = 0; i < N; i++)
        sem_init(&S[i], 0, 0);
    
    for (i = 0; i < N; i++) {
        // create philosopher processes
        pthread_create(&thread_id[i], NULL,
                       philospher, &phil[i]);
        printf("Philosopher %d is thinking\n", i + 1);
    }
    pthread_create(&thread_id[N], NULL,
                   printState, NULL);
    for (i = 0; i < N + 1; i++)
        pthread_join(thread_id[i], NULL);
}
