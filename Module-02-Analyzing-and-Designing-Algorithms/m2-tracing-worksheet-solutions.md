# CSCI 377 — Module 2 Tracing Worksheet: Solutions & Explanations

**Course:** CSCI 377 — Computer Algorithms
**Institution:** John Jay College of Criminal Justice, CUNY
**Semester:** Fall 2026

> This guide walks through the [`m2-tracing-worksheet.md`](m2-tracing-worksheet.md) exercise.
> Attempt the worksheet by hand **before** reading this file. Tracing an algorithm
> yourself — and getting it wrong the first time — is how the material actually
> sticks. Use this guide to check your work and to see the reasoning behind each
> answer, not as a shortcut around doing the trace.

---

## Counting convention used on the worksheet

The worksheet counts a **key comparison** every time the expression

```text
A[j] > key
```

is actually evaluated — including the comparison that finally fails and ends the
loop.

This is intentionally different from the CLRS cost-table variable \(t_i\), which
counts evaluations of the **entire while condition**:

```text
j > 0 and A[j] > key
```

Because `and`/`&&` short-circuits, those two counts can differ once `j` moves
past the beginning of the array: the `j > 0` check can fail on its own, without
`A[j] > key` ever being evaluated. If your count doesn't match the table below,
this is the first thing to check.

---

## Part 1 — Trace: A = ⟨5, 2, 4, 6, 1, 3⟩

| pass | key | array after | key comparisons | shifts |
|:---:|:---:|:---|---:|---:|
| i = 2 | 2 | `2 5 4 6 1 3` | 1 | 1 |
| i = 3 | 4 | `2 4 5 6 1 3` | 2 | 1 |
| i = 4 | 6 | `2 4 5 6 1 3` | 1 | 0 |
| i = 5 | 1 | `1 2 4 5 6 3` | 4 | 4 |
| i = 6 | 3 | `1 2 3 4 5 6` | 4 | 3 |
|  |  | **Totals** | **12** | **9** |

### Walking through pass i = 5 (key = 1)

This pass has the most work of the five, which makes it a good one to check
your understanding against.

The sorted prefix built up so far is:

```text
2 4 5 6
```

The key, `1`, is smaller than every value in that prefix, so it has to travel
all the way to the front. The comparisons happen right-to-left:

```text
6 > 1   → true → shift 6 right
5 > 1   → true → shift 5 right
4 > 1   → true → shift 4 right
2 > 1   → true → shift 2 right
```

All four comparisons succeed, so all four values shift right. At that point
`j` has moved past index 0 — there's no `A[j]` left to compare against, so the
loop stops on the boundary check alone. That's why this pass shows **4**
comparisons and **4** shifts, not 5 of either.

If you traced 5 comparisons here, you likely counted the boundary check
(`j > 0` failing) as a fifth `A[j] > key` evaluation. Under this worksheet's
convention, that boundary failure doesn't count — nothing was compared to
`key`.

---

## Part 2 — Best case, worst case, and a general formula

### (a) Totals for the given array

- key comparisons = **12**
- shifts = **9**

### (b) Rearranging for maximum work

Insertion sort does the most work when every new key is smaller than
everything already sorted — i.e., the array is in **reverse order**:

```text
6 5 4 3 2 1
```

Each pass `i` now has to push its key past all `i - 1` previously-sorted
elements:

```text
key comparisons = 1 + 2 + 3 + 4 + 5 = 15
shifts           = 1 + 2 + 3 + 4 + 5 = 15
```

### (c) Rearranging for minimum work

Insertion sort does the least work when the array is **already sorted**:

```text
1 2 3 4 5 6
```

- key comparisons = **5**
- shifts = **0**

Each of the five passes performs exactly one comparison, discovers the key is
already in the right place, and stops immediately — no shifting needed.

### (d) General formula for the worst case

For an array of \(n\) elements, the maximum possible number of key
comparisons is:

\[
1 + 2 + \cdots + (n-1) = \frac{n(n-1)}{2}
\]

**Why:** on pass \(i\), the incoming key can be forced to compare against all
\(i - 1\) values currently in the sorted prefix, in the worst case. Summing
that over every pass gives the formula above.

#### Why the sum stops at `n - 1`, and not `n`

This tripped a few people up in class, so it's worth slowing down on. The
array has `n` elements — it's natural to expect the sum to involve `n`
somewhere obvious, and it's tempting to guess the last (biggest) term should
just be `n`. It isn't. Two things are going on:

1. **Only `n - 1` passes ever happen.** The very first element never gets a
   pass at all — a single element is already "sorted" by itself, so
   insertion sort's outer loop starts on the *second* element. Out of `n`
   elements, only `n - 1` of them go through the insert-into-the-prefix
   process.

2. **The last pass compares against `n - 1` elements, not `n`.** A key is
   only ever compared against elements *before* it — never against itself,
   and never against anything after it. When the last element of the array
   is being inserted, the sorted prefix in front of it holds every other
   element: that's `n - 1` elements, not `n`. So even the single most
   expensive pass — the one that has to travel through the entire existing
   prefix — tops out at `n - 1` comparisons.

Check it against our own `n = 6` array from part (b), `6 5 4 3 2 1`: there
are 6 elements, but only 5 passes happen (`i = 2` through `i = 6`), and the
biggest term in the sum you computed, `1 + 2 + 3 + 4 + 5`, is `5`, which is
`6 - 1` — not `6`. That's not a coincidence; it's exactly this rule.

So as the pass number `i` goes from `2` up to `n`, its worst-case cost
`i - 1` goes from `1` up to `n - 1` — giving `n - 1` terms total, topping out
at `n - 1`:

```text
1 + 2 + ... + (n - 1)
```

The `n(n-1)/2` formula itself is just the standard sum of consecutive
integers, \(1 + 2 + \cdots + m = \frac{m(m+1)}{2}\), with `m = n - 1`
substituted in:

\[
1 + 2 + \cdots + (n-1) = \frac{(n-1)\big((n-1)+1\big)}{2} = \frac{(n-1)\,n}{2}
\]

— the same thing as `n(n-1)/2`, just written with the factors swapped.

This is the same kind of summation you'll see formalized later when we work
through the CLRS line-cost analysis — it's worth having this concrete example
in mind when that notation shows up.

---

## Part 3 — Short-answer questions

### (e) Why does the best case have zero shifts?

The input is already sorted, so on every pass:

```text
A[j] <= key
```

is true, meaning `A[j] > key` is false the very first time it's evaluated.
The `while` loop's condition fails immediately, so its body — the line that
performs the shift — never runs.

### (f) What changes if the comparison becomes `A[j] >= key`?

The array still ends up sorted numerically. What changes is **stability**:
two equal-valued elements can now swap their original relative order.
(With `>`, an element already in place is left alone when a new key ties it;
with `>=`, it gets shifted out of the way instead.) You may also see extra
shifts and comparisons whenever duplicate keys appear.

### (g) Why is merge sort much less sensitive to input order?

Merge sort chooses where to split the array from **indices** (the midpoint),
not from the **values** stored there. For a fixed input size, the recursive
split-and-merge shape is therefore the same no matter whether the input is
sorted, reversed, or random.

Inside `MERGE`, every output position is written exactly once per merge step.
The exact number of comparisons can shift slightly depending on the values,
but the total work at each level of the recursion stays linear in the number
of elements at that level. That's why the overall running time stays

\[
\Theta(n \log n)
\]

across all input orders — unlike insertion sort, whose running time swings
between \(\Theta(n)\) and \(\Theta(n^2)\) depending on how the input is
arranged.

---

## Where to go next

- Run [`insertion_sort_trace.cpp`](insertion_sort_trace.cpp) and compare its
  printed comparison/shift counts against your hand trace.
- Run [`merge_sort_trace.cpp`](merge_sort_trace.cpp) to see the recursive
  split/merge structure that makes part (g) true.
- Run [`sort_benchmark.cpp`](sort_benchmark.cpp) to see parts (b), (c), and
  (g) show up as actual timing differences as `n` grows.
