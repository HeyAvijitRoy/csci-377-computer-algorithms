# CSCI 377 — Big-O, Big-Omega, and Big-Theta: A Primer

**Course:** CSCI 377 — Computer Algorithms  
**Instructor:** Avijit Roy    
**Institution:** John Jay College of Criminal Justice, CUNY  
**Semester:** Fall 2026  

> **Where this fits:** In Module 2, we used `Θ` informally — as shorthand for
> "grows like." This file is the bridge to Module 3, where we'll make `O`,
> `Ω`, and `Θ` precise and use them formally. Read this on your own between
> classes; we'll build on it, not repeat it, when we meet again.

---

## Why three different symbols?

Every algorithm has a running time that depends on the input size `n`. We
almost never want to talk about the *exact* number of operations — it's
messy, it depends on the machine, and it's usually more detail than we need.
Instead, we want to talk about **growth rate**: as `n` gets large, does the
running time scale like `n`? Like `n²`? Like `n log n`?

`O`, `Ω`, and `Θ` are three different ways of relating a function to a growth
rate, and they answer three different questions:

| Notation | Question it answers | Kind of bound |
|:---:|---|:---:|
| `O(g(n))` | Does the running time grow **no faster than** `g(n)`? | upper bound |
| `Ω(g(n))` | Does the running time grow **at least as fast as** `g(n)`? | lower bound |
| `Θ(g(n))` | Does the running time grow **exactly at the rate of** `g(n)`? | tight bound |

If that reads like "upper bound, lower bound, and both at once," that's
exactly right — and it's the fastest way to remember what each one claims.

---

## The formal definitions

You'll see these written with two constants, `c` and `n₀`. Don't let the
symbols intimidate you — all three definitions are really saying the same
kind of thing: *"eventually, and up to a constant multiplier, this
comparison holds."*

### Big-O: upper bound

`f(n) = O(g(n))` means: there exist positive constants `c` and `n₀` such that

```text
0 <= f(n) <= c · g(n)     for all n >= n₀
```

In words: once `n` is large enough (`n >= n₀`), `f(n)` never exceeds some
constant multiple of `g(n)`. `f` grows **no faster** than `g`.

### Big-Omega: lower bound

`f(n) = Ω(g(n))` means: there exist positive constants `c` and `n₀` such that

```text
0 <= c · g(n) <= f(n)     for all n >= n₀
```

In words: once `n` is large enough, `f(n)` is always at least some constant
multiple of `g(n)`. `f` grows **at least as fast** as `g`.

### Big-Theta: tight bound

`f(n) = Θ(g(n))` means `f(n)` is **both** `O(g(n))` **and** `Ω(g(n))`.
Equivalently, there exist positive constants `c₁`, `c₂`, and `n₀` such that

```text
0 <= c₁ · g(n) <= f(n) <= c₂ · g(n)     for all n >= n₀
```

In words: `f(n)` is sandwiched between two constant multiples of `g(n)`,
forever, once `n` is large enough. `g(n)` isn't just *a* bound — it's the
**exact growth rate**.

---

## An analogy: speed limits on a highway

* **`O(g(n))`** is like a speed limit sign: "you will go **at most** this
  fast." It doesn't promise you'll actually go that fast — you might be
  stuck in traffic going much slower — it only rules out going faster.
* **`Ω(g(n))`** is like a minimum speed sign: "you will go **at least** this
  fast." It doesn't promise you won't go faster.
* **`Θ(g(n))`** is a promise of both signs at once, at the same location: you
  are guaranteed to be traveling within a fixed range of exactly this speed
  — not faster, not slower, up to a constant factor.

---

## The trap: O/Ω/Θ and best-case/worst-case are two *different* axes

This is the single most common source of confusion, so it gets its own
section.

**Best-case / worst-case** describes *which input* you're talking about —
the input that makes the algorithm do the least work, or the most.

**`O` / `Ω` / `Θ`** describes *how a running-time function grows* — it says
nothing about which input you picked. You can attach any of the three to the
best case, the worst case, or the running time in general.

They're independent questions, and you're allowed to combine them. Using
insertion sort as the example:

* **Insertion sort's worst-case running time is `Θ(n²)`.** On reverse-sorted
  input, the running time grows *exactly* quadratically — both an upper and
  a lower bound of `n²` hold for that specific input.
* **Insertion sort's best-case running time is `Θ(n)`.** On already-sorted
  input, the running time grows *exactly* linearly.
* **Insertion sort's running time, across *all* inputs, is `O(n²)`** — `n²`
  operations is never exceeded, no matter what the input looks like. But it
  is **not** `Ω(n²)` across all inputs, because sorted input only does
  `Θ(n)` work — far less than a constant multiple of `n²` for large `n`. So
  across all inputs, only the upper bound (`O`) holds unconditionally; the
  lower bound depends on which case you're in.

Merge sort is the tidy counterexample: its running time is `Θ(n log n)` in
the best case, the worst case, and everything in between — all three
notations agree, for every input shape. That's exactly why
[`sort_benchmark.cpp`](sort_benchmark.cpp)'s merge-sort timings barely moved
across random, sorted, and reverse-sorted input, while insertion sort's
swung wildly.

---

## When do you actually reach for each one?

* **Reach for `O`** when you want to state a guarantee that something won't
  get any worse than a certain rate — this is by far the most common use in
  practice. When people casually say an algorithm "is `O(n log n)`," they
  usually mean this: an upper bound on how bad it can get.
* **Reach for `Ω`** when you want to state a *floor* — a guarantee that some
  amount of work is unavoidable. This shows up less in describing a specific
  algorithm and more in proving that *no* algorithm solving a given problem
  can do better than a certain rate (for example: any comparison-based
  sorting algorithm requires `Ω(n log n)` comparisons in the worst case — a
  fact about the *problem*, not about any one algorithm).
* **Reach for `Θ`** when you have a precise, matching upper and lower bound
  and want to say so — this is the strongest, most informative claim of the
  three, because it rules out the running time being either much faster or
  much slower than `g(n)`.

---

## A quick sanity check: does the constant matter?

No — none of the three notations care about constant factors or
lower-order terms. `Θ(n²)`, `Θ(5n²)`, and `Θ(0.01n² + 1000n + 7)` are all the
same growth rate, because for large enough `n`, the highest-order term
dominates and a constant multiplier can always be chosen to absorb the rest.
This is *not* a claim that constants never matter in practice —
[`sort_benchmark.cpp`](sort_benchmark.cpp) is proof that they can matter a
great deal for a specific `n` — it's a statement about what these notations
choose to describe: shape of growth, not implementation speed.

---

## Practice before next class

Don't just read the definitions — try applying them. For each, decide
whether the claim is about an upper bound, a lower bound, or a tight bound,
and whether it's talking about a best case, a worst case, or all inputs:

1. "Linear search's worst-case running time is `Θ(n)`."
2. "Linear search's best-case running time is `Θ(1)`."
3. "Any algorithm that must examine every element of an unsorted array is
   `Ω(n)`."
4. "Insertion sort is `O(n²)`" (with no best-case/worst-case qualifier at
   all — what is this claim actually promising, and what is it *not*
   promising?)
5. Try writing out, in your own words, why `Θ(g(n))` is a *strictly
   stronger* claim than `O(g(n))` alone.

We'll pick a few of these apart together at the start of Module 3.

---

## Where to go if you want more

* **Textbook:** *Introduction to Algorithms*, 3rd ed. (Cormen, Leiserson,
  Rivest, Stein) — Chapter 3, "Growth of Functions," covers this material in
  full formal detail, including how to prove a bound directly from the
  definition using `c` and `n₀`.
* **This repository:** revisit
  [`sort_benchmark.cpp`](sort_benchmark.cpp) and try connecting its growth
  factors (≈4x per doubling for insertion sort, ≈2x-and-a-bit for merge
  sort) back to the `Θ` bounds discussed above.

Official readings, due dates, and any quiz coverage are posted in
**Brightspace** — this file is a supplement, not a replacement.
