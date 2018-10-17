//
//  main.cpp
//  Calculating waiting time
//
//  Created by Zesheng Jia on 2018-10-09.
//  Copyright © 2018 Zesheng Jia. All rights reserved.
//

/************************************************************************************
 
 File:             Matrice addition
 
 Description:      Using thread technique to add two matrice
 
 Author:           Zesheng Jia A00416452
 
 *************************************************************************************/

#include <iostream>
#include <pthread.h>
#include <vector>
#include <string.h>
#include <fstream>
#include <sstream>


using namespace std;

#define SIZE 6



struct initMatrix{
    string filename;
    // array container
    vector<vector<int>> array;
};

struct additionMatrix{
    // calculate No. index of row
    int indexOfRow;
    // array container
    vector<int> matrix1;
    vector<int> matrix2;
    initMatrix *result;
    
};

// number of rows
int numberOfRow;

/************************************************************************
 
 Tool Functions
 
 *************************************************************************/


/************************************************************************
 
 Function:        split
 
 Description:     spilt line into elements
 
 *************************************************************************/


std::vector<std::string> &split(const std::string &s, char delim, std::vector<std::string> &elems) {
    std::stringstream ss(s);
    std::string item;
    while(std::getline(ss, item, delim)) {
        elems.push_back(item);
    }
    return elems;
}
// https://stackoverflow.com/questions/275404/splitting-strings-in-c



/************************************************************************
 
 Function:        arrayConvert
 
 Description:     convert string to array
 
 *************************************************************************/

vector<int> arrayConvert(string content){
    
    vector<int> array;
    vector<string> numbers ;
    split(content, ' ', numbers);
    int i = 0;
    for(string n : numbers){
        stringstream geek(n);
        geek >> i;
        array.push_back(i);
    }
    return array;
}
//https://www.geeksforgeeks.org/converting-strings-numbers-cc/

/************************************************************************
 
 Function:        readContent
 
 Description:     read content from file and change to 2D array
 
 *************************************************************************/

vector <vector<int>> readContent(string filename){
    string content;
    string line;
    vector <vector<int>> array;
    int count = 0;
    
    ifstream myfile (filename);
    if (myfile.is_open()) {
        while (getline(myfile,line)) {
            count++;
            array.push_back(arrayConvert(line));
        }
    }
    numberOfRow = count;
    myfile.close();
    return array;
}

/************************************************************************
 
 Function:        processFile
 
 Description:     read file and store in array
 
 *************************************************************************/

vector <vector<int>> processFile(string filename){
    return readContent(filename);
}

/************************************************************************
 
 Thread Functions
 
 *************************************************************************/


/************************************************************************
 
 Function:        calculateMatrix
 
 Description:     calculate two matrices
 
 *************************************************************************/

void *calculateMatrix(void *param) {
    additionMatrix *m1 = (additionMatrix*)param;
    int index = m1->indexOfRow;
    vector<int> row1 = m1->matrix1;
    vector<int> row2 = m1->matrix2;
    
    for (int i = 0; i < row1.size(); i++) {
        m1->result->array[index][i] = row2[i] + row1[i];
    }
    pthread_exit(0);
    
}


/************************************************************************
 
 Function:        obtainMatrix
 
 Description:     obtain Matrix
 
 *************************************************************************/
void *obtainMatrix(void *param) {
    initMatrix *ma = (initMatrix*)param; // change void to stuct
    ma->array = processFile(ma->filename);
    pthread_exit(0);
}


/************************************************************************
 
 Function:        printValue
 
 Description:     print the 2 values
 
 *************************************************************************/
void printValue(initMatrix matrice){
    cout << "The Resulting matrix  is " << endl;
    for(vector<int> arr : matrice.array){
        for(int n : arr){
            cout << n << " ";
        }
        cout << endl;
    }
}



int main(int argc, char *argv[]) {

    initMatrix matrice[2];
    additionMatrix additionMatrix[4];
    
    // initialize 6 pthread
    pthread_t tid[SIZE];
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    
    matrice[0].filename = "/Users/WillJia/Desktop/IOS Lecture/Projects/threads/matrixCal/file1.txt";
    matrice[1].filename = "/Users/WillJia/Desktop/IOS Lecture/Projects/threads/matrixCal/file2.txt";

    // start to create 2 pthread to obtain matrix
    pthread_create(&tid[0], &attr, obtainMatrix, &matrice[0]);
    pthread_create(&tid[1], &attr, obtainMatrix, &matrice[1]);
    
    // join threads
    for (int i = 0; i < 2 ; i++) {
        pthread_join(tid[i], NULL);
    }

    
    initMatrix result = matrice[1];

    for (int i = 0; i < numberOfRow; i++) {
        additionMatrix[i].indexOfRow = i;
        additionMatrix[i].matrix1 = matrice[0].array[i];
        additionMatrix[i].matrix2 = (matrice[1].array[i]);
        additionMatrix[i].result = &result;
    }
    

    
    

//    // start to create 4 pthreads to calculate two matrice's addition
    pthread_create(&tid[2], &attr, calculateMatrix, &additionMatrix[0]);
    pthread_create(&tid[3], &attr, calculateMatrix, &additionMatrix[1]);
    pthread_create(&tid[4], &attr, calculateMatrix, &additionMatrix[2]);
    pthread_create(&tid[5], &attr, calculateMatrix, &additionMatrix[3]);
//
//
    
    // join threads
    for (int i = 2; i < 6 ; i++) {
        pthread_join(tid[i], NULL);
    }
    
    printValue(result);
}

