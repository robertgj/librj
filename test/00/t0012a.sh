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
# Test file for splayTree_t balance function

d=3;
for (a=16;a<=2048;a=a*2;)
{
  d = d+1;

  l=create();
  for (x=0;x<a-2;;)
  {
    y = rand(20000);
    p = find (l, y);
    if (p == 0)
    {
      insert(l, y);
      x = x+1;
    }
  }

  "Balance"; x=balance(l); print x;

  "Size"; x=size(l); print x;
  "Check"; x=check(l); print x;
  "Depth"; x=depth(l); print x;
  if (x != d) "Bad depth!";

  destroy(l);

  l=create();
  for (x=0;x<a-1;;)
  {
    y = rand(20000);
    p = find (l, y);
    if (p == 0)
    {
      insert(l, y);
      x = x+1;
    }
  }

  "Balance"; x=balance(l); print x;

  "Size"; x=size(l); print x;
  "Check"; x=check(l); print x;
  "Depth"; x=depth(l); print x;
   if (x != d) "Bad depth!";

  destroy(l);

  l=create();
  for (x=0;x<a;;)
  {
    y = rand(20000);
    p = find (l, y);
    if (p == 0)
    {
      insert(l, y);
      x = x+1;
    }
  }

  "Balance"; x=balance(l); print x;

  "Size"; x=size(l); print x;
  "Check"; x=check(l); print x;
  "Depth"; x=depth(l); print x;
   if (x != d+1) "Bad depth!";

  destroy(l);

  l=create();
  for (x=0;x<a+1;;)
  {
    y = rand(20000);
    p = find (l, y);
    if (p == 0)
    {
      insert(l, y);
      x = x+1;
    }
  }

  "Balance"; x=balance(l); print x;

  "Size"; x=size(l); print x;
  "Check"; x=check(l); print x;
  "Depth"; x=depth(l); print x;
   if (x != d+1) "Bad depth!";

  destroy(l);
}
EOF
if [ $? -ne 0 ]; then echo "Failed input cat"; fail; fi

#
# the output should look like this
#
cat > test.ok << 'EOF'
Balance
1
Size
14
Check
1
Depth
4
Balance
1
Size
15
Check
1
Depth
4
Balance
1
Size
16
Check
1
Depth
5
Balance
1
Size
17
Check
1
Depth
5
Balance
1
Size
30
Check
1
Depth
5
Balance
1
Size
31
Check
1
Depth
5
Balance
1
Size
32
Check
1
Depth
6
Balance
1
Size
33
Check
1
Depth
6
Balance
1
Size
62
Check
1
Depth
6
Balance
1
Size
63
Check
1
Depth
6
Balance
1
Size
64
Check
1
Depth
7
Balance
1
Size
65
Check
1
Depth
7
Balance
1
Size
126
Check
1
Depth
7
Balance
1
Size
127
Check
1
Depth
7
Balance
1
Size
128
Check
1
Depth
8
Balance
1
Size
129
Check
1
Depth
8
Balance
1
Size
254
Check
1
Depth
8
Balance
1
Size
255
Check
1
Depth
8
Balance
1
Size
256
Check
1
Depth
9
Balance
1
Size
257
Check
1
Depth
9
Balance
1
Size
510
Check
1
Depth
9
Balance
1
Size
511
Check
1
Depth
9
Balance
1
Size
512
Check
1
Depth
10
Balance
1
Size
513
Check
1
Depth
10
Balance
1
Size
1022
Check
1
Depth
10
Balance
1
Size
1023
Check
1
Depth
10
Balance
1
Size
1024
Check
1
Depth
11
Balance
1
Size
1025
Check
1
Depth
11
Balance
1
Size
2046
Check
1
Depth
11
Balance
1
Size
2047
Check
1
Depth
11
Balance
1
Size
2048
Check
1
Depth
12
Balance
1
Size
2049
Check
1
Depth
12
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
