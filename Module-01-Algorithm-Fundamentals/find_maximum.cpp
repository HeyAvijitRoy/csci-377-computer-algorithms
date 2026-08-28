/*
    File: find_maximum.cpp
    Course: CSCI 377 - Computer Algorithms
    Instructor: Avijit Roy

    Topic:
    Finding the maximum value in a vector using a sequential scan.

    Why this matters for algorithms:
    --------------------------------
    This is a simple example of an algorithm that processes every element in a collection.

    To find the maximum value, the algorithm:

    1. Assumes the first element is the current maximum.
    2. Visits each remaining element one at a time.
    3. Compares each element with the current maximum.
    4. Updates the current maximum whenever a larger value is found.

    What you should learn:
    ----------------------
    - How an algorithm maintains a "current best" value.
    - How a loop can traverse a vector.
    - How comparisons affect the state of an algorithm.
    - How to manually trace an algorithm step by step.
    - Why finding a maximum requires examining all elements.

    Things to try yourself:
    -----------------------
    1. Change the numbers in the vector and predict the answer
       before running the program.

    2. Trace the value of currentMax after every iteration.

    3. Modify this algorithm to find the MINIMUM value instead.

    4. Try vectors where:
       - the maximum is the first element,
       - the maximum is the last element,
       - the maximum appears more than once,
       - all values are negative.

    5. Count how many comparisons the algorithm performs as
       the vector becomes larger.

    Think about:
    ------------
    If there are n elements in the vector, how much work does
    this algorithm perform as n grows?
*/

#include <iostream>
#include <vector>

using namespace std;

/*
    Returns the largest integer stored in the vector.

    Assumption:
    The vector contains at least one element.
*/
int findMaximum(const vector<int>& values)
{
    // Start by assuming the first value is the maximum.
    int currentMax = values[0];

    // Examine every remaining element.
    for (int i = 1; i < static_cast<int>(values.size()); ++i)
    {
        // If we discover a larger value,
        // update our current maximum.
        if (values[i] > currentMax)
        {
            currentMax = values[i];
        }
    }

    // Gives us the largest value found in the vector.
    return currentMax;
}

int main()
{
    vector<int> values = {31, 41, 59, 26, 41, 58};

    cout << "Values: ";

    for (int value : values)
    {
        cout << value << " ";
    }

    cout << '\n';

    int maximum = findMaximum(values);

    cout << "Maximum: " << maximum << '\n';

    return 0;
}