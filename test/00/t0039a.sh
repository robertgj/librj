#!/bin/sh
#
prog="intersectList_test"
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
%!PS
0 0 moveto 100 100 lineto stroke
100 100 moveto 200 200 lineto stroke
0 100 moveto 100 100 lineto stroke
100 100 moveto 200 100 lineto stroke
0 200 moveto 100 100 lineto stroke
100 100 moveto 200 0 lineto stroke
100 0 moveto 100 100 lineto stroke
100 100 moveto 100 200 lineto stroke
showpage
EOF
if [ $? -ne 0 ]; then echo "Failed input cat"; fail; fi

#
# the output should look like this
#
cat > test.ok << 'EOF'
%!PS
0 0 moveto 100 100 lineto stroke
100 100 moveto 200 200 lineto stroke
0 100 moveto 100 100 lineto stroke
100 100 moveto 200 100 lineto stroke
0 200 moveto 100 100 lineto stroke
100 100 moveto 200 0 lineto stroke
100 0 moveto 100 100 lineto stroke
100 100 moveto 100 200 lineto stroke
100 100 3 0 360 arc fill
showpage
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
