//
//  main.cpp
//  Project5
//
//  Created by Sean McGunigal on 4/15/15.
//  Copyright (c) 2015 Sean McGunigal. All rights reserved.
//

#include <iostream>
#include <cstring>
#include <cstdlib>
#include <ctime>
#include <queue>
using namespace std;

// Function to generate a random string with a given seed
void randomString(char** array, int index, int seed);

// Function for bubble sort (which I didn't use because it took so long)
float bubbleSort(char** array, int numElements);

// Function for radix sort, which returns the time spent
float radixSort(char** array, int numElements);

// Function to make a deep copy of the strings in the array
char** deepCopy(char** array, int length);

// Function to copy only the pointers to the strings (not used)
char** shallowCopy(char** array, int length);

// Function to perform quicksort, which returns the time spent
float quickSort(char** array, int numElements);

// The recursive portion of the quicksort function
void quickSortAlg(char** array, int leftIndex, int rightIndex);

// My own partition function
int myPartition(char** array, int leftIndex, int rightIndex);

// My function to swap two elements in an array
void mySwap(char** array, int a, int b);

// My function to perform heap sort, which returns the time spent
float myHeapSort(char** array, int numElements);

// The recursive part of the heap sort algorithm
void heapSortAlg(char** array, int numElements);

// My own heapify function
void Heapify(char** array, int count);

// My own siftDown function
void siftDown(char** array, int start, int end);

// Main Function
int main(int argc, const char * argv[])
{
    // Run the loop for each size given in the project description
    for (int size = 10000; size <= 50000; size += 5000) {
        
        cout << "****************************************" << endl;
        cout << "* The array size is now " << size << " *" << endl;
        cout << "****************************************" << endl;
        
        float radixAverageTime = 0.0;
        float quickAverageTime = 0.0;
        float heapAverageTime = 0.0;
        
        for (int trial = 0; trial < 10; trial++) {
            
            // I gave each string a little extra space so as to avoid memory overflows
            char** testArray = new char*[size];
            for (int i = 0; i < size; i++) {
                testArray[i] = new char[15];
                for (int j = 0; j < 15; j++)
                    testArray[i][j] = '\0';
            }
            for (int i = 0; i < size; i++)
                randomString(testArray, i, i+1);
            
            // Copy the random array to be radix sorted
            char** radixArray = deepCopy(testArray, size);
            
            cout << "The first 10 strings of the unsorted array for radix sort are: " << endl;
            for (int i = 0; i < 10; i++)
                cout << radixArray[i] << endl;
            
            cout << "Sorting using radix sort!" << endl;
            float tempTime = radixSort(radixArray, size);
            
            radixAverageTime += tempTime;
            
            cout << "The first 10 elements of the sorted radix sort are:" << endl;
            for (int i = 0; i < 10; i++)
                cout << radixArray[i] << endl;
            
            // Copy the random array to be quicksorted
            char** quickArray = deepCopy(testArray, size);
            
            cout << "The first 10 strings of the unsorted array for quicksort are: " << endl;
            for (int i = 0; i < 10; i++)
                cout << quickArray[i] << endl;
            
            cout << "Sorting using quicksort!" << endl;
            tempTime = quickSort(quickArray, size);
            quickAverageTime += tempTime;
            
            cout << "The first 10 elements of the sorted quicksort are:" << endl;
            for (int i = 0; i < 10; i++)
                cout << quickArray[i] << endl;
            
            cout << "The first 10 strings of the unsorted array for heap sort are: " << endl;
            for (int i = 0; i < 10; i++)
                cout << testArray[i] << endl;
            
            // Perform the heap sort on the original test array (which is still random)
            cout << "Sorting using heap sort!" << endl;
            tempTime = myHeapSort(testArray, size);
            heapAverageTime += tempTime;
            
            cout << "The first 10 elements of the sorted heap sort are:" << endl;
            for (int i = 0; i < 10; i++)
                cout << testArray[i] << endl;
            
            // Set every character to null before deleting the arrays
            for (int i = 0; i < size; i++) {
                for (int j = 0; j < 15; j++) {
                    radixArray[i][j] = '\0';
                    quickArray[i][j] = '\0';
                    testArray[i][j] = '\0';
                }
            }
            
            // Delete the arrays
            delete [] radixArray;
            delete [] quickArray;
            delete [] testArray;
        }
        
        // The timing is used for the graphing
        cout << "The average time for the radix sort with " << size << " elements was " << (radixAverageTime/10.0) << " seconds." << endl;
        cout << "The average time for the quicksort with " << size << " elements was " << (quickAverageTime/10.0) << " seconds." << endl;
        cout << "The average time for the heapsort with " << size << " elements was " << (heapAverageTime/10.0) << " seconds." << endl;
        cout << endl;
    }
    
}

// Function which generates random strings based on both the time and the given seed, so as to ensure every element is different
// every time
void randomString(char** array, int index, int seed) {
    int length = 0;
    //char* output = new char[11];
    //for (int i = 0; i < 11; i++)
    //    output[i] = '\0';
    
    srand(seed*time(NULL));
    
    length = rand()%10 + 1;
    
    // Convert from a random int to an ascii character
    for (int i = 0; i < length; i++) {
        int value = rand()%62;
        if (value <= 9)
            array[index][i] = value + 48;
        else if (value >= 10 && value <= 35)
            array[index][i] = value + 55;
        else
            array[index][i] = value + 61;
    }
    //return output;
}

// Function for bubble sort, even though it wasn't used
float bubbleSort(char** array, int numElements) {
    clock_t time = clock();
    bool mistakes = true;
    while (mistakes) {
        mistakes = false;
        for (int j = 0; j < numElements-1; j++) {
            if (strcmp(array[j], array[j+1]) > 0) {
                char* temp = array[j];
                array[j] = array[j+1];
                array[j+1] = temp;
                mistakes = true;
            }
        }
    }
    // return the time spent
    time = clock() - time;
    return ((float)time/CLOCKS_PER_SEC);
}

// Function for radix sort
float radixSort(char** array, int numElements) {
    clock_t time = clock();
    queue<char*>* radix = new queue<char*>[65];
    for (int i = 9; i >= 0; i--) {
        for (int j = 0; j < numElements; j++) {
            if ((array[j])[i] == '\0')
                radix[0].push(array[j]);
            else {
                //if (array[j][i] != NULL)
                int value = array[j][i];
                if (value <= 57) {
                    if (value < 48) {
                        // I put this here to debug in case I ran out of memory or there was an overflow
                        value = 0;
                        (array[j])[i] = '\0';
                        //cout << "ERROR" << endl;
                    } else
                    value -= 47;
                } else if (value >=65 && value <= 90)
                    value -= 54;
                else if (value > 90)
                    value -= 60;
                radix[value].push(array[j]);
            }
        }
        int index = 0;
        for (int k = 0; k < 63; k++) {
            while (!radix[k].empty()) {
                array[index] = radix[k].front();
                radix[k].pop();
                index++;
            }
        }
    }
    
    delete [] radix;
    
    // Return the time spent
    time = clock() - time;
    return ((float)time/CLOCKS_PER_SEC);
}

// Function to copy each string fully
char** deepCopy(char** array, int length) {
    char** output = new char*[length];
    
    for (int i = 0; i < length; i++) {
        output[i] = new char[15];
        for (int j = 0; j < 15; j++)
            output[i][j] = '\0';
    }
    
    for (int i = 0; i < length; i++)
        strcpy(output[i], array[i]);
    
    return output;
}

// Function to copy the string pointers (not used)
char** shallowCopy(char** array, int length) {
    char** output = new char*[length];
    
    for (int i = 0; i < length; i++)
        output[i] = array[i];
    
    return output;
}

// Function to perform quick sort
float quickSort(char** array, int numElements) {
    clock_t time = clock();
    quickSortAlg(array, 0, numElements-1);
    time = clock() - time;
    // returns the time spent
    return ((float)time/CLOCKS_PER_SEC);
}

// Function called by QuickSort recursively
void quickSortAlg(char** array, int leftIndex, int rightIndex) {
    if (leftIndex < rightIndex) {
        int location = myPartition(array, leftIndex, rightIndex);
        quickSortAlg(array, leftIndex, location-1);
        quickSortAlg(array, location+1, rightIndex);
    }
}

//Partition function called by quickSortAlg
int myPartition(char** array, int leftIndex, int rightIndex) {
    int pivotPoint = leftIndex;
    char* pivotValue = array[pivotPoint];
    mySwap(array, pivotPoint, rightIndex);
    int storeIndex = leftIndex;
    
    for (int i = leftIndex; i < rightIndex; i++) {
        if (strcmp(array[i], pivotValue) < 0) {
            mySwap(array, i, storeIndex);
            storeIndex += 1;
        }
    }
    mySwap(array, storeIndex, rightIndex);
    return storeIndex;
}

// Function to swap two pointers, used in the myPartition function
void mySwap(char** array, int a, int b) {
    char* tempPointer = array[a];
    array[a] = array[b];
    array[b] = tempPointer;
}

// My heapsort function
float myHeapSort(char** array, int numElements) {
    clock_t time = clock();
    heapSortAlg(array, numElements);
    time = clock() - time;
    // returns the time spent
    return ((float)time/CLOCKS_PER_SEC);
}

// My heapsort algorithm, called by myHeapSort
void heapSortAlg(char** array, int numElements) {
    Heapify(array, numElements);
    
    int end = numElements - 1;
    while (end > 0) {
        mySwap(array, end, 0);
        end -= 1;
        siftDown(array, 0, end);
    }
}

// Function to heapify, called by heapSortAlg
void Heapify(char** array, int count) {
    int start = ((count - 2 ) / 2);
    while (start >= 0) {
        siftDown(array, start, count - 1);
        start -= 1;
    }
}

// SiftDown function, called by both heapify and heapSortAlg
void siftDown(char** array, int start, int end) {
    int root = start;
    while ((root * 2 + 1) <= end) {
        int child = root * 2 + 1;
        int swapMe = root;
        if (strcmp(array[swapMe], array[child]) < 0)
            swapMe = child;
        if ((child + 1 <= end) && (strcmp(array[swapMe], array[child+1]) < 0)) {
            swapMe = child + 1;
        }
        if (swapMe == root)
            return;
        else {
            mySwap(array, root, swapMe);
            root = swapMe;
        }
    }
}
