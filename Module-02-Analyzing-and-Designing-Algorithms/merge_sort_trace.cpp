// CSCI 377: Computer Algorithms — Module 2
// Analyzing and Designing Algorithms
// Instructor: Avijit Roy
// John Jay College of Criminal Justice, CUNY
// Fall 2026
//
// Teaching implementation by Avijit Roy.
// Algorithm structure follows CLRS, Introduction to Algorithms, 4th ed.
// Indentation visualizes recursion depth. The program also counts key
// comparisons and writes performed by MERGE.
//

#include <iostream>
#include <string>
#include <vector>

// File-scope counters purely for classroom instrumentation: they total
// work across the ENTIRE run of mergeSort, not just one call. This is not
// idiomatic style for production code — a real function wouldn't reach for
// globals here — it's done this way so main() can print one final total
// without threading extra parameters through every recursive call.
static long long mergeWrites = 0;
static long long keyComparisons = 0;

// Prints one line describing the current subarray A[p:r], indented by
// 3 spaces per recursion level. Printing this at the top of mergeSort
// (tag "split"), at its base case (tag "base "), and after merge() runs
// (tag "merge") turns the call sequence into a readable, sideways
// recursion tree: deeper recursive calls print further to the right.
void show(const std::vector<int>& A,
          int p,
          int r,
          int depth,
          const std::string& tag) {
    std::cout << std::string(depth * 3, ' ')
              << tag << " A[" << p << ":" << r << "] =";

    for (int k = p; k <= r; ++k) {
        std::cout << " " << A[k];
    }
    std::cout << '\n';
}

void merge(std::vector<int>& A, int p, int q, int r) {
    // L and R are COPIES of the two already-sorted halves A[p:q] and
    // A[q+1:r], not views into A. That copy is what makes it safe for the
    // loops below to overwrite A[p..r] in place: we're never reading a
    // position in A that we still need after it's been written.
    std::vector<int> L(A.begin() + p,     A.begin() + q + 1);
    std::vector<int> R(A.begin() + q + 1, A.begin() + r + 1);

    // Three pointers: i walks L, j walks R, k is the next position in A
    // to fill. Only i and j ever get compared to each other; k only writes.
    int i = 0;
    int j = 0;
    int k = p;

    // Because L and R are each individually sorted, the smallest value not
    // yet placed into A must be at the FRONT of one of the two lists. So we
    // never need to look past L[i] or R[j] — comparing just those two
    // "front" elements is enough to know which one goes into A next.
    while (i < static_cast<int>(L.size()) &&
           j < static_cast<int>(R.size())) {

        ++keyComparisons;   // this is the comparison MERGE performs

        // <= preserves stability: equal values from the left half stay first.
        if (L[i] <= R[j]) {
            A[k++] = L[i++];   // take from L: write A[k], then advance k and i
        } else {
            A[k++] = R[j++];   // take from R: write A[k], then advance k and j
        }
        ++mergeWrites;
    }

    // One side is empty. Because that side ran out first, EVERY value still
    // in the other side is already >= everything already written into A,
    // and the remaining values are already in sorted order relative to each
    // other — so they can be copied over directly with no more comparisons.
    while (i < static_cast<int>(L.size())) {
        A[k++] = L[i++];
        ++mergeWrites;
    }

    while (j < static_cast<int>(R.size())) {
        A[k++] = R[j++];
        ++mergeWrites;
    }
}

void mergeSort(std::vector<int>& A, int p, int r, int depth = 0) {
    // Base case: p >= r means A[p:r] has 0 or 1 elements, which is already
    // sorted by definition. This is what stops the recursion — no further
    // splitting or merging happens for a subarray this small.
    if (p >= r) {
        show(A, p, r, depth, "base ");
        return;
    }

    // Printed on the way DOWN, before dividing: shows the subarray as it
    // looks right before this call splits it in two.
    show(A, p, r, depth, "split");

    // Overflow-safe midpoint expression.
    const int q = p + (r - p) / 2;

    // depth + 1 is passed to both recursive calls so show() indents them
    // one level further right than this call — that growing indentation is
    // what makes the printed output read as a recursion tree.
    mergeSort(A, p,     q, depth + 1);   // recursively sort the left half
    mergeSort(A, q + 1, r, depth + 1);   // recursively sort the right half

    // Both halves are sorted at this point. Combine them back into A[p:r].
    merge(A, p, q, r);

    // Printed on the way UP, after combining: shows A[p:r] now fully
    // sorted, which is why the tag is "merge" rather than "split".
    show(A, p, r, depth, "merge");
}

int main() {
    std::vector<int> A = {8, 2, 4, 9, 3, 6};

    std::cout << "MERGE-SORT on A = 8 2 4 9 3 6\n\n";
    mergeSort(A, 0, static_cast<int>(A.size()) - 1);

    std::cout << "\nSorted:";
    for (int x : A) {
        std::cout << " " << x;
    }

    std::cout << "\n\nInstrumentation for this input:\n";
    std::cout << "  key comparisons inside MERGE: " << keyComparisons << '\n';
    std::cout << "  element writes into A:         " << mergeWrites << "\n\n";

    std::cout
        << "Important: Theta(n log n) describes the growth rate.\n"
        << "It does NOT mean that every implementation performs exactly\n"
        << "n*log2(n) primitive operations on every input.\n";

    return 0;
}
