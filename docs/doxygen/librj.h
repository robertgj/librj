/**
 * \file librj.h
 * Main Page for the doxygen documentation.
 */

#if !defined(LIBRJ_H)
#define LIBRJ_H

/** \mainpage Experiments with binary trees in C
 *
 * \section intro_sec Introduction
 *
 * This project records my experiments with various data structures such as
 * \e binary \e heaps, \e redblack \e trees etc. I have done my best to make
 * the implementations "clean" under static and runtime analysis. A simple
 * interpreter provides a framework for testing each data structure.
 *
 * \section contents_sec Contents
 *
 * \c binaryHeap.h contains the public interface for a binary heap type.
 *
 * \c list.h contains the public interface for a doubly linked list type. 
 *
 * \c stack.h contains the public interface for a stack type. 
 *
 * \c bsTree.h contains the public interface for a balanced tree type based
 * on the pseudo-code in the paper by \e Andersson. See: "Balanced Search
 * Trees Made Simple", Arne Andersson, Proc. Workshop on Algorithms and Data
 * Structures, pp. 60-71, 1993.
 *
 * \c redblackTree.h contains the public interface for a red-black tree type. 
 * See: "A dichromatic framework for balanced trees", L. J. Guibas and
 * R. Sedgewick, Proc. 19th Annual Symposium on Foundations of Computer
 * Science, October, 1978. This implementation is a modified version
 * of the macros in the NetBSD source file \c tree.h by \e Niels \e Provos.
 * See: https://github.com/provos/libevent-niels/blob/master/WIN32-Code/tree.h
 *
 * \c redblackCache.h contains the public interface for a cache implemented
 * with a red-black tree.
 *
 * \c trbTree.h contains the public interface for a threaded red-black tree 
 * type. 
 *
 * \c skipList.h contains the public interface for a skip list type. This
 * implementation is based on \c jsw_slib.c by \e Julienne \e Walker. See:
 * "Skip Lists: A Probabilistic Alternative to Balanced Trees", William Pugh,
 * Commun. ACM, June 1990, Vol. 33, No. 6, pp 668-676. Walker's source code
 * appears to no longer be available on the interwebs. A modified version is at:
 * https://github.com/flightaware/speedtables/tree/master/ctables/skiplists
 *
 * \c splayTree.h contains the public interface for a splay tree type. 
 * See :"Self-Adjusting Binary Search Trees", D.D. Sleator and R. E. Tarjan,
 * Journal of the Association for Computing Machinery, Vol. 32, No. 3, July
 * 1985, pp. 652-686. This implementation is a modified version of the macros
 * in the NetBSD source file \c tree.h by \e Niels \e Provos. A splay tree has
 * the advantage that the most recently accessed entry is moved to the root of
 * the tree. On the other hand, when entries are accessed in sorted order, the
 * tree is rearranged into a list-like structure with a corresponding increase
 * in access time from \f$ \mathcal{O}(lg N)\f$ to \f$ \mathcal{O}(N)\f$ .
 * This splay tree implementation does not automatically rebalance the tree
 * but does provide a \c splayTreeBalance() function to rebalance the tree when
 * called by the user. See, for example, \c test/00/t0068a.sh .
 *
 * \c splayCache.h contains the public interface for a cache implemented
 * with a splay tree. 
 *
 * \c swTree.h contains the public interface for a binary tree that implements
 * the rebalancing algorithm of \e Stout and \e Warren : "A simple algorithm is
 * given which takes an arbitrary binary search tree and rebalances it to form
 * another of optimal shape, using time linear in the number of nodes and only
 * a constant amount of space (beyond that used to store the initial tree).
 * This algorithm is therefore optimal in its use of both time and space."
 * See: "Tree Rebalancing in Optimal Time and Space", Q. F. Stout and
 *      B. L. Warren, Communications of the ACM, September 1986, Volume 29,
 *      Number 9, pp. 902-908
 *
 * \c sgTree.h contains the public interface for a binary tree that implements
 * the scapegoat self-balancing binary search tree described by \e Galperin
 * and \e Rivest. See:
 *  "ScapegoatTrees", Igal Galperin and Ronald L. Rivest ,
 *  https://people.csail.mit.edu/rivest/pubs/GR93.pdf
 *
 * \section design_sec Design of the interpreter
 *
 * The interpreter for each data structure contains:
 *
 *   1. \c interp_lex.l , \c interp_yacc.y and \c interp_ex.c implement
 *      a modified version of a simple interpreter shown in "A Compact Guide to
 *      Lex & Yacc" by Thomas Niemann, http://epaperpress.com/lexandyacc .
 *   2. interp_wrapper.h defines the operations that may or may not be
 *      implemented for each data structure. For example, \e create and
 *      \e destroy, operating on the entire data structure, or \e insert ,
 *      \e delete , \e push , \e pop etc. etc. operating on the entries in the
 *      data structure.
 *   3. \c interp_callbacks.c defines interpreter callback functions
 *      installed by the \e create operation. These callback functions
 *      implement interpreter memory handling, a comparison operator and
 *      debugging messages.
 *   4. An implementation of the interpreter wrapper functions for each data
 *      structure. For example, \c binaryHeap_wrapper.c
 *   5. A header file for the public interface of the data structure. For
 *      example, \c binaryHeap.h .
 *   6. An implementation of the data structure. For example, \c binaryHeap.c .
 *
 * Notes on the data structure implementation:
 *   1. User data is passed to library as a \e void pointer to an entry type
 *      defined by the user in \c interp_data.h . That type may be a simple
 *      \e key, [ \e key , \e value ] pair etc. For simplicity with these
 *      examples, I have used \c size_t as the entry \e key only with no
 *      \e value .
 *   2. The data structure implementation does not directly declare static or
 *      global \c variable data. If the \c duplicateEntry() and/or 
 *      \c deleteEntry() call-back functions are defined, then they will be
 *      called when appropriate. (The former is called to replace an existing
 *      entry).
 *   3. The comparison function used by \e insert, \e remove, \e find etc 
 *      looks at the \e contents of the user defined type, \e not the value of
 *      the pointer. This is simpler but has some disadvantages:
 *       - requires repeated searches for entries
 *       - has no validity checking on non \c NULL pointers passed. For 
 *         example, checking if the pointer has been freed. That is up to
 *         the caller.
 *
 * \section port_sec Porting 
 * Known porting problems:
 *   1. The interpreter assumes \c sizeof(size_t)=sizeof(void*)
 *   2. Small differences in floating point results between compiling 
 *      for 64-bit and 32-bit systems
 *
 * \section build_sec Building
 * This project uses GNU \c make to build executables. The generic project rules
 * are in \c make.rules. A top-level \c Makefile includes a \c .mk makefile
 * from each source directory. For example, here is the top-level \c Makefile:
\verbatim
SOURCE_DIRS=src docs
include make.rules
\endverbatim
 * and here is \c src/list/list.mk :
\verbatim
list_PROGRAMS := list_interp
PROGRAMS += $(list_PROGRAMS)
VPATH += src/list
list_interp_C_SOURCES := list.c list_wrapper.c
list_interp_STATIC_LIBRARIES := interp.a
$(call add_extra_CFLAGS_macro,$(list_interp_C_SOURCES),-Isrc/interp)
\endverbatim
 * The \c make.rules file defines compiler options for \c PORT=debug
 * (debugging), \c PORT=yacc (flex/bison debugging), \c PORT=sanitize
 * (address sanitiser), \c PORT=coverage, \c PORT=profile etc. The default
 * options are for the release build.
 *
 * Comments:
 *  1. I only suppress sanitizer and analyzer warnings from the generated
 *     interpreter files \c interp_lex.c and \c interp_yacc.c . However,
 *     compiling with \c -Wanalyzer-too-complex shows that \c -fanalyzer
 *     often bails out for other source files. For example with calls to
 *     the interpreter call-back functions like \c binaryHeap->debug(...) .
 *  2. The generated file \c interp_lex.c gives null dereferemce
 *     warnings when compiled with \c -fanalyzer
 *  3. The generated file \c interp_yacc.c gives run-time 
 *     warnings when compiled with \c -fsanitize=bounds-strict
 *  4. The address sanitiser build appears to give false positives when
 *     compiling with \c -O2 and \c -O3 optimisation.
 *
 * Build the \e html version of the \e doxygen documentation with:
\verbatim
$ make docs
\endverbatim
 *
 * \section test_sec Testing
 * I maintain \c librj with Peter Miller's \c aegis source code control
 * software. Each change is expected to pass the shell scrupts in
 * the \c test directory. These shell scripts run an interpreter test script
 * for the data structure being tested. In addition, each test script is
 * expected to run without warnings under the \c valgrind emulator or with
 * a \c -fsanitize build. Run the test scripts under \c valgrind by, for
 * example: 
\verbatim
VALGRIND_CMD="valgrind --leak-check=full" sh test/00/t0010a.sh
\endverbatim
 *
 * Initial tests were hard-coded in C. Next, I implemented a Python wrapper for
 * the list class. The result was cumbersome and not valgrind clean. I now use a
 * modified version of a simple interpreter shown in "A Compact Guide to Lex &
 * Yacc" by Thomas Niemann (see http://epaperpress.com/lexandyacc).
 * The interpreter is built from the files \c src/interp/interp_lex.l and
 * \c src/interp/interp_yacc.y with implementation details in
 * \c src/interp/interp_ex.c. The directory for each data structure
 * contains a "wrapper" file that is a "shim" between the implementation
 * of the data structure and the interpreter. The following
 * script shows the syntax for a test of \c redblackTree_interp:
\verbatim
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
\endverbatim
 *
 * Notes:
 *   1. 26 integer variables named \c a to \c z are allowed.
 *   2. Strings are contained within double quotes.
 *   3. \c # starts a comment.
 *   4. The interpreter uses \c size_t as the entry type. I assume \c void*
 *      and \c size_t have the same size. 
 *   5. The \c & and \c * operators work as for C
 *   6. \c print prints a signed value and \c uprint prints an unsigned value.
 *   7. \c rand(100) returns a random integer in the range 0 to 99.
 *   8. Failing to put a \c ; after the third statement in a \c for() is a
 *      syntax error.
 *
 * Test coverage is determined with GNU \c gcov and the \c -sPORT=coverage
 * build. For example, for \c list_t , \c test.in is from
 * \c test/00/t0055a.sh :
\verbatim
$ make PORT=coverage list_interp
$ bin/list_interp < test.in
$ gcov -o obj -s list src/list/list.c
$ less list.c.gcov
\endverbatim
 * Running all the hand-coded tests gave a test coverage of 85.53% for
 * \c src/list/list.c and 92.72% for \c src/redblackTree/redblackTree.c . 
 *
 * \todo Add a memory pool to the test interpreter
 * \todo Re-design the interpreter to enable unwinding of the expression tree
 *       after a syntax error.
 * \todo Test coverage could be improved by modifying the test interpreter so
 *       that the test can choose to fail memory allocation.
 *
 * \section perf_comp Performance comparison of binary tree implementations
 * The interpreters for the \c bsTree_t , \c redblackTree_t , \c sgTree_t ,
 * \c skipList_t , \c splayTree_t and \c swTree_t implementations were
 * benchmarked with this shell script:
\include benchmark.sh
 * The PC operating system was:
\verbatim
6.1.12-200.fc37.x86_64 GNU/Linux
\endverbatim
 * The CPU is an Intel i7-7700K 4.20GHz with 16GiB RAM. The compiler was Red Hat
 * GCC
 12.2.1-4 with options "-O3 -flto". The \c valgrind version is 3.20.0. The
 * binary files were stripped of debugging symbols. A simple test program 
 * shows \c sizeof(uintptr_t)==8. The preamble of the output from \c cg_annotate
 * running on a \c cachegrind output file is:
\verbatim
I1 cache:         32768 B, 64 B, 8-way associative
D1 cache:         32768 B, 64 B, 8-way associative
LL cache:         8388608 B, 64 B, 16-way associative
\endverbatim
 * The shell script \c benchmark_plot.sh extracts and plots the results.
 * The benchmark results are plotted on log-log axes with the x-axis being
 * the number of keys, N, inserted and the y-axis being the result divided by N.
 *
 * \subsection perf_random Performance comparison with random keys inserted
 * This section compares the performance of each tree implementation when the
 * keys inserted and deleted are both \c y=rand(k) . The benchmark results for
 * execution time are shown in the following plot:
@image html extime_random.svg
 *
 * The benchmark results for memory allocation are shown in the following plot:
@image html alloc_random.svg
 *
 * The benchmark results for instruction references are shown in the following
 * plot:
@image html Iref_random.svg
 *
 * The benchmark results for data reads are shown in the following plot:
@image html Drd_random.svg
 *
 * The benchmark results for data writes are shown in the following plot:
@image html Dwr_random.svg
 *
 * \subsection perf_sorted Performance comparison with sorted keys inserted
 * This section compares the performance of each tree implementation when the
 * keys inserted are sorted, \c y=x , and random keys are deleted, \c y=rand(k) .
 * The benchmark results for execution time are shown in the following plot:
@image html extime_sorted.svg
 *
 * The benchmark results for memory allocation are shown in the following plot:
@image html alloc_sorted.svg
 *
 * The benchmark results for instruction references are shown in the following
 * plot:
@image html Iref_sorted.svg
 *
 * The benchmark results for data reads are shown in the following plot:
@image html Drd_sorted.svg
 *
 * The benchmark results for data writes are shown in the following plot:
@image html Dwr_sorted.svg
 *
 * \subsection perf_comment Comments on the performance comparison
 *  1. I have not attempted to extract the contribution of the interpreter
 *     (particularly \c interpRand()) to these test results. Scanning the
 *     output of \c gprof suggests that the interpreter contribution can be
 *     neglected for tests with more than 10000 entries.
 *  2. These tests do not consider the effect of a larger "real-world" 
 *     executable with more I cache misses.
 *  3. The binary tree implementations variously use recursive and non-recursive
 *     \e insert and/or \e remove operations.
 *  4. For inserting random keys, the "knee" in execution time at about
 *     N=100000 is most likely due to the size of the LL cache.
 *  5. The values of \c depth_factor for the \c swTree_t and \c alpha for the
 *     \c sgTree_t have been set to values that avoid regular rebalancing of
 *     these trees with the random keys being inserted. Rebalancing these trees
 *     is an \f$ \mathcal{O}(N)\f$ operation that must be done sufficiently
 *     rarely that the \e amortised cost is \f$ \mathcal{O}(lg\; N)\f$.
 *     The \e Rivest and \e Galperin paper shows results for
 *     \f$ 0.55 \le \alpha \le 0.75 \f$. \f$ \alpha = 0.4 \f$ gives good
 *     results with random keys but fails with sorted inserted keys.
 *  6. I do not rebalance the \c splayTree_t . It appears that this is
 *     unnecessary with the random data used in this benchmark. 
 *  7. I do not attempt to pack the tree structures. For example, the
 *     single bit \c redblackTreeColour_e occupies 4 bytes and
 *     the \c redblackTreeNode_t is aligned so that it occupies 40 bytes.
 *
 * \section example_sec An example : Finding line segment intersections
 *
 * \b intersectList implements the algorithm for finding intersections of
 * line segments from Chapter 2 of "Computational Geometry: Algorithms and 
 * Applications", Second Edition, De Berg et al. Springer-Verlag, 
 * ISBN 3-540-65620-0.
 *
 * Given a list of line segments, the endpoints are stored as "events" in
 * the event queue, a balanced tree ordered by position. The "upper" 
 * endpoints are stored with a list of segments sharing that upper endpoint.
 * Events are considered in order. Segments that lie on the current event
 * sweep line are stored in the status tree, a balanced tree ordered by
 * intersection with the sweep line and segment slope. The status tree is
 * initially empty.
 *
 * For each event:
 *
 * 1. Find all segments stored in the status tree that contain the
 * \c event ; they are adjacent. Let \c lower(event) denote the subset of
 * segments found whose lower endpoint is \c event, and let
 * \c interior(event) denote the subset of segments found that contain
 * \c event in their interior and let \c upper(event) denote the subset of
 * segments whose upper endpoint is \c event. Note that, by design,
 * segments in \c upper(event) cannot appear in the status tree since
 * they have not been encountered yet.
 * 2. If <CODE>upper(event)+lower(event)+interior(event)</CODE>contains more
 * than one segment then create an intersection point for \c event. 
 * 3. Delete the segments in <CODE>lower(event)+interior(event)</CODE>
 * from the status tree.
 * 4. Insert the segments in <CODE>upper(event)+interior(event)</CODE> into
 * the status tree, The order of the segments in the status tree should
 * correspond to the order in which they are intersected by a sweep
 * line just below \c event. If there is a horizontal segment, it
 * comes last among all segments containing \c event. (Deleting and
 * re-inserting the segments of \c interior(event) reverses their order.)
 * 5. If <CODE>upper(event)+interior(event)</CODE> is empty then:
 *    - let \c sl and \c sr be the left and right neighbours of \c event in
 *      the status tree
 *    - check for a new event point at the intersection of \c sl and \c sr
 *
 * otherwise:
 *
 *    1. let \c s' be the leftmost segment of 
 *      <CODE>upper(event)+interior(event)</CODE> in the tree 
 *      and \c sl be the left neighbour of \c s' in the tree
 *    2. check for a new event point at the intersection of \c s' and \c sl
 *    3. let \c s'' be the rightmost segment of 
 *      <CODE>upper(event)+interior(event)</CODE>
 *      in the tree and \c sr be the right neighbour of \c s'' in the tree
 *    4. check for a new event point at the intersection of \c s'' and \c sr
 * .
 * The test program for \b intersectList, in file \c intersectList_test.c ,
 * reads and writes <I>PostScript</I> files. The test program 
 * \c intersectList_random_test.c generates a random list of line segments and
 * writes a <I>PostScript</I> file. The following figure shows an
 * example of the output from \c intersectList_random_test.c :
@image html intersectList_random_test.svg width=600px
 *
 * This image was created with:
\verbatim
$ bin/intersectList_random_test > intersectList_random_test.ps
$ ps2epsi intersectList_random_test.ps intersectList_random_test.epsi
$ convert -monochrome -density 300 -units pixelsperinch \
    intersectList_random_test.epsi intersectList_random_test.svg
\endverbatim
 */

#endif
