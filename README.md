## Experiments with binary trees in C

This project records my experiments with various data structures such as binary 
heaps, redblack trees etc. I have done my best to make the implementations 
“clean” under static and runtime analysis. A simple interpreter provides a 
framework for testing each data structure. An example finds the intersections of 
line segments.

### Contents

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

### External source code and licensing

The *splay* and *redblack* binary tree implementations are based on
[tree.h](https://github.com/provos/libevent-niels/blob/master/WIN32-Code/tree.h) 
 by Niels Provos.

The *skip-list* implementation is based on *jsw_slib.c*, etc. by Julienne Walker.

The *docs* directory contains a fork of the [Poole](http://getpoole.com)
theme for the [Jekyll](http://jekyllrb.com) static web page generator.

Other code is licensed under the [MIT licence](LICENCE).
