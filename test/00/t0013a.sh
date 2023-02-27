#!/bin/sh
#
prog="splayTree_interp"
tmp=/tmp/$$
here=`pwd`
bin=$here/bin
if [ $? -ne 0 ]; then echo "Failed pwd"; exit 1; fi


fail()
{
        echo FAILED $prog 1>&2
        cd $here
        rm -rf $tmp
        exit 1
}


pass()
{
        echo PASSED $prog
        cd $here
        rm -rf $tmp
        exit 0
}

trap "fail" 1 2 3 15
mkdir $tmp
if [ $? -ne 0 ]; then echo "Failed mkdir"; exit 1; fi
cd $tmp
if [ $? -ne 0 ]; then echo "Failed cd"; fail; fi

#
# the input should look like this
#
cat > test.in << 'EOF'
# Some more splay tree tests
# More tests for splayTree_t balance function

  l=create();

  "Size";x=size(l);print x;"Balance"; x=balance(l); print x;
  "Check";x=check(l); print x;"Depth"; x=depth(l); print x;

  x = rand(2000);
  insert(l, x);
  "Size";x=size(l);print x;"Balance"; x=balance(l); print x;
  "Check";x=check(l); print x;"Depth"; x=depth(l); print x;


  x = rand(2000);
  insert(l, x);
  "Size";x=size(l);print x;"Balance"; x=balance(l); print x;
  "Check";x=check(l); print x;"Depth"; x=depth(l); print x;

  x = rand(2000);
  insert(l, x);
  "Size";x=size(l);print x;"Balance"; x=balance(l); print x;
  "Check";x=check(l); print x;"Depth"; x=depth(l); print x;

  for(x=0;x<15;x=x+1;)
  {
    insert(l, x);
  }

  "Check";x=check(l); print x;
  "Size";x=size(l);print x; 
  "Depth"; x=depth(l); print x;
  "Balance"; x=balance(l); print x;

  for(x=0;x<15;x=x+1;)
  {
    insert(l, x);
  }

  "Check";x=check(l); print x;
  "Size";x=size(l);print x; 
  "Depth"; x=depth(l); print x;
  "Balance"; x=balance(l); print x;
  "Check";x=check(l); print x;
  "Size";x=size(l);print x; 
  "Depth"; x=depth(l); print x;

  "Find";
  for(x=0;x<15;x=x+1;)
  {
    p=find(l,x); print *p;
  }

  "Check";x=check(l); print x;
  "Size";x=size(l); print x;
  "Depth"; x=depth(l); print x;
  "Walk"; walk(l, show);
  "Balance"; x=balance(l); print x;
  "Check";x=check(l); print x;
  "Size";x=size(l); print x;
  "Depth"; x=depth(l); print x;
  "Walk"; walk(l, show);
  "Check";x=check(l); print x;
  "Size";x=size(l);print x;
  "Depth"; x=depth(l); print x;

  destroy(l);

EOF
if [ $? -ne 0 ]; then echo "Failed input cat"; fail; fi

#
# the output should look like this
#
cat > test.ok << 'EOF'
Size
0
Balance
0
Check
0
Depth
0
Size
1
Balance
1
Check
1
Depth
1
Size
2
Balance
1
Check
1
Depth
2
Size
3
Balance
1
Check
1
Depth
2
Check
1
Size
18
Depth
15
Balance
1
Check
1
Size
18
Depth
15
Balance
1
Check
1
Size
18
Depth
5
Find
0
1
2
3
4
5
6
7
8
9
10
11
12
13
14
Check
1
Size
18
Depth
15
Walk
0
1
2
3
4
5
6
7
8
9
10
11
12
13
77
1798
1000
14
Balance
1
Check
1
Size
18
Depth
5
Walk
1
0
3
5
4
7
9
8
6
11
13
12
77
1798
1000
14
10
2
Check
1
Size
18
Depth
5
EOF
if [ $? -ne 0 ]; then echo "Failed output cat"; fail; fi


#
# run and see if the results match
#
$VALGRIND_CMD $bin/$prog <test.in >test.out
if [ $? -ne 0 ]; then echo "Failed running $prog"; fail; fi
diff test.ok test.out
if [ $? -ne 0 ]; then echo "Failed diff"; fail; fi


#
# this much worked
#
pass
