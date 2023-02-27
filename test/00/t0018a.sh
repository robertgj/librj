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
# Test list clear()

  l=create();

  for(x=0;x<15;x=x+1;)
  {
    y = rand(2000);
    insert(l, y);
    "Size";y=size(l);print y;
  }

  walk(l,show);
  "Clear";clear(l);
  "Size";y=size(l);print y;

  destroy(l);
EOF
if [ $? -ne 0 ]; then echo "Failed input cat"; fail; fi

#
# the output should look like this
#
cat > test.ok << 'EOF'
Size
1
Size
2
Size
3
Size
4
Size
5
Size
6
Size
7
Size
8
Size
9
Size
10
Size
11
Size
12
Size
13
Size
14
Size
15
1000
1798
77
474
139
863
1446
1450
1599
503
1745
620
643
666
1510
Clear
Size
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
