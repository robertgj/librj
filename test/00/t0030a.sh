#!/bin/sh
#
prog="statusTree_test"
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
EOF
if [ $? -ne 0 ]; then echo "Failed input cat"; fail; fi

#
# the output should look like this
#
cat > test.ok << 'EOF'

status tree order test 0:
Segment a=(0,0) b=(1,1)
Segment a=(1,0) b=(0,1)
Segment a=(1,0) b=(2,1)
Segment a=(2,0) b=(1,1)
Segment a=(0,1) b=(0,0)
Segment a=(0,0) b=(1,0)
Segment a=(-1,0) b=(0,0)
Segment a=(0.5,0) b=(0.5,1)
Segment a=(0,0) b=(0.5,1)
Segment a=(0.75,0) b=(0.25,1)
Segment a=(0.25,0) b=(0.75,1)
Segment a=(0.4,0) b=(0.6,1)
Segment a=(0.6,0) b=(0.4,1)
Segment a=(0.55,0) b=(0.45,1)
Segment a=(0.45,0) b=(0.55,1)

15 segments
  segment (-1,0) (0,0)
  sweep intersection at (0,0)
  segment (0,0) (1,0)
  sweep intersection at (0,0)
  segment (0,0) (1,1)
  sweep intersection at (0,0)
  segment (0,0) (0.5,1)
  sweep intersection at (0,0)
  segment (0,1) (0,0)
  sweep intersection at (0,0)
  segment (0.25,0) (0.75,1)
  sweep intersection at (0.25,0)
  segment (0.4,0) (0.6,1)
  sweep intersection at (0.4,0)
  segment (0.45,0) (0.55,1)
  sweep intersection at (0.45,0)
  segment (0.5,0) (0.5,1)
  sweep intersection at (0.5,0)
  segment (0.55,0) (0.45,1)
  sweep intersection at (0.55,0)
  segment (0.6,0) (0.4,1)
  sweep intersection at (0.6,0)
  segment (0.75,0) (0.25,1)
  sweep intersection at (0.75,0)
  segment (1,0) (2,1)
  sweep intersection at (1,0)
  segment (1,0) (0,1)
  sweep intersection at (1,0)
  segment (2,0) (1,1)
  sweep intersection at (2,0)

statusTreeGetUpper():
Segment a=(0,1) b=(0,0)
statusTreeGetLower():
Segment a=(0,1) b=(0,0)
statusTreeGetLeftMost():
Segment a=(-1,0) b=(0,0)
statusTreeGetRightMost():
Segment a=(0,1) b=(0,0)
statusTreeGetNext():
Segment a=(0.25,0) b=(0.75,1)
statusTreeGetPrev():
Segment a=(0,0) b=(0.5,1)

status tree order test 1:
Not using:
Segment a=(0,0) b=(1,0)
Not using:
Segment a=(-1,0) b=(0,0)
Segment a=(0,0) b=(1,1)
Segment a=(1,0) b=(0,1)
Segment a=(1,0) b=(2,1)
Segment a=(2,0) b=(1,1)
Segment a=(0,1) b=(0,0)
Segment a=(0.5,0) b=(0.5,1)
Segment a=(0,0) b=(0.5,1)
Segment a=(0.75,0) b=(0.25,1)
Segment a=(0.25,0) b=(0.75,1)
Segment a=(0.4,0) b=(0.6,1)
Segment a=(0.6,0) b=(0.4,1)
Segment a=(0.55,0) b=(0.45,1)
Segment a=(0.45,0) b=(0.55,1)

13 segments
  segment (0,1) (0,0)
  sweep intersection at (0,1)
  segment (1,0) (0,1)
  sweep intersection at (0,1)
  segment (0.75,0) (0.25,1)
  sweep intersection at (0.25,1)
  segment (0.6,0) (0.4,1)
  sweep intersection at (0.4,1)
  segment (0.55,0) (0.45,1)
  sweep intersection at (0.45,1)
  segment (0,0) (0.5,1)
  sweep intersection at (0.5,1)
  segment (0.5,0) (0.5,1)
  sweep intersection at (0.5,1)
  segment (0.45,0) (0.55,1)
  sweep intersection at (0.55,1)
  segment (0.4,0) (0.6,1)
  sweep intersection at (0.6,1)
  segment (0.25,0) (0.75,1)
  sweep intersection at (0.75,1)
  segment (0,0) (1,1)
  sweep intersection at (1,1)
  segment (2,0) (1,1)
  sweep intersection at (1,1)
  segment (1,0) (2,1)
  sweep intersection at (2,1)

statusTreeGetUpper():
Segment a=(1,0) b=(0,1)
statusTreeGetLower():
Segment a=(1,0) b=(0,1)
statusTreeGetLeftMost():
Segment a=(0,1) b=(0,0)
statusTreeGetRightMost():
Segment a=(1,0) b=(0,1)
statusTreeGetNext():
Segment a=(0.75,0) b=(0.25,1)
statusTreeGetPrev():
Segment a=(0,1) b=(0,0)

status tree order test 2:
Not using:
Segment a=(0,0) b=(1,0)
Not using:
Segment a=(-1,0) b=(0,0)
Segment a=(0,0) b=(1,1)
Segment a=(1,0) b=(0,1)
Segment a=(1,0) b=(2,1)
Segment a=(2,0) b=(1,1)
Segment a=(0,1) b=(0,0)
Segment a=(0.5,0) b=(0.5,1)
Segment a=(0,0) b=(0.5,1)
Segment a=(0.75,0) b=(0.25,1)
Segment a=(0.25,0) b=(0.75,1)
Segment a=(0.4,0) b=(0.6,1)
Segment a=(0.6,0) b=(0.4,1)
Segment a=(0.55,0) b=(0.45,1)
Segment a=(0.45,0) b=(0.55,1)

13 segments
  segment (0,1) (0,0)
  sweep intersection at (0,0.5)
  segment (0,0) (0.5,1)
  sweep intersection at (0.25,0.5)
  segment (0,0) (1,1)
  sweep intersection at (0.5,0.5)
  segment (0.25,0) (0.75,1)
  sweep intersection at (0.5,0.5)
  segment (0.4,0) (0.6,1)
  sweep intersection at (0.5,0.5)
  segment (0.45,0) (0.55,1)
  sweep intersection at (0.5,0.5)
  segment (0.5,0) (0.5,1)
  sweep intersection at (0.5,0.5)
  segment (0.55,0) (0.45,1)
  sweep intersection at (0.5,0.5)
  segment (0.6,0) (0.4,1)
  sweep intersection at (0.5,0.5)
  segment (0.75,0) (0.25,1)
  sweep intersection at (0.5,0.5)
  segment (1,0) (0,1)
  sweep intersection at (0.5,0.5)
  segment (1,0) (2,1)
  sweep intersection at (1.5,0.5)
  segment (2,0) (1,1)
  sweep intersection at (1.5,0.5)

statusTreeGetUpper():
Segment a=(0.5,0) b=(0.5,1)
statusTreeGetLower():
Segment a=(0.5,0) b=(0.5,1)
statusTreeGetLeftMost():
Segment a=(0,0) b=(1,1)
statusTreeGetRightMost():
Segment a=(1,0) b=(0,1)
statusTreeGetNext():
Segment a=(1,0) b=(2,1)
statusTreeGetPrev():
Segment a=(0.75,0) b=(0.25,1)

status tree order test 3:
Not using:
Segment a=(0,0) b=(1,0)
Not using:
Segment a=(-1,0) b=(0,0)
Segment a=(0,0) b=(1,1)
Segment a=(1,0) b=(0,1)
Segment a=(1,0) b=(2,1)
Segment a=(2,0) b=(1,1)
Segment a=(0,1) b=(0,0)
Segment a=(0.5,0) b=(0.5,1)
Segment a=(0,0) b=(0.5,1)
Segment a=(0.75,0) b=(0.25,1)
Segment a=(0.25,0) b=(0.75,1)
Segment a=(0.4,0) b=(0.6,1)
Segment a=(0.6,0) b=(0.4,1)
Segment a=(0.55,0) b=(0.45,1)
Segment a=(0.45,0) b=(0.55,1)

13 segments
  segment (0,1) (0,0)
  sweep intersection at (0,0.25)
  segment (0,0) (0.5,1)
  sweep intersection at (0.125,0.25)
  segment (0,0) (1,1)
  sweep intersection at (0.25,0.25)
  segment (0.25,0) (0.75,1)
  sweep intersection at (0.375,0.25)
  segment (0.4,0) (0.6,1)
  sweep intersection at (0.45,0.25)
  segment (0.45,0) (0.55,1)
  sweep intersection at (0.475,0.25)
  segment (0.5,0) (0.5,1)
  sweep intersection at (0.5,0.25)
  segment (0.55,0) (0.45,1)
  sweep intersection at (0.525,0.25)
  segment (0.6,0) (0.4,1)
  sweep intersection at (0.55,0.25)
  segment (0.75,0) (0.25,1)
  sweep intersection at (0.625,0.25)
  segment (1,0) (0,1)
  sweep intersection at (0.75,0.25)
  segment (1,0) (2,1)
  sweep intersection at (1.25,0.25)
  segment (2,0) (1,1)
  sweep intersection at (1.75,0.25)

statusTreeGetUpper():
Segment a=(0,0) b=(1,1)
statusTreeGetLower():
Segment a=(0,0) b=(1,1)
statusTreeGetLeftMost():
Segment a=(0,0) b=(1,1)
statusTreeGetRightMost():
Segment a=(0,0) b=(1,1)
statusTreeGetNext():
Segment a=(0.25,0) b=(0.75,1)
statusTreeGetPrev():
Segment a=(0,0) b=(0.5,1)

status tree order test 4:
Not using:
Segment a=(0,0) b=(1,0)
Not using:
Segment a=(-1,0) b=(0,0)
Segment a=(0,0) b=(1,1)
Segment a=(1,0) b=(0,1)
Segment a=(1,0) b=(2,1)
Segment a=(2,0) b=(1,1)
Segment a=(0,1) b=(0,0)
Segment a=(0.5,0) b=(0.5,1)
Segment a=(0,0) b=(0.5,1)
Segment a=(0.75,0) b=(0.25,1)
Segment a=(0.25,0) b=(0.75,1)
Segment a=(0.4,0) b=(0.6,1)
Segment a=(0.6,0) b=(0.4,1)
Segment a=(0.55,0) b=(0.45,1)
Segment a=(0.45,0) b=(0.55,1)

13 segments
  segment (0,1) (0,0)
  sweep intersection at (0,0.25)
  segment (0,0) (0.5,1)
  sweep intersection at (0.125,0.25)
  segment (0,0) (1,1)
  sweep intersection at (0.25,0.25)
  segment (0.25,0) (0.75,1)
  sweep intersection at (0.375,0.25)
  segment (0.4,0) (0.6,1)
  sweep intersection at (0.45,0.25)
  segment (0.45,0) (0.55,1)
  sweep intersection at (0.475,0.25)
  segment (0.5,0) (0.5,1)
  sweep intersection at (0.5,0.25)
  segment (0.55,0) (0.45,1)
  sweep intersection at (0.525,0.25)
  segment (0.6,0) (0.4,1)
  sweep intersection at (0.55,0.25)
  segment (0.75,0) (0.25,1)
  sweep intersection at (0.625,0.25)
  segment (1,0) (0,1)
  sweep intersection at (0.75,0.25)
  segment (1,0) (2,1)
  sweep intersection at (1.25,0.25)
  segment (2,0) (1,1)
  sweep intersection at (1.75,0.25)

statusTreeGetUpper():
Segment a=(0,0) b=(1,1)
statusTreeGetLower():
Segment a=(0,0) b=(0.5,1)
statusTreeGetLeftMost():
No segment!
statusTreeGetRightMost():
No segment!
statusTreeGetNext():
No segment!
statusTreeGetPrev():
No segment!
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
