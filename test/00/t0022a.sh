#!/bin/sh
#
prog="trbTree_interp"
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
# Test file for trbTree_t remove()

"Test trbTree remove()";

l=create();
"Insert";
for(x=0; x<2500; x=x+1;)
{
  y = rand(500);
  insert(l, y);
}

"Remove";
for(x=0; x<2500; x=x+1;)
{
  y = rand(500);
  remove(l, &y);

  z = check(l);
  if (z == 0)
  {
    "!!!Check FAILED!!!"; x=1000000;
  }	

  y = rand(500);
  insert(l, y);

  z = check(l);
  if (z == 0)
  {
    "!!!Check FAILED!!!"; x=1000000;
  }
}

"Destroy";
destroy(l);
EOF
if [ $? -ne 0 ]; then echo "Failed input cat"; fail; fi

#
# the output should look like this
#
cat > test.ok << 'EOF'
Test trbTree remove()
Insert
Remove
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
