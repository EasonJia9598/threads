//
//  main.cpp
//  Calculating waiting time
//
//  Created by Zesheng Jia on 2018-10-09.
//  Copyright © 2018 Zesheng Jia. All rights reserved.
//

/************************************************************************************
 
 File:             calculate_waiting_time
 
 Description:      Using thread technique to calculating waiting time.
 
 Author:           Zesheng Jia A00416452
 
 *************************************************************************************/

#include <iostream>
#include <pthread.h>
#include <algorithm>    // std::sort
#include <vector>
#include <iomanip>



using namespace std;

// Global values
float FCFS_waiting_time = 0;
float SJF_waiting_time = 0;

// array container
vector<int> numbers;

/************************************************************************
 
 Function:        calculate_sum
 
 Description:     calculate sum from beginning to end_index
 
 *************************************************************************/

int calculate_sum(int end_index , vector<int> array) {
    
    float sum = 0;
    for (int i = 0 ; i < end_index; i++) {
        sum += array[i];
    }
    return sum;
}

/************************************************************************
 
 Function:        calculate_FCFS_waiting_time
 
 Description:     calculate FCFS CPU schedule waiting_time
 
 *************************************************************************/
void *calculate_FCFS_waiting_time(void *param) {
    
    float sum = 0;
    for (int i = 1; i < numbers.size(); i++) {
        sum += calculate_sum(i , numbers);
    }
    // get average value
    FCFS_waiting_time = sum / (float)numbers.size();;
    pthread_exit(0);
}

/************************************************************************
 
 Function:        calculate_SJF_waiting_time
 
 Description:     calculate SJF CPU schedule waiting_time

 *************************************************************************/
void *calculate_SJF_waiting_time(void *param) {
    
    float sum = 0;
    vector<int> temp = numbers;
    sort(temp.begin(), temp.end());
    
    for (int i = 1; i < temp.size(); i++) {
        sum += calculate_sum(i , temp);
    }
    // get average value
    SJF_waiting_time = sum / (float)numbers.size();
    pthread_exit(0);
}

/************************************************************************
 
 Function:        printValue
 
 Description:     print the 2 values
 
 *************************************************************************/
void printValue(){
    cout << "The average waiting time (FCFS) is " << FCFS_waiting_time << endl;
    cout << "The average waiting time (SJF) is " << fixed << setprecision(2) << SJF_waiting_time << endl;
}



int main(int argc, char *argv[]) {
    
    // save every arguments into vector
    for (int i = 1; i < argc; i++) {
        numbers.push_back(atoi(argv[i]));
    }
    
    
    // initialize 5 pthread
    pthread_t tid[2];
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    
    // start to create 5 pthread with specific functions
    pthread_create(&tid[0], &attr, calculate_SJF_waiting_time, NULL);
    pthread_create(&tid[0], &attr, calculate_FCFS_waiting_time, NULL);

    
    // join threads
    for (int i = 0; i < 2 ; i++) {
        pthread_join(tid[i], NULL);
    }
    
    printValue();
}

