#!/bin/sh
#
prog="stack_interp"
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
# Test file for stack_t

# First simple test 
l=create();
for(x=0; x<20; x=x+1;)
{
  y = rand(500);
  "Push"; 
  push(l, y); print y;
  "Size"; 
  z=size(l); print z;
}

"Walk";
walk (l, show);

for(x=0; x<21; x=x+1;)
{
  "Pop";
  pop(l);  print y;
  "Size";
  z=size(l); print z;
}

"Walk";
walk(l,show);

"Destroy";
destroy(l);

# Test destroy
l=create();
y = rand(500);
"y=";print y;
z=push(l, y);
"After push *z=";print *z;
y = rand(500);
"y=";print y;
z=push(l, y);
"After push *z=";print *z;
v=peek(l);
"After peek v=";print v;
"Pop";
pop(l);
"Size";
print size(l);
"Destroy";
destroy(l);

# Test clear
l=create();

for(x=0; x<15; x=x+1;)
{
  y = rand(500);
  "Push"; 
  push(l, y); print y;
  "Size";
  z=size(l); print z;
}

"Walk";
walk (l, show);

for(x=0; x<10; x=x+1;)
{
  "Peek " ;
  y = peek(l);   print y;
  "Size";
  z=size(l); print z;
  "Pop " ;
  pop(l);
  "Size";
  z=size(l); print z;
}

"Walk";
walk(l,show);

"Clear";
clear(l);

"Walk";
walk(l,show);

"Destroy";
destroy(l);

# Test for NULL pointers
print push(0,0);
print pop(0);
print peek(0);
print size(0);
print walk(0,show);
clear(0);
destroy(0);

# Done
exit;
EOF
if [ $? -ne 0 ]; then echo "Failed input cat"; fail; fi

#
# the output should look like this
#
cat > test.ok << 'EOF'
Push
249
Size
1
Push
449
Size
2
Push
19
Size
3
Push
118
Size
4
Push
34
Size
5
Push
215
Size
6
Push
361
Size
7
Push
362
Size
8
Push
399
Size
9
Push
125
Size
10
Push
435
Size
11
Push
154
Size
12
Push
160
Size
13
Push
166
Size
14
Push
376
Size
15
Push
54
Size
16
Push
310
Size
17
Push
207
Size
18
Push
108
Size
19
Push
189
Size
20
Walk
249
449
19
118
34
215
361
362
399
125
435
154
160
166
376
54
310
207
108
189
Pop
189
Size
19
Pop
189
Size
18
Pop
189
Size
17
Pop
189
Size
16
Pop
189
Size
15
Pop
189
Size
14
Pop
189
Size
13
Pop
189
Size
12
Pop
189
Size
11
Pop
189
Size
10
Pop
189
Size
9
Pop
189
Size
8
Pop
189
Size
7
Pop
189
Size
6
Pop
189
Size
5
Pop
189
Size
4
Pop
189
Size
3
Pop
189
Size
2
Pop
189
Size
1
Pop
189
Size
0
Pop
189
Size
0
Walk
Destroy
y=
166
After push *z=
166
y=
310
After push *z=
310
After peek v=
310
Pop
Size
1
Destroy
Push
215
Size
1
Push
3
Size
2
Push
95
Size
3
Push
44
Size
4
Push
346
Size
5
Push
111
Size
6
Push
340
Size
7
Push
389
Size
8
Push
291
Size
9
Push
342
Size
10
Push
494
Size
11
Push
339
Size
12
Push
247
Size
13
Push
266
Size
14
Push
249
Size
15
Walk
215
3
95
44
346
111
340
389
291
342
494
339
247
266
249
Peek 
249
Size
15
Pop 
Size
14
Peek 
266
Size
14
Pop 
Size
13
Peek 
247
Size
13
Pop 
Size
12
Peek 
339
Size
12
Pop 
Size
11
Peek 
494
Size
11
Pop 
Size
10
Peek 
342
Size
10
Pop 
Size
9
Peek 
291
Size
9
Pop 
Size
8
Peek 
389
Size
8
Pop 
Size
7
Peek 
340
Size
7
Pop 
Size
6
Peek 
111
Size
6
Pop 
Size
5
Walk
215
3
95
44
346
Clear
Walk
Destroy
0
0
NULL pointer!
0
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
