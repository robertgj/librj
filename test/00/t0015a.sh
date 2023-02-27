#!/bin/sh
#
prog="redblackCache_interp"
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
# Test file for red-black cache_t (same as splay tree test!)

# First simple test 
"First simple test";
l=create();
"Insert";
for(x=0; x<50; x=x+1;)
{
  insert(l, x);
}

"Find";
x = 1234;
p = find(l, x);
if (p == 0)
{
  "Didn't find "; print x;
}
else
{
  "Found "; print *p;
}

x = 12;
p = find(l, x);
if (p == 0)
{
  "Didn't find "; print x;
}
else
{
  "Found "; print *p;
}

"Depth";
x=depth(l);
print x;

"Size";
x=size(l);
print x;

"Check";
x=check(l);
print x;

"Walk";
walk(l, show);

"Clear";
clear(l);

"Walk";
walk(l, show);

"Destroy";
destroy(l);

# Second simple test
"Second simple test";
l=create();
"Insert";
for(x=0; x<10; x=x+1;)
{
  insert(l, x);
}

"Walk";
walk(l, show);

"Destroy";
destroy(l);

# Test NULL pointers
print find(0,0);
print insert(0,0);
print depth(0);
print size(0);
print walk(0,show);
print check(0);
clear(0);
destroy(0);

# Random test
"Random test";
l=create();
for (x=0;x<1000;x=x+1;)
{
  print x;
  y = rand(2000);
  print y;
  insert(l, y);
}
for (x=0;x<1001;x=x+1;)
{
  y = rand(2000);
  p = find(l, y);
  if (p != 0)
  {
     "Found "; print *p;
  }
  else
  {
     "Didn't find "; print y;
     insert(l, y);
  }
  if (x%10 == 0)
  {
    "At "; print x;

    "Depth"; y=depth(l); print y;

    "Size"; y=size(l); print y;

    "Check"; y=check(l); print y;
    if(y == 0)
    {
      "!!!Check FAILED!!!"; x=1000000;
    }
  }
}
"Destroy";
destroy(l);
EOF
if [ $? -ne 0 ]; then echo "Failed input cat"; fail; fi

#
# the output should look like this
#
cat > test.ok.gz.uue << 'EOF'
begin-base64 664 test.ok.gz
H4sICDlEX1QAA3Rlc3Qub3V0AJWdS4/txnWF5/wVmmXKYr2HQQwDmcaDjA1b
gQU7smErg+TXZ3+Lp/ve3qt9xQtB6tM8xWLt99oPtn7709//8csP//jpv//2
lx9/+OXHf/xy/PvP//jx778cv/3p5z8ev/npjz//yy8//Fd8/uEoV23Hb//6
P/fn4zc//u2XPx37+N1P//fj0c/j3/704x/+fJTjP3//lz/zfdtHW0ebRxtH
60drR6tHi+vlaOdR91HXUedRx1H7ETvXetTrqOWo53Ht41rHNY9rHFc/rnZc
9biu4yrHdR5lH2UdZR5lHKUfpR2lHqUc5YzjrGMe44jnHXFHHCcO9pcff//3
+1i/CQL//tf/PX734x/+GmR8QraWfbbL263nV//8x+9//uNf//u+P451nvGf
ONeOox8zSAvqg+o43Y6t1qixZWnBjjh5C44FDX3veFo/K4cvM/gUdAxovI4R
/ArCxhgisbMieAH1ceUKhgYLVjAsmNGCgfHF7BMGjbFgVawR19a44F9sGYys
q8DSsk6YG0c74XMLUQXHS41DBvNLH5JDKaMjklLnhXTKXheCinUFme24O6RX
zrER5N4TkY4gI4RbVpwzSC87fgmRz7kRfrn2hR7E1lKJck6eHzSOiaKU3VGZ
DuvmMYOOOFvnYohlLVRtxpF67DTa0WOjvmJ5hfFxGa7Mo7NPcKKjIUFf8CW+
rkePY+3gYo+jxFlG8CqkFcSsoCDYVM/4ESzosJ8j9iM40INTSOGasZ6d+FrS
iy1CVYM5E+EE7RM1jFPGpbLiwRMC+Rm7lfhlhiLEM2bsdu6BlpQZ0p/YQjkm
eh+KEZu1Mw4VTI4b4XnoyYLvcWlBdVztSI+THzsOtybSjSeXEoJZKMk8Qjw7
vohVLQSwb53aYlX8Gke6Qt020mKTWLeOEOEIoQVXVqhzPLaKhDhRPYOmOFXQ
uSEqpMQ3Vap2IkCEcXIsVOSMM22sYnK2YEo8DTEE9R3uhDo3rDdO0NFONB7V
C5Wf7IbOFzQVcU3pYshOH2KXigZJ8ePH5nc0C7vpUBUX0IhQ/lKD++G6gji+
5ZivZWhYwQaQV2HVYKfbDPiKbTk6hoByFiyhie9SXxaGjQ+e1kKE8Ci+HBu7
4rC6BylwDPSlSGZYBDth8do7ftliYNgqrIMoWBRW0XkKVtG42GT43BI7L3S9
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
loWPlnEAAA==
====
EOF
if [ $? -ne 0 ]; then echo "Failed output cat"; fail; fi

uudecode test.ok.gz.uue
if [ $? -ne 0 ]; then echo "Failed output uudecode"; fail; fi

gunzip -f test.ok.gz
if [ $? -ne 0 ]; then echo "Failed output gunzip"; fail; fi

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
