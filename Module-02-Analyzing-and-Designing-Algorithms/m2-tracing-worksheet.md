# CSCI 377 — Module 2 Tracing Worksheet

**Instructor:** Avijit Roy  
**John Jay College of Criminal Justice, CUNY**  
**Fall 2026**

Name: ________________________________  Date: ______________

No laptops. Pen or pencil only. You have about 8 minutes for Part 1.

---

## Part 1 — Trace insertion sort

**A = ⟨5, 2, 4, 6, 1, 3⟩**

Fill in the array after each pass completes, and count the comparisons and
shifts that pass performed.

A comparison is one evaluation of `A[j] > key`, **including the one that fails
and ends the loop.** A shift is one execution of `A[j+1] = A[j]`.

| pass | key | array after the pass | comparisons | shifts |
|:----:|:---:|:--------------------:|:-----------:|:------:|
| start | — | `5  2  4  6  1  3` | — | — |
| i = 2 | | `__ __ __ __ __ __` | | |
| i = 3 | | `__ __ __ __ __ __` | | |
| i = 4 | | `__ __ __ __ __ __` | | |
| i = 5 | | `__ __ __ __ __ __` | | |
| i = 6 | | `__ __ __ __ __ __` | | |
| | | **totals** | | |

---

## Part 2 — Four questions

**(a)** What are your totals?

comparisons = ________   shifts = ________

---

**(b)** Rearrange these same six numbers so that insertion sort does **as much
work as possible.**

arrangement: `__ __ __ __ __ __`

comparisons = ________   shifts = ________

---

**(c)** Rearrange them so it does **as little work as possible.**

arrangement: `__ __ __ __ __ __`

comparisons = ________   shifts = ________

---

**(d)** For an array of **n** elements, what is the largest number of
comparisons any input could force? Write it as a formula in n.

Show the reasoning, not just the answer:

_________________________________________________________________

_________________________________________________________________

---

## Part 3 — If you finish early

**(e)** Your answer to (c) had zero shifts. Explain in one sentence why the
inner `while` loop cannot execute its body even once on that input.

_________________________________________________________________

**(f)** Suppose line 5 read `A[j] >= key` instead of `A[j] > key`. The
algorithm would still sort correctly. Name one thing that would change.

_________________________________________________________________

**(g)** Merge sort does the same amount of work on all three of your
arrangements from (a), (b) and (c). Why does the input order not matter to it?

_________________________________________________________________

---

## Reference — the pseudocode

```
INSERTION-SORT(A, n)
1   for i = 2 to n
2       key = A[i]
3       // insert A[i] into sorted A[1:i-1]
4       j = i - 1
5       while j > 0 and A[j] > key
6           A[j+1] = A[j]
7           j = j - 1
8       A[j+1] = key
```
