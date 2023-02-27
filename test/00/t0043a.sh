#!/bin/sh
#
prog="segmentIntersection_test"
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
100 100 moveto 200 200 lineto stroke
150 150 moveto 250 250 lineto stroke

200 100 moveto 300 200 lineto stroke
350 250 moveto 450 350 lineto stroke

300 100 moveto 400 100 lineto stroke
350 100 moveto 450 100 lineto stroke

500 100 moveto 500 200 lineto stroke
500 150 moveto 500 250 lineto stroke

150 550 moveto 250 650 lineto stroke
100 500 moveto 200 600 lineto stroke

350 500 moveto 450 500 lineto stroke
300 500 moveto 400 500 lineto stroke

500 550 moveto 500 650 lineto stroke
500 500 moveto 500 600 lineto stroke
showpage
EOF
if [ $? -ne 0 ]; then echo "Failed input cat"; fail; fi

#
# the output should look like this
#
cat > test.ok << 'EOF'
Segment intersect test:
s1: (100,100) (200,200)
s2: (150,150) (250,250)
type segmentInteriorS1 at (150, 150)
Segment intersect test:
s1: (200,100) (300,200)
s2: (350,250) (450,350)
type segmentDisjoint at (0, 0)
Segment intersect test:
s1: (300,100) (400,100)
s2: (350,100) (450,100)
type segmentInteriorS1 at (350, 100)
Segment intersect test:
s1: (500,100) (500,200)
s2: (500,150) (500,250)
type segmentInteriorS1 at (500, 150)
Segment intersect test:
s1: (150,550) (250,650)
s2: (100,500) (200,600)
type segmentInteriorS1 at (200, 600)
Segment intersect test:
s1: (350,500) (450,500)
s2: (300,500) (400,500)
type segmentInteriorS1 at (400, 500)
Segment intersect test:
s1: (500,550) (500,650)
s2: (500,500) (500,600)
type segmentInteriorS1 at (500, 600)
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
