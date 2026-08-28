/*
    File: linear_search.cpp
    Course: CSCI 377 - Computer Algorithms
    Instructor: Avijit Roy

    Topic:
    Linear Search

    Why this matters for algorithms:
    --------------------------------
    Linear Search is one of the simplest searching algorithms.

    The algorithm begins with the first element and examines
    elements one at a time until:

    1. The target is found, or
    2. There are no more elements to examine.

    Unlike some faster searching techniques, Linear Search does
    not require the data to be sorted.

    What you should learn:
    ----------------------
    - How sequential searching works.
    - How an algorithm terminates when a solution is found.
    - How an algorithm reports failure when no solution exists.
    - How the location of a target affects the amount of work.
    - How to manually trace comparisons.

    Things to try yourself:
    -----------------------
    1. Search for the first value in the vector.

    2. Search for the last value in the vector.

    3. Search for a value that does not exist.

    4. Count how many comparisons happen in each case.

    5. Add duplicate values and determine which occurrence
       Linear Search returns.

    6. Increase the vector to 10, 100, or more values and think
       about how the amount of searching changes.

    Think about:
    ------------
    What is the best possible case?

    What is the worst possible case?

    If the vector contains n elements, how many elements might
    Linear Search need to inspect?
*/

#include <iostream>
#include <vector>

using namespace std;

/*
    Performs Linear Search.

    Parameters:
        data   - vector containing the values to search
        target - value we are trying to find

    Returns:
        Index of the first matching element if found.
        -1 if the target does not exist.
*/
int linearSearch(const vector<int>& data, int target)
{
    // Start at index 0 and examine each element.
    for (int i = 0; i < static_cast<int>(data.size()); ++i)
    {
        // Stop immediately when the target is found.
        if (data[i] == target)
        {
            return i;
        }
    }

    // The loop finished without finding the target.
    return -1;
}

int main()
{
    vector<int> values = {8, 3, 9, 4, 7, 2};

    cout << "Values: ";

    for (int value : values)
    {
        cout << value << " ";
    }

    cout << "\n\n";

    int target;

    cout << "Enter a value to search for: ";
    cin >> target;

    int result = linearSearch(values, target);

    if (result != -1)
    {
        cout << target
             << " found at index "
             << result
             << '\n';
    }
    else
    {
        cout << target
             << " was not found."
             << '\n';
    }

    return 0;
}