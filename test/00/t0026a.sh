#!/bin/sh
#
prog="list_interp"
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
# Test file for listGetNext() and listGetPrevious()

l=create();
"Insert";
for(x=0; x<10; x=x+1;)
{
  insert(l, x);
}

"Find";
x = 2;
p = find(l,x);
if (p == 0)
{
  "Didn't find "; print x;
}
else
{
  "Found "; print *p;
}

"Size";
x=size(l);
print x;

"First";
p=first(l);
print *p;
p=next(l, p);
print *p;
p=first(l);
print *p;
p=previous(l, p);
print *p;

"Last";
p=last(l);
print *p;
p=next(l, p);
print *p;
p=last(l);
print *p;
p=previous(l, p);
print *p;

"Next";
p=first(l); print *p;
for(x=0; x<11; x=x+1;)
{
  p = next(l, p); print *p;
}

"Previous";
p=last(l); print *p;
for(x=0; x<11; x=x+1;)
{
  p = previous(l, p); print *p;
}
"Walk";
walk(l, show);

"Destroy";
destroy(l);
EOF
if [ $? -ne 0 ]; then echo "Failed input cat"; fail; fi

#
# the output should look like this
#
cat > test.ok << 'EOF'
Insert
Find
Found 
2
Size
10
First
0
1
0
NULL pointer!
0
Last
9
NULL pointer!
0
9
8
Next
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
NULL pointer!
0
NULL pointer!
0
Previous
9
8
7
6
5
4
3
2
1
0
NULL pointer!
0
NULL pointer!
0
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
Destroy
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
