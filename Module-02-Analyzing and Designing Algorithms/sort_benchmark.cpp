// CSCI 377: Computer Algorithms — Module 2
// Insertion Sort vs. Merge Sort Benchmark
// Instructor: Avijit Roy
// John Jay College of Criminal Justice, CUNY
// Fall 2026
//
// Teaching benchmark by Avijit Roy.
// The purpose is to observe growth trends, not to claim universal timings.
//
// Notes:
// - A fixed random seed makes random inputs reproducible.
// - Each measurement uses a fresh copy of the same input.
// - One warm-up trial is discarded.
// - The median of five measured trials is reported.
// - Correctness is checked outside the timed region.
// - Absolute milliseconds vary by machine. Focus on the "x prev" columns.

#include <algorithm>
#include <chrono>
#include <cstdint>
#include <iomanip>
#include <iostream>
#include <random>
#include <stdexcept>
#include <vector>

using Clock = std::chrono::steady_clock;

void insertionSort(std::vector<int>& A) {
    const int n = static_cast<int>(A.size());

    for (int i = 1; i < n; ++i) {
        const int key = A[i];
        int j = i - 1;

        while (j >= 0 && A[j] > key) {
            A[j + 1] = A[j];
            --j;
        }
        A[j + 1] = key;
    }
}

void merge(std::vector<int>& A, int p, int q, int r) {
    std::vector<int> L(A.begin() + p,     A.begin() + q + 1);
    std::vector<int> R(A.begin() + q + 1, A.begin() + r + 1);

    int i = 0;
    int j = 0;
    int k = p;

    while (i < static_cast<int>(L.size()) &&
           j < static_cast<int>(R.size())) {
        A[k++] = (L[i] <= R[j]) ? L[i++] : R[j++];
    }

    while (i < static_cast<int>(L.size())) {
        A[k++] = L[i++];
    }

    while (j < static_cast<int>(R.size())) {
        A[k++] = R[j++];
    }
}

void mergeSort(std::vector<int>& A, int p, int r) {
    if (p >= r) {
        return;
    }

    const int q = p + (r - p) / 2;
    mergeSort(A, p, q);
    mergeSort(A, q + 1, r);
    merge(A, p, q, r);
}

enum class Shape {
    Random,
    Sorted,
    Reversed
};

std::vector<int> makeInput(int n, Shape shape, std::mt19937& rng) {
    std::vector<int> A(n);

    for (int i = 0; i < n; ++i) {
        A[i] = i;
    }

    if (shape == Shape::Random) {
        std::shuffle(A.begin(), A.end(), rng);
    } else if (shape == Shape::Reversed) {
        std::reverse(A.begin(), A.end());
    }

    return A;
}

constexpr int WARMUP = 1;
constexpr int TRIALS = 5;

// A volatile sink makes the sorted result observably used, preventing an
// aggressive optimizer from treating the computation as irrelevant.
static volatile std::uint64_t resultSink = 0;

double median(std::vector<double> values) {
    std::sort(values.begin(), values.end());
    return values[values.size() / 2];
}

void validateAndConsume(const std::vector<int>& A) {
    if (!std::is_sorted(A.begin(), A.end())) {
        throw std::runtime_error("Sort validation failed.");
    }

    if (!A.empty()) {
        resultSink ^= static_cast<std::uint64_t>(A.front());
        resultSink ^= static_cast<std::uint64_t>(A.back()) << 1U;
        resultSink ^= static_cast<std::uint64_t>(A[A.size() / 2]) << 2U;
    }
}

double timeInsertionMs(const std::vector<int>& original) {
    std::vector<double> runs;

    for (int trial = 0; trial < WARMUP + TRIALS; ++trial) {
        std::vector<int> A = original;

        const auto start = Clock::now();
        insertionSort(A);
        const auto stop = Clock::now();

        validateAndConsume(A);  // outside the timed interval

        if (trial >= WARMUP) {
            runs.push_back(
                std::chrono::duration<double, std::milli>(stop - start).count()
            );
        }
    }

    return median(runs);
}

double timeMergeMs(const std::vector<int>& original) {
    std::vector<double> runs;

    for (int trial = 0; trial < WARMUP + TRIALS; ++trial) {
        std::vector<int> A = original;

        const auto start = Clock::now();
        mergeSort(A, 0, static_cast<int>(A.size()) - 1);
        const auto stop = Clock::now();

        validateAndConsume(A);  // outside the timed interval

        if (trial >= WARMUP) {
            runs.push_back(
                std::chrono::duration<double, std::milli>(stop - start).count()
            );
        }
    }

    return median(runs);
}

void runTable(Shape shape,
              const char* label,
              const std::vector<int>& sizes) {
    std::mt19937 rng(377);

    std::cout << "\n" << label << '\n';
    std::cout << std::string(64, '-') << '\n';

    std::cout << std::setw(10) << "n"
              << std::setw(17) << "insertion(ms)"
              << std::setw(10) << "x prev"
              << std::setw(15) << "merge(ms)"
              << std::setw(10) << "x prev"
              << '\n';

    std::cout << std::string(64, '-') << '\n';

    double prevInsertion = 0.0;
    double prevMerge = 0.0;

    for (int n : sizes) {
        const std::vector<int> input = makeInput(n, shape, rng);

        const double insertionMs = timeInsertionMs(input);
        const double mergeMs = timeMergeMs(input);

        std::cout << std::setw(10) << n
                  << std::setw(17) << std::fixed << std::setprecision(3)
                  << insertionMs;

        if (prevInsertion > 0.0) {
            std::cout << std::setw(9) << std::setprecision(1)
                      << insertionMs / prevInsertion << "x";
        } else {
            std::cout << std::setw(10) << "-";
        }

        std::cout << std::setw(15) << std::setprecision(3) << mergeMs;

        if (prevMerge > 0.0) {
            std::cout << std::setw(9) << std::setprecision(1)
                      << mergeMs / prevMerge << "x";
        } else {
            std::cout << std::setw(10) << "-";
        }

        std::cout << '\n';

        prevInsertion = insertionMs;
        prevMerge = mergeMs;
    }
}

int main() {
    const std::vector<int> sizes =
        {1000, 2000, 4000, 8000, 16000, 32000, 64000};

    std::cout << "CSCI 377 - Insertion Sort vs. Merge Sort\n";
    std::cout << "Instructor: Avijit Roy\n";
    std::cout << "Every row doubles n. Focus on growth factors, not exact ms.\n";

    runTable(
        Shape::Random,
        "RANDOM INPUT (typical insertion-sort behavior)",
        sizes
    );

    runTable(
        Shape::Reversed,
        "REVERSE-SORTED INPUT (insertion-sort worst case)",
        sizes
    );

    runTable(
        Shape::Sorted,
        "ALREADY-SORTED INPUT (insertion-sort best case)",
        sizes
    );

    std::cout << "\nQuestions:\n";
    std::cout << "  1. When n doubles, what happens to each runtime?\n";
    std::cout << "  2. Why is sorted input dramatically different for insertion sort?\n";
    std::cout << "  3. Why is merge sort much less sensitive to input order?\n";
    std::cout << "  4. Why should we not treat these milliseconds as universal constants?\n";

    // Referencing the sink also makes its role explicit.
    if (resultSink == UINT64_MAX) {
        std::cerr << "unreachable: " << resultSink << '\n';
    }

    return 0;
}
