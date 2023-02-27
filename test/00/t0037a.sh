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
130 180 moveto 184  42 lineto stroke
403 156 moveto  28 111 lineto stroke
172 474 moveto 351 105 lineto stroke
324  82 moveto 472 463 lineto stroke
143 127 moveto 475 411 lineto stroke
220 352 moveto  85  56 lineto stroke
295 250 moveto 178 129 lineto stroke
showpage
EOF
if [ $? -ne 0 ]; then echo "Failed input cat"; fail; fi

#
# the output should look like this
#
cat > test.ok << 'EOF'
%!PS
130 180 moveto 184 42 lineto stroke
403 156 moveto 28 111 lineto stroke
172 474 moveto 351 105 lineto stroke
324 82 moveto 472 463 lineto stroke
143 127 moveto 475 411 lineto stroke
220 352 moveto 85 56 lineto stroke
295 250 moveto 178 129 lineto stroke
440.255 381.279 3 0 360 arc fill
282.458 246.296 3 0 360 arc fill
285.452 240.125 3 0 360 arc fill
135.335 166.365 3 0 360 arc fill
350.288 149.675 3 0 360 arc fill
330.482 147.298 3 0 360 arc fill
148.798 131.96 3 0 360 arc fill
178 129 3 0 360 arc fill
151.214 125.786 3 0 360 arc fill
340.968 125.681 3 0 360 arc fill
114.837 121.42 3 0 360 arc fill
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
