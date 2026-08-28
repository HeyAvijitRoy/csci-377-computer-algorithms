# Module 01 — Algorithm Fundamentals

This module introduces foundational ideas used throughout **CSCI 377: Computer Algorithms**.

The examples in this folder focus on understanding how an algorithm processes data step by step, how to trace execution manually, and how the amount of work performed by an algorithm can vary depending on the input.

---

## Topics Covered

This module includes:

* Introduction to algorithms
* Step-by-step algorithm execution
* Sequential traversal
* Working with vectors
* Finding a maximum value
* Linear Search
* Manual algorithm tracing
* Comparisons and updates
* Best-case and worst-case thinking
* Connecting source code to algorithm behavior

---

## Files

```text
Module-01-Algorithm-Fundamentals/
│
├── README.md
├── find_maximum.cpp
└── linear_search.cpp
```

---

## `find_maximum.cpp`

This example demonstrates how an algorithm scans through a vector and maintains a current maximum value.

The basic idea is:

1. Assume the first element is the current maximum.
2. Examine each remaining element.
3. Compare the current element with the current maximum.
4. Update the current maximum when a larger value is found.
5. Return the final maximum value.

Students should be able to explain:

* why the first element is used for initialization,
* what `currentMax` represents,
* when `currentMax` changes,
* how many elements are examined,
* and why the algorithm must inspect the entire vector.

---

## Tracing `findMaximum()`

Tracing means following the algorithm manually and recording how important values change.

Consider:

```text
Index:  0   1   2   3   4   5
Value: 31  41  59  26  41  58
```

The algorithm begins with:

```text
currentMax = 31
```

A possible trace is:

| Index | Value Examined | Current Maximum Before | Comparison | Current Maximum After |
| ----: | -------------: | ---------------------: | ---------- | --------------------: |
|     0 |             31 |                      — | Initialize |                    31 |
|     1 |             41 |                     31 | 41 > 31    |                    41 |
|     2 |             59 |                     41 | 59 > 41    |                    59 |
|     3 |             26 |                     59 | 26 > 59    |                    59 |
|     4 |             41 |                     59 | 41 > 59    |                    59 |
|     5 |             58 |                     59 | 58 > 59    |                    59 |

Final result:

```text
Maximum = 59
```

The important part of tracing is not only knowing the final answer, but understanding **when and why the value changes**.

---

## Practice with `findMaximum()`

Try changing the vector and predicting the result before running the program.

Test cases should include situations where:

* the maximum is the first element,
* the maximum is the last element,
* the maximum appears more than once,
* all values are negative,
* values are already sorted,
* and values are in reverse order.

Also count how many comparisons are performed.

---

## Challenge: Find the Minimum

After understanding `findMaximum()`, try adapting the same idea to find the smallest value.

Create a function named:

```cpp
int findMinimum(const vector<int>& values)
```

Think about:

* What should the initial value be?
* What variable should replace `currentMax`?
* What comparison operator should change?
* When should the current minimum be updated?

Do not focus only on changing the code.

Be able to explain why the modified algorithm correctly finds the minimum value.

---

## `linear_search.cpp`

This example demonstrates **Linear Search**.

Linear Search examines elements one at a time from the beginning of a collection until:

* the target value is found, or
* there are no more elements to examine.

The function returns the index of the first matching value.

If the target does not exist, the function returns:

```text
-1
```

---

## How Linear Search Works

Suppose the vector is:

```text
Index:  0  1  2  3  4  5
Value:  8  3  9  4  7  2
```

If the target is:

```text
7
```

Linear Search checks:

```text
8 → 3 → 9 → 4 → 7
```

The target is found at:

```text
index 4
```

The algorithm stops as soon as the target is found.

---

## Tracing Linear Search

For a target of `7`:

| Index | Value Examined | Target | Match? |
| ----: | -------------: | -----: | ------ |
|     0 |              8 |      7 | No     |
|     1 |              3 |      7 | No     |
|     2 |              9 |      7 | No     |
|     3 |              4 |      7 | No     |
|     4 |              7 |      7 | Yes    |

The function returns:

```text
4
```

---

## Searching for a Missing Value

Suppose the target is:

```text
10
```

The algorithm checks:

```text
8 → 3 → 9 → 4 → 7 → 2
```

Because `10` does not exist in the vector, the algorithm must examine every element.

The function returns:

```text
-1
```

---

## Linear Search Practice

Test the algorithm using different target positions.

### Case 1 — Target Near the Beginning

Search for a value located near index `0`.

Observe how quickly the algorithm stops.

### Case 2 — Target Near the End

Search for a value located near the end of the vector.

Compare the number of comparisons with Case 1.

### Case 3 — Target Not Present

Search for a value that does not exist.

Observe that Linear Search must examine every element.

### Case 4 — Duplicate Values

Add the same value more than once.

Determine which index Linear Search returns.

Think about why the algorithm behaves that way.

---

## Best-Case and Worst-Case Thinking

Linear Search provides a simple example of how the amount of work performed by an algorithm can depend on the input.

### Best Case

The target is the first element.

Only one comparison is needed.

### Worst Case

The target is:

* the final element, or
* not present at all.

The algorithm may need to examine the entire vector.

As the number of elements grows, the amount of work required by Linear Search can also grow.

We will examine this idea more formally as we begin analyzing algorithm efficiency.

---

## Student Practice

After reviewing both examples, students should be able to:

1. Trace `findMaximum()` manually.
2. Explain how `currentMax` changes.
3. Create a `findMinimum()` function.
4. Trace a minimum-finding algorithm.
5. Perform Linear Search manually.
6. Identify the index where a target is found.
7. Explain what happens when a target is missing.
8. Compare early, late, and unsuccessful searches.
9. Count the number of comparisons.
10. Explain why different inputs may require different amounts of work.

---

## Recommended Testing

Do not run only one test case.

For each algorithm:

1. Predict the result.
2. Trace the algorithm manually.
3. Run the program.
4. Compare the result with your prediction.
5. Change the input.
6. Repeat the process.

Testing multiple cases helps reveal how an algorithm behaves beyond a single example.

---

## Key Questions

Before moving forward, make sure you can answer:

* What makes a procedure an algorithm?
* Why does `findMaximum()` examine every element?
* What information must be maintained while finding a maximum?
* How could the same structure be used to find a minimum?
* How does Linear Search examine data?
* Why can Linear Search stop early?
* When must Linear Search examine the entire vector?
* How does the location of the target affect the number of comparisons?
* What happens when duplicate values exist?
* How can tracing help us understand an algorithm?

---

## Important

The purpose of these examples is to understand the **algorithm, its execution, and its behavior**.

Do not simply copy or memorize the source code.

You should be able to:

* trace it,
* explain it,
* modify it,
* test it,
* and reason about how much work it performs.

Official assignments, quizzes, readings, deadlines, and submission instructions are posted in **Brightspace**.
