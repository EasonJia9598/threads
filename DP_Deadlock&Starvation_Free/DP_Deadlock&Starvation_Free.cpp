//
//  DP_Deadlock&Starvation_Free.cpp
//  DP_Deadlock&Starvation_Free
//
//  Created by Zesheng Jia on 2018-10-24.
//  Copyright © 2018 Zesheng Jia. All rights reserved.
//

/********************************************************************************
 
 Author:          Zesheng Jia (A00416452)
 
 Description:     DP_Deadlock & Starvation_Free
 
 ********************************************************************************/

#include<stdio.h>
#include<pthread.h>
#include<time.h>
#include<stdlib.h>
#include<unistd.h>
#include <string.h>
#include <iostream>

using namespace std;

 // number of eating times
int MAX_ITER = 200;
// number of philospers and forks
int num = 5;
// storing of average hungry time
double waiting_time_averages [ 5 ];
// philospher states
enum { thinking, hungry, eating } state [ 5 ];
// mutex for accessing philospher states to avoid deadlock
pthread_mutex_t mutextLock;
// condition variable for accessing
pthread_cond_t self [ 5 ];
pthread_t philo [ 6 ];
int identity [ 5 ] = { 0, 1, 2, 3, 4 };

string stateStr[3] = {"THINKING", "HUNGRY" , "EATING"} ;
int eatCount[5] = {0};


void *phil_run ( void *arg );
void pickup_forks ( int );
void return_forks ( int );
void test ( int );
void *printState(void *param);

int main ( )
{
    srand ( ( unsigned ) time ( NULL ) );
    // initialize mutextLock
    pthread_mutex_init ( &mutextLock, NULL );
    
    int i = 0;
    for ( i = 0; i < num; i++ ){                             // intialization
        state [ i ] = thinking;
        pthread_cond_init ( &self [ i ], NULL );
    }
    
    // thread identifier
    for ( i = 0; i < num; i++ ){
        // passing value to program
        // creating threads with default attributes and their number as
        pthread_create ( &philo [ i ], NULL, phil_run, &identity [i]);
    }
    
    pthread_create(&philo[5], NULL, printState, NULL);
    
    
    for ( i = 0; i < num + 1; i++ ){
        // waiting for thread termination sequentially
        pthread_join ( philo [ i ], NULL );
    }

    return 0;
}

/* phil_run allows each philospher to think and eat which we execute as seperate thread */

/************************************************************************
 
 strcut:        phil_run
 
 Description:   phil_run allows each philospher to think and eat which we execute as seperate thread
 
 *************************************************************************/
void *phil_run ( void *arg )
{
    while (1) {
        int *tmp_ptr = ( int *)arg;
        int id = *tmp_ptr;
        
        for ( int iter = 0; iter < MAX_ITER; iter++ )
        {
            // random thinking time
            int rand_time = ( rand())%2 + 1;
            sleep ( rand_time );
            pickup_forks ( id );                             // picking forks
            // random eating time
            int rnd_time = ( rand())%2 + 1;
            sleep ( rnd_time );
            return_forks ( id );                             // returning forks
        }
    }
    
}

/************************************************************************
 
 strcut:        pickup_forks
 
 Description:   function to pick forks
 
 *************************************************************************/
void pickup_forks ( int id )
{
    pthread_mutex_lock ( &mutextLock );                            // lock to access states
    state [ id ] = hungry;
    pthread_mutex_unlock ( &mutextLock );
    
    test ( id );
    
    pthread_mutex_lock ( &mutextLock );
    if ( state [ id ] != eating )
    {
        printf("P %d starts hungry ||  " , id + 1);
        pthread_cond_wait ( &self [ id ], &mutextLock );            // condition wait if forks not available
    }
    pthread_mutex_unlock ( &mutextLock );
}

/************************************************************************
 
 strcut:        return_forks
 
 Description:   function to release forks
 
 *************************************************************************/
void return_forks ( int id )
{
    pthread_mutex_lock ( &mutextLock );
    state [ id ] = thinking;
    printf("P %d starts thinking ||  " , id + 1);
    pthread_mutex_unlock ( &mutextLock );
    test ( ( id + num - 1 ) % num );                    // call to check for other philosphers to avoid starvation
    test ( ( id + 1 ) % num );
}

/************************************************************************
 
 strcut:        test
 
 Description:   function to allocate forks and avoid deadlocks
 
 *************************************************************************/
void test ( int id )
{
    int is_even = ( id % 2 == 0);                        // old implementation to avoid deadlock
    int num1 = ( id + num -1 ) % num;
    int num2 = ( id + 1 ) % num;
    if ( is_even )
    {
        
        pthread_mutex_lock( &mutextLock );
        if ( (state[num1] != eating) && (state[num2] != eating) && (state[id] == hungry) )
        {
            state[ id ] = eating;
            printf("P %d starts eating || " , id + 1);
            eatCount[id]++;
            pthread_cond_signal ( &self [ id ] );
        }
        pthread_mutex_unlock( &mutextLock );
    }
    else
    {
        pthread_mutex_lock( &mutextLock );
        if ( (state[num1] != eating) && (state[num2] != eating) && (state[id] == hungry) )
        {
            state[ id ] = eating;
            eatCount[id]++;
            printf("P %d starts eating || " , id + 1);
            pthread_cond_signal ( &self [ id ] );
        }
        pthread_mutex_unlock( &mutextLock );
    }
}


/************************************************************************
 
 strcut:        printState
 
 Description:   print every state
 
 *************************************************************************/
void *printState(void *param){
    while (1) {
        pthread_mutex_lock( &mutextLock );
        printf("\n\n[");
        for (int i = 0; i < 4; i++) {
            printf("P %d : %s , " , i + 1 ,  stateStr[(int)state[i]].c_str());
        }
        printf("P %d : %s ]\n[" , (int)5 , stateStr[(int)state[4]].c_str());
        
        for (int i = 0; i < 4; i++) {
            printf("P %d has eaten %d , " , i , eatCount[i]);
        }
        printf("P %d has eaten %d ]\n\n" , (int)5 , eatCount[4]);
        pthread_mutex_unlock( &mutextLock );

        sleep(2);
    }
}
