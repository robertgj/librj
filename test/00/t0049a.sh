#!/bin/sh
#
prog="skipList_interp"
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
# Test file for skipList_t

# Initial simple test
s=create();
"Size : "; print size(s);
"Insert and size";
for(x=1; x<=10; x=x+1;)
{
  insert(s,x);
  "Size : "; print size(s);
}
"Find";
for(x=0; x<10; x=x+1;)
{
  y=rand(10);
  z=find(s,y);
  "Found z= "; print *z;
}

"Insert";
insert(s,20);
z=find(s,20);
"Found z= "; print *z;

"Remove";
z=find(s,2);
remove(s,z);
z=find(s,2);
if (z != 0)
{
 "Found z= "; print *z;
}

"Min";
z=min(s);
print *z;

"Next";
z=min(s);
for(x=0; x<10; x=x+1;)
{
  print *z;
  z=next(s,z);
}
if (z != 0)
{
  print "Expected z==0!";
}
else
{
  print "z==0 (as expected)";
}

"Max";
z=max(s);
print *z;

"Previous";
z=max(s);
for(x=0; x<10; x=x+1;)
{
  print *z;
  z=previous(s,z);
}
if (z != 0)
{
  print "Expected z==0!";
}
else
{
  print "z==0 (as expected)";
}

"Walk";
walk(s,show);
insert(s,3);
walk(s,show);

"Check";
print check(s);

"Clear";
clear(s);
"Size:";
print size(s);

"Check";
print check(s);

"Destroy";
destroy(s);
"Complete";

# Another test
x=100000;
s=create();
for (i=x;i>0;i=i-1;) {
  insert(s,i);
}
for (i=0;i<x;i=i+1;) {
  insert(s,i);
}
"Size: "; print size(s);
"Check: "; print check(s);
for (i=1;i<=x;i=i+1;) {
  z=find(s,i);
  if (*z != i) {
    print "find(s,i) != i "; print i;
  }
  remove(s,z);
}
"Size: "; print size(s);
"Check: "; print check(s);
"Clear";
clear(s);
"Size:";
print size(s);
"Destroy";
destroy(s);
"Complete";

# Random test 1
x=100000;
s=create();
for (i=0;i<x;i=i+1;)
{
  if (rand(x) > (x/2)) { 
    y=rand(x); 
    insert(s,y);
  }
  if (rand(x) > (x/2)) {
    if (size(s) > 0) {
      y=rand(x);
      z=find(s,y);
      if (z != 0) {
        remove(s,z);
      }
    }
  }
}
"Size: "; print size(s);
"Check: "; print check(s);
"Destroy";
destroy(s);

# Random test 2
x=100000;
s=create();
"Size: "; print size(s);
for (i=0;i<x;i=i+1;) {
  y=rand(x); 
  insert(s,y);
}
"Size: "; print size(s);
"Check: "; print check(s);
"Clear";
clear(s);
"Size: "; print size(s);
"Check: "; print check(s);
"Destroy";
destroy(s);

# Test next and previous
s=create();
print next(s,0);
print previous(s,0);
insert(s,1);
x=find(s,1);
print next(s,x);
print previous(s,x);
destroy(s);

# Test NULL pointers
s=create();
print find(0,0);
print insert(0,0);
print remove(0,0);
print remove(s,0);
print size(0);
print min(0);
print max(0);
print next(0,0);
print next(s,0);
print previous(0,0);
print previous(s,0);
print walk(0,show);
print walk(0,0);
print check(0);
clear(0);
destroy(0);
destroy(s);
exit;
"Complete";
EOF
if [ $? -ne 0 ]; then echo "Failed input cat"; fail; fi

#
# the output should look like this
#
cat > test.ok << 'EOF'
Size : 
0
Insert and size
Size : 
1
Size : 
2
Size : 
3
Size : 
4
Size : 
5
Size : 
6
Size : 
7
Size : 
8
Size : 
9
Size : 
10
Find
Found z= 
4
Found z= 
8
Found z= 
NULL pointer!
0
Found z= 
2
Found z= 
NULL pointer!
0
Found z= 
3
Found z= 
6
Found z= 
6
Found z= 
7
Found z= 
2
Insert
Found z= 
20
Remove
Min
1
Next
1
3
4
5
6
7
8
9
10
20
z==0 (as expected)
0
Max
20
Previous
20
10
9
8
7
6
5
4
3
1
z==0 (as expected)
0
Walk
1
3
4
5
6
7
8
9
10
20
1
3
3
4
5
6
7
8
9
10
20
Check
1
Clear
Size:
0
Check
1
Destroy
Complete
Size: 
200000
Check: 
1
Size: 
100000
Check: 
1
Clear
Size:
0
Destroy
Complete
Size: 
40852
Check: 
1
Destroy
Size: 
0
Size: 
100000
Check: 
1
Clear
Size: 
0
Check: 
1
Destroy
0
0
0
0
0
0
0
0
0
0
0
0
0
0
0
0
Bad walk function
0
0
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
