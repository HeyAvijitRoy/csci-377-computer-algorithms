// CSCI 377: Computer Algorithms — Module 2
// Analyzing and Designing Algorithms
// Instructor: Avijit Roy
// John Jay College of Criminal Justice, CUNY
// Fall 2026
//
// Teaching implementation by Avijit Roy.
// Algorithm structure follows CLRS, Introduction to Algorithms, 4th ed.
// This version is instrumented for classroom tracing: it prints the array,
// key-comparison count, and shift count after every pass.
//

#include <iostream>
#include <string>
#include <vector>

void printArray(const std::vector<int>& A, const std::string& label = "") {
    std::cout << label;
    for (int x : A) {
        std::cout << " " << x;
    }
    std::cout << '\n';
}

void insertionSort(std::vector<int>& A) {
    const int n = static_cast<int>(A.size());

    long long totalKeyComparisons = 0;
    long long totalShifts = 0;

    // CLRS pseudocode uses 1-based indexing:
    //   for i = 2 to n
    //
    // C++ vectors use 0-based indexing:
    //   for (int i = 1; i < n; ++i)
    for (int i = 1; i < n; ++i) {
        // key is the value being inserted this pass. We save it in its own
        // variable BEFORE touching A, because A[i] is about to be
        // overwritten by shifts below.
        const int key = A[i];

        // j starts at the last index of the sorted prefix, immediately to
        // key's left, and walks leftward while it finds larger values.
        int j = i - 1;

        // Reset every pass: these count only this pass's work, separate
        // from the running totals below.
        int passKeyComparisons = 0;
        int passShifts = 0;

        // IMPORTANT:
        // We count "key comparisons" as evaluations of A[j] > key.
        // This is the same definition used on the Module 2 tracing worksheet.
        //
        // j >= 0 is a boundary check. Because && short-circuits, A[j] > key
        // is evaluated only when j is still a valid index.
        while (j >= 0) {
            ++passKeyComparisons;
            ++totalKeyComparisons;

            // A[j] is already <= key, so the gap for key is at j + 1.
            // Nothing left of j needs to move. This is the line that stops
            // the loop early on sorted or nearly-sorted input.
            if (A[j] <= key) {
                break;
            }

            // A[j] is larger than key, so it moves one slot right to open
            // a gap. This overwrites A[j + 1], which is safe because that
            // value was either already copied to key, or already shifted
            // right on a previous iteration of this same pass.
            A[j + 1] = A[j];   // shift right; this is NOT a swap
            ++passShifts;
            ++totalShifts;
            --j;   // move the boundary one step left and re-check
        }

        // The loop above exits two ways: A[j] <= key (break), or j has run
        // past index 0 (while condition false). Either way, j + 1 is
        // exactly the gap left behind by the shifts, so key belongs there.
        A[j + 1] = key;

        // Printed as i + 1 to match the worksheet's 1-based pass numbering
        // (CLRS pass "i = 2" is this loop's i = 1).
        std::cout << "pass i=" << (i + 1)
                  << "  key=" << key
                  << "  key comparisons=" << passKeyComparisons
                  << "  shifts=" << passShifts
                  << "  ->";
        printArray(A);
    }

    std::cout << "\nTotals: key comparisons=" << totalKeyComparisons
              << ", shifts=" << totalShifts << "\n";
}

int main() {
    std::vector<int> A = {8, 2, 4, 9, 3, 6};

    printArray(A, "Input: ");
    std::cout << '\n';

    // A is passed by reference (std::vector<int>&), so insertionSort sorts
    // this exact array in place. The "Sorted:" print below shows the same
    // A, now mutated.
    insertionSort(A);

    std::cout << '\n';
    printArray(A, "Sorted:");

    return 0;
}
