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
60 60 moveto 110 110 lineto stroke
160 60 moveto 110 110 lineto stroke
160 60 moveto 110 10 lineto stroke
60 60 moveto 110 10 lineto stroke
60 60 moveto 10 60 lineto stroke
110 170 moveto 110 110 lineto stroke
220 60 moveto 160 60 lineto stroke
110 10 moveto 110 0 lineto stroke
85 85 moveto 70 100 lineto stroke
135 85 moveto 150 100 lineto stroke
135 35 moveto 150 20 lineto stroke
85 35 moveto 70 20 lineto stroke
showpage
EOF
if [ $? -ne 0 ]; then echo "Failed input cat"; fail; fi

#
# the output should look like this
#
cat > test.ok << 'EOF'
%!PS
60 60 moveto 110 110 lineto stroke
160 60 moveto 110 110 lineto stroke
160 60 moveto 110 10 lineto stroke
60 60 moveto 110 10 lineto stroke
60 60 moveto 10 60 lineto stroke
110 170 moveto 110 110 lineto stroke
220 60 moveto 160 60 lineto stroke
110 10 moveto 110 0 lineto stroke
85 85 moveto 70 100 lineto stroke
135 85 moveto 150 100 lineto stroke
135 35 moveto 150 20 lineto stroke
85 35 moveto 70 20 lineto stroke
110 110 3 0 360 arc fill
135 85 3 0 360 arc fill
85 85 3 0 360 arc fill
160 60 3 0 360 arc fill
60 60 3 0 360 arc fill
135 35 3 0 360 arc fill
85 35 3 0 360 arc fill
110 10 3 0 360 arc fill
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
