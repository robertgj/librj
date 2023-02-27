#!/bin/sh
#
prog="splayCache_interp"
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
# Check splayCache balance and depth

k=100;

l=create();
for (x=0;x<k;x=x+1;)
{
  insert(l, x);
}
"Size";x=size(l);print x;
"Depth"; x=depth(l); print x;
"Check";x=check(l); print x;
"Balance";x=balance(l); print x;
"Size";x=size(l);print x;
"Depth"; x=depth(l); print x;
"Check";x=check(l); print x;
destroy(l);

l=create();
s=0;
while (s<(k-1))
{
  insert(l, rand(k));
  s=size(l);
}
"Size";x=size(l);print x;
"Depth"; x=depth(l); print x;
"Check";x=check(l); print x;
"Balance"; x=balance(l); print x;
"Size";x=size(l);print x;
"Depth"; x=depth(l); print x;
"Check";x=check(l); print x;
destroy(l);

exit;
EOF
if [ $? -ne 0 ]; then echo "Failed input cat"; fail; fi

#
# the output should look like this
#
cat > test.ok << 'EOF'
Size
100
Depth
100
Check
1
Balance
1
Size
100
Depth
7
Check
1
Size
99
Depth
18
Check
1
Balance
1
Size
99
Depth
7
Check
1
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
