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
# Test file for NULL pointers in list_t
l=create();
print find(0,0);
print find(l,0);
print insert(0,0);
print remove(0,0);
print remove(l,0);
print clear(0);
print clear(l);
destroy(0);
print size(0);
print size(l);
print first(0);
print last(0);
print next(0,0);
print next(l,0);
print previous(0,0);
print previous(l,0);
print walk(0,0);
print walk(l,0);
print copy(0,0);
print copy(l,0);
m=create();
copy(l,m);
destroy(l);
destroy(m);
EOF
if [ $? -ne 0 ]; then echo "Failed input cat"; fail; fi

#
# the output should look like this
#
cat > test.ok << 'EOF'
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
Bad walk function
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
