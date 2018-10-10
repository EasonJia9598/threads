//
//  main.cpp
//  threads
//
//  Created by WillJia on 2018-10-09.
//  Copyright © 2018 WillJia. All rights reserved.
//

/************************************************************************************
 
 File:             thread_calculting
 
 Description:      Using thread technique to calculate by 5 threads.
 
 Author:           Zesheng Jia A00416452
 
 *************************************************************************************/

#include <iostream>
#include <pthread.h>
#include <algorithm>    // std::sort
#include <vector>
#include <cmath>
#include <iomanip>

using namespace std;

// Global values
float average_value = 0;
int minimum_value = 0;
int maximum_value = 0;
int median_value = 0;
float standard_deviation_value= 0;

// array container
vector<int> numbers;

/************************************************************************
 
 Function:        average
 
 Description:     get average number
 
 *************************************************************************/
void *average(void *param) {
    float sum = 0;
    for (int n : numbers) {
        sum += n;
    }
    // get average value
    average_value = sum / (float)numbers.size();
    pthread_exit(0);
}

/************************************************************************
 
 Function:        minimum
 
 Description:     get minimum number
 
 *************************************************************************/
void *minimum(void *param) {
    int minimum = numbers[0];
    for (int n : numbers){
        if (n < minimum) {
           minimum = n;
        }
    }
    
    // get minimum value
    minimum_value = minimum;
    pthread_exit(0);
}

/************************************************************************
 
 Function:        maximum
 
 Description:     get maximum number
 
 *************************************************************************/
void *maximum(void *param) {
    int maximum = 0;
    for (int n : numbers){
        if (n > maximum ) {
            maximum = n;
        }
    }
    
    // get minimum value
    maximum_value = maximum;
    pthread_exit(0);
}

/************************************************************************
 
 Function:        median
 
 Description:     get median number
 
 *************************************************************************/
void *median(void *param) {
    unsigned long size = numbers.size();
    
    vector<int> temp = numbers;
    sort(temp.begin(), temp.end());
    
    median_value = temp[size / 2];
    pthread_exit(0);
}

/************************************************************************
 
 Function:        standard_deviation
 
 Description:     get standard deviation number
 
 *************************************************************************/
void *standard_deviation(void *param) {
    float sum = 0.0, mean, standardDeviation = 0.0;
    
    for(int n : numbers)
    {
        sum += n;
    }
    
    mean = sum/numbers.size();
    
    for(int n : numbers)
        standardDeviation += pow(n - mean, 2);
    standard_deviation_value = sqrt(standardDeviation / numbers.size());
    
    pthread_exit(0);
}

/************************************************************************
 
 Function:        printValue
 
 Description:     print the 5 values
 
 *************************************************************************/
void printValue(){
    cout << "The program will report" << endl;
    cout << "The average value is " << fixed << setprecision(2) << average_value << endl;
    cout << "The minimum value is " << minimum_value << endl;
    cout << "The maximum value is " << maximum_value << endl;
    cout << "The median value is " << median_value  << endl;
    cout << "The standard deviation is "<< standard_deviation_value << endl;
    
}




int main(int argc, char *argv[]) {
    
    // save every arguments into vector
    for (int i = 1; i < argc; i++) {
        numbers.push_back(atoi(argv[i]));
    }
    
    
    // initialize 5 pthread
    pthread_t tid[5];
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    
    // start to create 5 pthread with specific functions
    pthread_create(&tid[0], &attr, average, NULL);
    pthread_create(&tid[0], &attr, minimum, NULL);
    pthread_create(&tid[0], &attr, maximum, NULL);
    pthread_create(&tid[0], &attr, median, NULL);
    pthread_create(&tid[0], &attr, standard_deviation, NULL);
    
    // join threads
    for (int i = 0; i < 5 ; i++) {
        pthread_join(tid[i], NULL);
    }
    
    printValue();
}

// Demo output
/*
 90 81 78 95 79 72 85
 
 (./program 90 81 78 95 79 72 85)
 The program will report
 The average value is 82.86
 The minimum value is 72
 The maximum value is 95
 The median value is 81
 The standard deviation is 7.20
 */
