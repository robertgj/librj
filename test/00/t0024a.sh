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
# Test file for list_t listCopy()

l=create();

"Insert";
for(x=0; x<20; x=x+1;)
{
  insert(l, x);
}

"List l";
walk(l, show);

m=create();

"Insert";
for(x=20; x<40; x=x+1;)
{
  insert(m, x);
}

"List m";
walk(m, show);

"Clear";
clear(l);

"Size l"; print size(l);

"Copy";
copy(l, m);

"Size l"; print size(l);
walk(l, show);

"Size m"; print size(m);
walk(m, show);

"Size m"; print size(m);

"Copy";
copy(m, l);

"Size l"; print size(l);
walk(l, show);

"Size m"; print size(m);
walk(m, show);

"Destroy";
destroy(l);
destroy(m);
EOF
if [ $? -ne 0 ]; then echo "Failed input cat"; fail; fi

#
# the output should look like this
#
cat > test.ok << 'EOF'
Insert
List l
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
15
16
17
18
19
Insert
List m
20
21
22
23
24
25
26
27
28
29
30
31
32
33
34
35
36
37
38
39
Clear
Size l
0
Copy
Size l
20
20
21
22
23
24
25
26
27
28
29
30
31
32
33
34
35
36
37
38
39
Size m
20
20
21
22
23
24
25
26
27
28
29
30
31
32
33
34
35
36
37
38
39
Size m
20
Copy
Size l
20
20
21
22
23
24
25
26
27
28
29
30
31
32
33
34
35
36
37
38
39
Size m
20
20
21
22
23
24
25
26
27
28
29
30
31
32
33
34
35
36
37
38
39
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
