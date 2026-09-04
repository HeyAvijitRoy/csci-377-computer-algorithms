# Module 02 — Analyzing and Designing Algorithms

This module moves from *tracing a single algorithm* (Module 01) to
*proving an algorithm correct, counting its work, and comparing it against a
different design for the same problem*. We use **Insertion Sort** and
**Merge Sort** as the running example throughout.

By the end of this module you should be able to do more than recognize
insertion sort or merge sort by name — you should be able to **trace** them,
**prove** insertion sort correct with a loop invariant, **analyze** how much
work each performs, and **explain** why a different design (divide and
conquer) changes the growth rate entirely.

---

## Topics Covered

This module includes:

* Insertion sort — an incremental sorting strategy
* Sorting as a specification: order **and** permutation of the input
* Manual tracing with instrumentation (counting comparisons and shifts)
* Loop invariants and the three-part proof: initialization, maintenance,
  termination
* The RAM model and line-by-line cost analysis
* Best-case and worst-case behavior, and why input order drives it
* Order of growth / Θ used informally ("grows like")
* Divide and conquer as a design strategy
* Merge sort, the `MERGE` step, and why it runs in linear time
* Writing (not yet solving) the recurrence `T(n) = 2T(n/2) + Θ(n)`
* Recursion-tree reasoning for why merge sort is `Θ(n log n)`
* Stability in sorting algorithms
* Empirical benchmarking and growth trends as `n` doubles
* Why an asymptotically better algorithm isn't always the faster choice in
  practice

A note on scope: this module uses `Θ` informally, as shorthand for "grows
like." The **formal** definitions of `O`, `Ω`, and `Θ` — upper bound, lower
bound, tight bound — are introduced in Module 3. If you want to get ahead of
that (or the class is on break before Module 3 starts), see
[asymptotic-notation-primer.md](asymptotic-notation-primer.md) below.

---

## Files

* [insertion_sort_trace.cpp](insertion_sort_trace.cpp) — instrumented insertion sort
* [merge_sort_trace.cpp](merge_sort_trace.cpp) — instrumented, recursion-tracing merge sort
* [sort_benchmark.cpp](sort_benchmark.cpp) — timing comparison across sizes and input shapes
* [m2-tracing-worksheet.md](m2-tracing-worksheet.md) — in-class hand-tracing worksheet
* [m2-tracing-worksheet-solutions.md](m2-tracing-worksheet-solutions.md) — worked solutions guide
* [asymptotic-notation-primer.md](asymptotic-notation-primer.md) — self-study bridge into Module 3's formal O/Ω/Θ

---

## Insertion Sort: The Idea

Think of sorting playing cards already in your hand. You keep the cards you
already hold sorted. When a new card arrives, you don't throw everything down
and start over — you slide the new card left until it reaches the correct
spot among the cards you're already holding.

That's insertion sort: at the start of each pass, the front portion of the
array (the **sorted prefix**) is already sorted. The next value (the **key**)
gets inserted into its correct position within that prefix, shifting larger
values one step to the right to make room.

One easy misconception to avoid: "sorted so far" does not mean those
positions are *final*. A later, smaller key can still move into the middle of
the already-sorted prefix and push larger values further right.

---

## [insertion_sort_trace.cpp](insertion_sort_trace.cpp)

This program runs insertion sort on a fixed input and prints, after every
pass:

* the array in its current state,
* how many **key comparisons** (`A[j] > key`) were performed that pass,
* how many **shifts** (`A[j+1] = A[j]`) were performed that pass.

Students should be able to explain:

* why the outer loop starts at `i = 1` in the C++ version but `i = 2` in the
  CLRS pseudocode (0-based vs. 1-based indexing),
* why the inner loop's boundary check is `j >= 0` in C++ but `j > 0` in the
  pseudocode — and what would go wrong (a value that belongs at index 0
  failing to move all the way there) if you used `j > 0` in C++ instead,
* what a "shift" is (`A[j + 1] = A[j]`) and why it is not the same as a
  "swap" — the key is saved off in its own variable first, so overwriting
  `A[j]` is safe,
* and what it means for a pass to perform **zero** shifts.

Run it, then compare its printed counts against your own hand trace on the
[worksheet](m2-tracing-worksheet.md).

---

## Correctness: The Loop Invariant

Watching an algorithm work on a few examples is evidence, not proof. A **loop
invariant** is a statement that stays true every time we reach a fixed point
in a loop — and it lets us prove the algorithm correct for *every* input, not
just the ones we tried.

For insertion sort, the invariant is:

> At the start of each outer-loop iteration `i`, the subarray `A[0:i-1]`
> (C++, 0-based) consists of the elements originally in that range, in
> sorted order.

That single sentence encodes both halves of the sorting specification: the
prefix is **ordered**, and it is a **permutation** of the original elements
(nothing invented, removed, or duplicated).

Proving it has three parts — essentially induction:

* **Initialization** — before the first pass, the prefix is a single
  element. One element is trivially sorted.
* **Maintenance** — assuming the prefix is sorted when a pass begins, the
  algorithm saves the key, shifts every larger prefix value right, and
  places the key into the resulting gap. The prefix is now one element
  larger, still sorted, still the same set of elements.
* **Termination** — the outer loop eventually ends with the prefix equal to
  the whole array. The invariant then tells us the *entire* array is sorted.

Initialization is like a base case; maintenance is like an inductive step.
Termination is what turns "true at every step" into "true about the final
answer."

---

## The RAM Model and Counting Work

To compare algorithms without tying the conclusion to one laptop, we use the
**RAM model**: a simplification where ordinary primitive operations
(comparisons, arithmetic, assignment) are each treated as constant-time. Real
hardware is messier — caching and compiler optimization matter — but for
growth-rate analysis those effects usually change constants, not the
underlying shape of the growth.

Under this model, each line of insertion sort executes some number of times,
and *most* of those counts depend only on `n`. The one line whose count
depends on the **input's values**, not just its size, is the inner `while`
condition — CLRS calls its execution count `t_i`.

**A subtlety worth knowing:** the worksheet's "key comparison" and CLRS's
`t_i` are two different counts:

```text
key comparison (worksheet):  A[j] > key
t_i (CLRS cost table):       j > 0 and A[j] > key
```

Because `&&`/`and` short-circuits, these can differ once `j` moves past the
start of the array — the boundary check can fail on its own, without
`A[j] > key` ever being evaluated. Both counts are legitimate; the important
habit is to say which one you're using and stay consistent. See the
[solutions guide](m2-tracing-worksheet-solutions.md) for a worked example of
where this matters.

Summing `t_i` across all passes is exactly how the best-case/worst-case
formulas below are derived.

---

## Best Case and Worst Case

* **Best case — already sorted.** Every key is already `>=` everything
  before it, so each pass's `while` loop stops on its first check. One
  comparison per pass, no shifts. Running time grows linearly: `Θ(n)`.
* **Worst case — reverse sorted.** Every new key is smaller than the entire
  current prefix, so it travels all the way to the front. The number of
  shifts (and comparisons) on pass `i` is `i - 1`, so the total is:

  ```text
  1 + 2 + 3 + ... + (n - 1) = n(n - 1) / 2
  ```

  The dominant term is proportional to `n²`, so the worst-case running time
  is `Θ(n²)`.

### Why the sum stops at `(n - 1)`, not `n`

A common mix-up in class: it's tempting to reason "there are `n` elements,
so the last one should cost up to `n` comparisons." It doesn't — it costs at
most `n - 1`. Here's why, in two steps.

**Step 1 — there are only `n - 1` passes to begin with.** Insertion sort's
outer loop starts on the *second* element (`i = 2` in the pseudocode, `i = 1`
in the 0-based C++ loop). The first element is never given a pass at all,
because a single element is already a sorted array of size one — there's
nothing to insert it into. So out of `n` total elements, only `n - 1` of them
ever trigger a pass.

**Step 2 — the last pass can't compare a key against itself.** On the final
pass, we're inserting the `n`-th (last) element. It's tempting to think "this
is element number `n`, so it might take `n` comparisons." But a key is only
ever compared against elements *before* it — the sorted prefix. When we're
inserting the last element, that prefix contains everything **except** the
element itself, which is `n - 1` elements, not `n`. There is nothing after
the last element to compare against either. So the most expensive pass costs
`n - 1` comparisons, and that's also the *largest* term in the sum.

**Putting the two together:** as the pass number `i` runs from `2` up to `n`,
the worst-case cost of that pass, `i - 1`, runs from `1` up to `n - 1`. That
is exactly `n - 1` terms — one per pass — matching Step 1, and topping out at
`n - 1` — matching Step 2:

```text
pass i =        2   3   4   ...  n
cost (i - 1) =  1   2   3   ...  n-1
```

**Concrete check with `n = 6`** (the worksheet's reverse-sorted array
`6 5 4 3 2 1`): there are `6` elements, but only `5` passes happen
(`i = 2, 3, 4, 5, 6`), and the sum has `5` terms, topping out at `5 = 6 - 1`,
not `6`:

```text
1 + 2 + 3 + 4 + 5 = 15
```

**Where `n(n - 1) / 2` comes from:** this is the standard sum of consecutive
integers, `1 + 2 + ... + m = m(m + 1) / 2`, with `m = n - 1` substituted in:

```text
1 + 2 + ... + (n - 1) = (n - 1) · ((n - 1) + 1) / 2 = (n - 1) · n / 2
```

which is the same thing written the other way around, `n(n - 1) / 2`.

An easy trap: could we speed up *finding* the insertion point with binary
search? We could cut down comparisons, but shifting the elements out of the
way to open a gap is still the bottleneck — that's still up to `Θ(n)` work
per pass in the worst case. Reducing comparisons alone doesn't change the
growth rate. That bottleneck is what motivates a different design entirely.

---

## Divide and Conquer, and [merge_sort_trace.cpp](merge_sort_trace.cpp)

**Divide and conquer** solves a problem by (1) dividing it into smaller
instances of the same problem, (2) conquering each recursively, and (3)
combining their solutions. For sorting: split the array in half, sort each
half recursively, then merge the two sorted halves. The base case — a
one-element subarray — is already sorted, which is what stops the recursion.

`MERGE` is efficient specifically *because* both halves are already sorted:
the smallest remaining element must be at the front of one of the two
halves, so only those two front values ever need to be compared. Every
output position gets written exactly once, so `MERGE` runs in `Θ(n)` time.

This gives the recurrence:

```text
T(n) = 2T(n/2) + Θ(n)
```

— two subproblems of half the size, plus linear work to combine them. (Module
2 asks you to be able to **write** this recurrence from the algorithm's
structure; solving recurrences in general is Module 4's job.)

**Why `Θ(n log n)`, informally:** at the root, `MERGE` does about `cn` work.
At the next level, two subproblems of size `n/2` each do about `cn/2`,
totaling `cn` again. Every level of the recursion costs about `cn`, and
halving `n` down to 1 takes about `log₂ n` levels — so total work is roughly
`cn log₂ n`.

[merge_sort_trace.cpp](merge_sort_trace.cpp) runs merge sort on a fixed input and prints the
recursive split/merge structure, indented by recursion depth, so the
indentation is a textual picture of the recursion tree. It also counts:

* key comparisons performed inside `MERGE`,
* element writes performed inside `MERGE`.

Students should be able to explain:

* why the array is split by **index** (`q = p + (r - p) / 2`) rather than by
  value — and why that's exactly what makes merge sort's running time
  insensitive to input order, unlike insertion sort,
* how `L[i] <= R[j]` (rather than `<`) preserves **stability** — ties keep
  the left element first, so equal-valued elements never reverse their
  original relative order,
* why every element of `A[p..r]` is written exactly once per call to
  `MERGE`,
* and how the indentation in the output reflects the recursion tree.

---

## [sort_benchmark.cpp](sort_benchmark.cpp)

This program measures actual running time (not a hand count) for insertion
sort and merge sort, across increasing input sizes, on three input shapes:
random, reverse-sorted, and already-sorted.

For each input size it discards one warm-up trial and reports the **median**
of five timed trials, so a single slow measurement doesn't distort the
result.

Watch the **growth factor**, not the millisecond values — your machine will
produce different absolute numbers, but the ratios should reproduce
reliably:

* Insertion sort should trend toward roughly **4x** per doubling of `n` on
  random or reverse-sorted input — exactly what `(2n)² = 4n²` predicts.
* Merge sort should trend toward **a bit above 2x** per doubling — doubling
  `n` doubles the data, and the `log n` factor grows only slightly.

**The counterintuitive result to look for:** on already-sorted input at
large `n`, insertion sort can dramatically *outperform* merge sort — its
work is linear and its per-operation overhead is tiny, while merge sort
still pays for its full recursive decomposition and merging regardless of
input order. `Θ(n log n)` beating `Θ(n²)` is a statement about *large-n
growth*, not a guarantee that it wins on every input and size. "Better" also
depends on input shape, memory constraints, and whether you need a
predictable worst case.

Students should be able to explain:

* why insertion sort's timings change dramatically across the three input
  shapes, while merge sort's do not,
* why the sorted-input case is the one where insertion sort can beat merge
  sort, and why that isn't a contradiction of `Θ(n log n) < Θ(n²)`,
* why correctness (`std::is_sorted`) is checked *outside* the timed region,
* and why absolute millisecond values are not meaningful to compare across
  different machines — the growth factor is.

---

## [asymptotic-notation-primer.md](asymptotic-notation-primer.md)

Throughout this module, `Θ` was used informally — as shorthand for "grows
like." This file makes `O`, `Ω`, and `Θ` precise: their formal definitions,
what each one actually claims (upper bound, lower bound, tight bound), a
worked explanation of why best-case/worst-case and O/Ω/Θ are two *separate*
questions that get conflated constantly, and practice prompts to try before
Module 3.

If there's a gap of a class or two before Module 3 starts, this is the
independent-study reading to work through in the meantime.

---

## [m2-tracing-worksheet.md](m2-tracing-worksheet.md)

An in-class, closed-laptop worksheet: trace insertion sort by hand on a
6-element array, count comparisons and shifts, then reason about best case,
worst case, and why merge sort behaves differently.

Complete this **before** looking at the [solutions file](m2-tracing-worksheet-solutions.md) below. Getting a
count wrong and figuring out why is more valuable than confirming a count
that was already right.

---

## [m2-tracing-worksheet-solutions.md](m2-tracing-worksheet-solutions.md)

A worked walkthrough of the tracing worksheet, with explanations for each
answer — not just the final numbers. Use it to check your work and to see
the reasoning behind the best-case/worst-case formulas and the stability and
input-sensitivity questions.

---

## Student Practice

After working through this module, students should be able to:

1. Trace insertion sort by hand and count comparisons and shifts per pass.
2. State the loop invariant for insertion sort and walk through
   initialization, maintenance, and termination.
3. Explain why insertion sort's worst case is `Θ(n²)` and best case is
   `Θ(n)`, and derive `n(n-1)/2`.
4. Explain the RAM model and why it lets us reason about algorithms without
   referencing a specific machine.
5. Trace merge sort's recursive split/merge structure.
6. Write the recurrence `T(n) = 2T(n/2) + Θ(n)` for merge sort and explain,
   informally, why it works out to `Θ(n log n)`.
7. Explain why merge sort's running time does not depend on input order,
   while insertion sort's does.
8. Explain what makes a sorting algorithm "stable" and give an example of an
   algorithm losing stability.
9. Read a doubling benchmark table and connect the growth pattern (4x vs.
   ~2x) to the corresponding Θ bound.
10. Explain why an algorithm with a better asymptotic bound can still lose on
    a specific input, and give the sorted-input example as evidence.
11. State the formal definitions of `O`, `Ω`, and `Θ`, and explain why
    best-case/worst-case and O/Ω/Θ are answering two different questions.

---

## Recommended Testing

For each program:

1. Predict the output before running it.
2. Run the program.
3. Compare the result with your prediction.
4. Change the input (a different array, or a different value of `n`).
5. Repeat the process.

For [sort_benchmark.cpp](sort_benchmark.cpp) in particular, try adding a larger size to the
`sizes` vector in `main()` and predict the new row before running it.

---

## Key Questions

Before moving forward, make sure you can answer:

* What is a key comparison, and how is it different from `t_i` (the full
  `while` condition)?
* What are the two parts of the sorting specification, and why does
  "permutation" matter as much as "ordered"?
* What does the insertion-sort loop invariant say, and why do all three
  parts (initialization, maintenance, termination) matter for the proof?
* What arrangement of input forces insertion sort to do the most work? The
  least?
* Why doesn't searching for the insertion point faster (e.g., binary search)
  fix insertion sort's worst-case growth rate?
* Why does merge sort split by index rather than by value?
* Where does the recurrence `T(n) = 2T(n/2) + Θ(n)` come from?
* Why is merge sort's running time the same shape regardless of input order?
* What does it mean for a sort to be stable, and why might that matter in a
  real application (e.g., sorting records that already have a secondary
  order)?
* Why can insertion sort beat merge sort on a specific input, even though
  `Θ(n log n)` is a "better" growth rate than `Θ(n²)`?

---

## Important

The purpose of these examples is to understand the **algorithms, their
correctness, their execution, and their comparative behavior** — not to
memorize the source code.

You should be able to:

* trace each algorithm by hand,
* prove insertion sort correct using its loop invariant,
* explain why its running time behaves the way it does,
* modify it,
* benchmark it,
* and describe its growth rate informally using Θ.

Official assignments, quizzes, readings, deadlines, and submission
instructions are posted in **Brightspace**.
