---
layout: post
title: Experiments with binary trees in C
---
## Introduction

This project records my experiments with various data structures such as
*binary heaps*, *redblack trees* etc. I have done my best to make
the implementations "clean" under static and runtime analysis. A simple
interpreter provides a framework for testing each data structure. An example
finds the intersections of line segments.

## Contents

- `binaryHeap.h` contains the public interface for a binary heap type.

- `list.h` contains the public interface for a doubly linked list type. 

- `stack.h` contains the public interface for a stack type. 

- `bsTree.h` contains the public interface for a balanced tree type based
on the pseudo-code in the paper by *Andersson*. See: "Balanced Search
Trees Made Simple", Arne Andersson, Proc. Workshop on Algorithms and Data
Structures, pp. 60-71, 1993.

- `redblackTree.h` contains the public interface for a red-black tree type. 
See: "A dichromatic framework for balanced trees", L. J. Guibas and
R. Sedgewick, Proc. 19th Annual Symposium on Foundations of Computer Science, 
October, 1978. This implementation is a modified version of the macros in 
[tree.h](https://github.com/provos/libevent-niels/blob/master/WIN32-Code/tree.h)
by *Niels Provos*.

- `redblackCache.h` contains the public interface for a cache implemented
with a red-black tree.

- `trbTree.h` contains the public interface for a threaded red-black tree 
type. 

- `skipList.h` contains the public interface for a skip list type. This
implementation is based on `jsw_slib.c` by *Julienne Walker*. See:
"Skip Lists: A Probabilistic Alternative to Balanced Trees", William Pugh,
Commun. ACM, June 1990, Vol. 33, No. 6, pp 668-676. Walker's source code
appears to no longer be available on the interwebs. A modified version is 
used by
[speedtables](https://github.com/flightaware/speedtables/tree/master/ctables/skiplists).

- `splayTree.h` contains the public interface for a splay tree type. 
See :"Self-Adjusting Binary Search Trees", D.D. Sleator and R. E. Tarjan,
Journal of the Association for Computing Machinery, Vol. 32, No. 3, July
1985, pp. 652-686. This implementation is a modified version of the macros in 
[tree.h](https://github.com/provos/libevent-niels/blob/master/WIN32-Code/tree.h)
by *Niels Provos*. A splay tree has the advantage that the most recently 
accessed entry is moved to the root of the tree. On the other hand, when entries 
are accessed in sorted order, the tree is rearranged into a list-like structure 
with a corresponding increase in access time from \\(\mathcal{O}(lg N)\\) to 
\\(\mathcal{O}(N)\\). This splay tree implementation does not automatically 
rebalance the tree but does provide a `splayTreeBalance()` function to rebalance 
the tree when called by the user. See, for example, `test/00/t0068a.sh`.

- `splayCache.h` contains the public interface for a cache implemented
with a splay tree. 

- `swTree.h` contains the public interface for a binary tree that implements
the rebalancing algorithm of *Stout* and *Warren* : "A simple algorithm is
given which takes an arbitrary binary search tree and rebalances it to form
another of optimal shape, using time linear in the number of nodes and only
a constant amount of space (beyond that used to store the initial tree).
This algorithm is therefore optimal in its use of both time and space."
See: "Tree Rebalancing in Optimal Time and Space", Q. F. Stout and
     B. L. Warren, Communications of the ACM, September 1986, Volume 29,
     Number 9, pp. 902-908

- `sgTree.h` contains the public interface for a binary tree that implements
the *scapegoat* self-balancing binary search tree described by *Galperin*
and *Rivest*. See ["ScapegoatTrees", Igal Galperin and 
Ronald L. Rivest](https://people.csail.mit.edu/rivest/pubs/GR93.pdf).

## Design of the interpreter

The interpreter for each data structure contains:
  1. `interp_lex.l`, `interp_yacc.y` and `interp_ex.c` implement a modified
     version of a simple interpreter shown in 
     ["A Compact Guide to Lex & Yacc"](http://epaperpress.com/lexandyacc) 
     by *Thomas Niemann*.
  2. `interp_wrapper.h` defines the operations that may or may not be
     implemented for each data structure. For example, `create` and
     `destroy`, operating on the entire data structure, or `insert`,
     `delete`, `push`, `pop` etc. operating on the entries in the
     data structure.
  3. `interp_callbacks.c` defines interpreter call-back functions
     installed by the `create` operation. These call-back functions
     implement interpreter memory handling, a comparison operator and
     debugging messages.
  4. An implementation of the interpreter wrapper functions for each data
     structure. For example, `binaryHeap_wrapper.c`.
  5. A header file for the public interface of the data structure. For
     example, `binaryHeap.h`.
  6. An implementation of the data structure. For example, `binaryHeap.c`.

Notes on the data structure implementation:
  1. User data is passed to library as a `void` pointer to an entry type
     defined by the user in `interp_data.h`. That type may be a simple
     `key`, `[ key , value` pair etc. For simplicity with these
     examples, I have used `size_t` as the entry `key` only with no
     `value`.
  2. The data structure implementation does not directly declare static or
     global variable data. If the `duplicateEntry()` and/or 
     `deleteEntry()` call-back functions are defined, then they will be
     called when appropriate. (The former is called to replace an existing
     entry).
  3. The comparison function used by `insert`, `remove`, `find` etc 
     looks at the *contents* of the user defined type, *not* the value of
     the pointer. This is simpler but has some disadvantages:
      - requires repeated searches for entries
      - has no validity checking on non `NULL` pointers passed. For 
        example, checking if the pointer has been freed. That is up to
        the caller.

## Porting 
Known porting problems:
  1. The interpreter assumes `sizeof(size_t)=sizeof(void*)`.
  2. Small differences in floating point results between compiling 
     for 64-bit and 32-bit systems

## Building
This project uses GNU `make` to build executables. The generic project rules
are in `make.rules`. A top-level `Makefile` includes a `.mk` makefile
from each source directory. For example, here is the top-level `Makefile`:
```
SOURCE_DIRS=src docs
include make.rules
```
and here is `src/list/list.mk`:
```
list_PROGRAMS := list_interp
PROGRAMS += $(list_PROGRAMS)
VPATH += src/list
list_interp_C_SOURCES := list.c list_wrapper.c
list_interp_STATIC_LIBRARIES := interp.a
$(call add_extra_CFLAGS_macro,$(list_interp_C_SOURCES),-Isrc/interp)
```
The `make.rules` file defines compiler options for `PORT=debug`
(debugging), `PORT=yacc` (flex/bison debugging), `PORT=sanitize`
(address sanitiser), `PORT=coverage`, `PORT=profile` etc. The default
options are for the release build.

Comments:
 1. I only suppress sanitizer and analyzer warnings from the generated
    interpreter files `interp_lex.c` and `interp_yacc.c` . However,
    compiling with `-Wanalyzer-too-complex` shows that `-fanalyzer`
    often bails out for other source files. For example with calls to
    the interpreter call-back functions like `binaryHeap->debug(...)`.
 2. The generated file `interp_lex.c` gives null dereferemce
    warnings when compiled with `-fanalyzer`.
 3. The generated file `interp_yacc.c` gives run-time 
    warnings when compiled with `-fsanitize=bounds-strict`
 4. The address sanitiser build appears to give false positives when
    compiling with `-O2` and `-O3 optimisation`.

Build the *html* version of the *doxygen* documentation with:
```
make docs
```
## Testing
I maintain `librj` with *Peter Miller*'s `aegis` source code control
software. Each change is expected to pass the shell scrupts in
the `test` directory. These shell scripts run an interpreter test script
for the data structure being tested. In addition, each test script is
expected to run without warnings under the `valgrind` emulator or with
a `-fsanitize` build. Run the test scripts under `valgrind` by, for
example: 
```
VALGRIND_CMD="valgrind --leak-check=full" sh test/00/t0010a.sh
```
Initial tests were hard-coded in C. Next, I implemented a Python wrapper for
the list class. The result was cumbersome and not valgrind clean. I now use a
modified version of a simple interpreter shown in "A Compact Guide to Lex &
Yacc" by [Thomas Niemann](http://epaperpress.com/lexandyacc).
The interpreter is built from the files `src/interp/interp_lex.l` and
`src/interp/interp_yacc.y` with implementation details in
`src/interp/interp_ex.c`. The directory for each data structure
contains a "wrapper" file that is a "shim" between the implementation
of the data structure and the interpreter. The following
script shows the syntax for a test of `redblackTree_interp`:
```
  # Test file
  t=time(0);
  l=create();
  "Insert";
  for(x=0; x<30; x=x+1;)
  {
    print x;
    insert(l, x);
  }
  x = rand(1000);
  "x=";print x;
  insert(l, x);
  x=depth(l);
  x = 1234;
  p = find(l, x);
  if (p == 0)
  {
    "Didn't find "; print x;
  }
  else
  {
    print *p;
  }
  walk(l, show);
  x=2;
  p=find(l, x);
  remove(l, &x);
  x=0;
  p=min(l);
  while( x<size(l) ) {
    x=x+1;
    print *p;
    p=next(l, p);
  }
  destroy(l); 
  uprint time(t);
  exit;                                                               
```
Notes:
  1. 26 integer variables named `a` to `z` are allowed.
  2. Strings are contained within double quotes.
  3. `#` starts a comment.
  4. The interpreter uses `size_t` as the entry type. I assume `void*`
     and `size_t` have the same size. 
  5. The `&` and `*` operators work as for C
  6. `print` prints a signed value and `uprint` prints an unsigned value.
  7. `rand(100)` returns a random integer in the range 0 to 99.
  8. Failing to put a `;` after the third statement in a `for()` is a syntax 
     error.


Test coverage is determined with GNU `gcov` and the `-sPORT=coverage` build. 
For example, for `list_t` , `test.in` is from `test/00/t0055a.sh`:
```
$ make PORT=coverage list_interp
$ bin/list_interp < test.in
$ gcov -o obj -s list src/list/list.c
$ less list.c.gcov
```
Running all the hand-coded tests gave a test coverage of 85.53% for
`src/list/list.c` and 92.72% for `src/redblackTree/redblackTree.c`. 

### To-do
 - Add a memory pool to the test interpreter
 - Re-design the interpreter to enable unwinding of the expression tree
   after a syntax error.
 -  Test coverage could be improved by modifying the test interpreter so
    that the test can choose to fail memory allocation.

## Performance comparison of binary tree implementations
The interpreters for the `bsTree_t`, `redblackTree_t`, `sgTree_t`,
`skipList_t` , `splayTree_t` and `swTree_t` implementations were
benchmarked with the `benchmark.sh` shell script. The PC operating system was
`6.1.12-200.fc37.x86_64 GNU/Linux`. The CPU is an Intel i7-7700K 4.20GHz with 
16GiB RAM. The compiler was Red Hat `GCC 12.2.1-4` with options `-O3 -flto`. 
The  `valgrind` version is `3.20.0`. The binary files were stripped of 
debugging symbols. A simple test program shows `sizeof(uintptr_t)==8`. The
preamble of the output from `cg_annotate` running on a `cachegrind` output file 
is:
```
I1 cache:         32768 B, 64 B, 8-way associative
D1 cache:         32768 B, 64 B, 8-way associative
LL cache:         8388608 B, 64 B, 16-way associative
```
The shell script `benchmark_plot.sh` extracts and plots the results.
The benchmark results are plotted on log-log axes with the x-axis being
the number of keys, N, inserted and the y-axis being the result divided by N.

### Performance comparison with random keys inserted
This section compares the performance of each tree implementation when the
keys inserted and deleted are both `y=rand(k)`. The benchmark results for
execution time are shown in the following plot:

![]({{ site.baseurl }}/public/extime_random.svg)

The benchmark results for memory allocation are shown in the following plot:

![]({{ site.baseurl }}/public/alloc_random.svg)

The benchmark results for instruction references are shown in the following plot:

![]({{ site.baseurl }}/public/Iref_random.svg)

The benchmark results for data reads are shown in the following plot:

![]({{ site.baseurl }}/public/Drd_random.svg)

The benchmark results for data writes are shown in the following plot:

![]({{ site.baseurl }}/public/Dwr_random.svg)

### Performance comparison with sorted keys inserted
This section compares the performance of each tree implementation when the
keys inserted are sorted, `y=x` , and random keys are deleted, `y=rand(k)`.
The benchmark results for execution time are shown in the following plot:

![]({{ site.baseurl }}/public/extime_sorted.svg)

The benchmark results for memory allocation are shown in the following plot:

![]({{ site.baseurl }}/public/alloc_sorted.svg)

The benchmark results for instruction references are shown in the following plot:

![]({{ site.baseurl }}/public/Iref_sorted.svg)

The benchmark results for data reads are shown in the following plot:

![]({{ site.baseurl }}/public/Drd_sorted.svg)

The benchmark results for data writes are shown in the following plot:

![]({{ site.baseurl }}/public/Dwr_sorted.svg)

### Comments on the performance comparison
 1. I have not attempted to extract the contribution of the interpreter
    (particularly `interpRand()`) to these test results. Scanning the
    output of `gprof` suggests that the interpreter contribution can be
    neglected for tests with more than 10000 entries.
 2. These tests do not consider the effect of a larger "real-world" 
    executable with more I cache misses.
 3. The binary tree implementations variously use recursive and non-recursive
    `insert` and/or `remove` operations.
 4. For inserting random keys, the "knee" in execution time at about 
    N=100000 is most likely due to the size of the LL cache.
 5. The values of `depth_factor` for the `swTree_t` and `alpha` for the
    `sgTree_t` have been set to values that avoid regular rebalancing of
    these trees with insertion of random keys. Rebalancing these trees
    is an \\( \mathcal{O}(N)\\) operation that must be done sufficiently
    rarely that the *amortised* cost is \\( \mathcal{O}(lg\; N)\\).
    The *Rivest* and *Galperin* paper shows results for
    \\( 0.55 \le \alpha \le 0.75 \\). \\( \alpha = 0.4 \\) gives good results 
    with random keys but fails with sorted inserted keys.
 6. I do not rebalance the `splayTree_t` . It appears that this is
    unnecessary with the random and sorted data used in this benchmark. 
 7. I do not attempt to pack the tree structures. For example, the
    single bit `redblackTreeColour_e` occupies 4 bytes and
    the `redblackTreeNode_t` is aligned so that it occupies 40 bytes.

## An example : Finding line segment intersections

`intersectList_t` implements the algorithm for finding intersections of
line segments from Chapter 2 of "Computational Geometry: Algorithms and 
Applications", Second Edition, De Berg et al. Springer-Verlag, 
ISBN 3-540-65620-0.

Given a list of line segments, the endpoints are stored as "events" in
the event queue, a balanced tree ordered by position. The "upper" 
endpoints are stored with a list of segments sharing that upper endpoint.
Events are considered in order. Segments that lie on the current event
sweep line are stored in the status tree, a balanced tree ordered by
intersection with the sweep line and segment slope. The status tree is
initially empty.

For each event:

1. Find all segments stored in the status tree that contain the
`event` ; they are adjacent. Let `lower(event)` denote the subset of
segments found whose lower endpoint is `event`, and let
`interior(event)` denote the subset of segments found that contain
`event` in their interior and let `upper(event)` denote the subset of
segments whose upper endpoint is `event`. Note that, by design,
segments in `upper(event)` cannot appear in the status tree since
they have not been encountered yet.
2. If `upper(event)+lower(event)+interior(event)` contains more
than one segment then create an intersection point for `event`. 
3. Delete the segments in `lower(event)+interior(event)`
from the status tree.
4. Insert the segments in `upper(event)+interior(event)` into
the status tree, The order of the segments in the status tree should
correspond to the order in which they are intersected by a sweep
line just below `event`. If there is a horizontal segment, it
comes last among all segments containing `event`. (Deleting and
re-inserting the segments of `interior(event)` reverses their order.)
5. If `upper(event)+interior(event)` is empty then:
   - let `sl` and `sr` be the left and right neighbours of `event` in
     the status tree
   - check for a new event point at the intersection of `sl` and `sr`

otherwise:

   1. let `s'` be the leftmost segment of 
     `upper(event)+interior(event)` in the tree 
     and `sl` be the left neighbour of `s'` in the tree
   2. check for a new event point at the intersection of `s'` and `sl`
   3. let `s''` be the rightmost segment of `upper(event)+interior(event)`
     in the tree and `sr` be the right neighbour of `s''` in the tree
   4. check for a new event point at the intersection of `s''` and `sr`.

The test program for `intersectList_t`, in file `intersectList_test.c`,
reads and writes *PostScript* files. The test program 
`intersectList_random_test.c` generates a random list of line segments and
writes a *PostScript* file. The following figure shows an
example of the output from `intersectList_random_test.c`:

![]({{ site.baseurl }}/public/intersectList_random_test.svg)

## About this page
This page was generated by the [Jekyll](http://jekyllrb.com) static site
generator using the [Poole](http://getpoole.com) theme by
[Mark Otto]({{ site.baseurl }}/LICENSE.md). The equations were rendered by
[MathJax](https://www.mathjax.org/).

