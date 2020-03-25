/*
 *  COMP 15 Project 1 CalcYouLater
 *
 *  DatumStack.cpp
 *
 *  This file contains the implementation for the DatumStack class, containing
 *  constructors, the isEmpty, clear, size, top, pop and push functions.
 *
 *  Modified By (UTLN): Sean Lim (slim07)
 *           On       : 2/18/2020
 *
 */

#include <iostream>
#include <vector>
#include <string>
#include <stdexcept>

#include "DatumStack.h"

using namespace std;

/* Function DatumStack
* Parameters: none
* Returns: none
* Does: Default/nullery constructor */
DatumStack::DatumStack()
{
    length = 0;
};

/* Function DatumStack
* Parameters: Datum arr[], int arr_length - array of Datums, length of array
* Returns: none
* Does: Constructor that iterates through array of Datum objects and add them
to a DatumStack */
DatumStack::DatumStack(Datum arr[], int arr_length)
{
    for (int i = 0; i < arr_length; i++) {
        push(arr[i]);
    }
    length = arr_length;
};

/* Function isEmpty
* Parameters: none
* Returns: boolean value
* Does: lets us know if DatumStack is empty or not */
bool DatumStack::isEmpty()
{
    if (length == 0) {
        return true;
    } else {
        return false;
    }
};

/* Function void
* Parameters: none
* Returns: none
* Does: clears our DatumStack */
void DatumStack::clear() {
    for (int i = 0; i < length; i++) {
        stack.pop_back();
    }
    length = 0;
};

/* Function size
* Parameters: none
* Returns: an integer
* Does: Returns us the size of the DatumStack */
int DatumStack::size() {
    return length;
};

/* Function top
* Parameters: none
* Returns: a Datum object
* Does: Returns to us the Datum object at the top of the stack */
Datum DatumStack::top() {
    if (isEmpty()) {
        throw runtime_error("empty_stack");
    }
    return stack.at(size() - 1);
};

/* Function pop
* Parameters: none
* Returns: none
* Does: Removes Datum object from top of DatumStack */
void DatumStack::pop() {
    if (isEmpty()) {
        throw runtime_error("empty_stack");
    }
    stack.pop_back();
    length -= 1;
};

/* Function push
* Parameters: Datum toPush
* Returns: none
* Does: adds Datum object to the top of DatumStack */
void DatumStack::push(Datum toPush) {
    stack.push_back(toPush);
    length += 1;
};
