#!/bin/sh
#
prog="binaryHeap_interp"
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
# Test file for binaryHeap_t

# Initial simple test
h=create();
"Size : "; print size(h);
"Push and peek";
for(x=10; x>0; x=x-1;)
{
  push(h,x);
  "Peek : "; print peek(h);
  "Size : "; print size(h);
}
"Show";
walk(h,show);
"Pop";
for(x=0; x<10; x=x+1;)
{
  "Peek : "; print peek(h);
  pop(h);
  "Size : "; print size(h);
  "Check : "; print check(h);
}
"Destroy";
destroy(h);

# Second test
h=create();
pop(h);
push(h,1);
push(h,2);
"Pop";
"Size : "; print size(h);
pop(h);
"Size : "; print size(h);
pop(h);
"Size : "; print size(h);
pop(h);
"Size : "; print size(h);
"Show";
walk(h,show);
"Destroy";
destroy(h);

# Random test
x=100;
h=create();
for (i=0;i<2000;i=i+1;)
{
  if (rand(x) > (x/2)) { y=rand(x); "Push : "; print y; push(h,y);}
  if (rand(x) > (x/2)) { "Pop"; pop(h); }
  "Peek : "; print peek(h);
  "Size : "; print size(h);
  "Check : "; print check(h);
}
"Show";
walk(h,show);
"Destroy";
destroy(h);

# Test NULL inputs
print push(0,0);
print peek(0);
print pop(0);
clear(0);
destroy(0);
print size(0);
print depth(0);
print walk(0,show);
print check(0);

# Done
exit;
EOF
if [ $? -ne 0 ]; then echo "Failed input cat"; fail; fi

#
# the output should look like this
#
cat > test.ok.gz.uue << 'EOF'
begin-base64 664 test.ok.gz
H4sICNMFX1QAA3Rlc3Qub3V0ANWdS68dt7GF5/oVJ/MMutnsV6Y3Q8MIYOQH
GNYBJCSQBFkXQe6vz3mIp9fmXqtXsXtvxRc2DGnzc/HRfBSLxeIvH//v8eEv
D++6d3/7398/PPz66f3Dl8fHf7z729N/nn/vu3e/fEf6tx/Xt9/S22/L22/D
22/z22/57bfp7bfx7bfx7bfp7bf89tv89tvw9tvy9lt6+23dCr2V+an4Hz7/
693Pn98/PnRPv3z59evjp28vf/ztw8d/vn/620P/8PDnh/T0S//K9YIbnrn8
9Et65ZLgxmduevplfOWGjeuRm5+55emX/Mplwa0PT38bXplxY56zf/p3fU2Y
rhKm14R5SxheE54a5SVluUqZXxPWLSG/Jjy19+cvpH3Xd//z4fG3f1x2jwSf
iSQP8GVJcobOQJJH6D8keYIuR5Jn6KUkeYGOTZJXGAskGcZKh+l/ffz929fP
/35pwk3+Jmr7n+BPz70W/8eSyc9//+mnhy+fP3769vj1T+9EjneEn2eJ54bs
D5TqvtX4gxXnSKvmbT7Nab+zyfTvolK6qKDDx/kCH+d9fJoZmmidtnk5r2L4
fUfXiaGZoUuvykvxlYxhOn8cTG4oytJGX+KTws+mQx5LL74SMmqmLNPDtkDP
y/5HH4cLyRKP5A7MOu334H4rYW8qw1FamTReLpQKD4rbMl72v6tKLsO1IyTt
4AeTy2efCDmpT9QbaTkzdKLtTj8R1S9kOpQsjb70yZQ+xWico/P+V8yXnSuP
fixP3X4Hm2Y1vVB8rsbqsN+9ZXqZlWeG8gn8su6z0vNKU8FC2fuWd+LWam7p
90ejTN9WoxtKy1vfz4Fe5sTB0g7d4bC4sWvDt8qMZyrj0lHGuN9LYeo4Icyl
86WLrkdpmxdT3s9OpoMadY3KNYTPLLyPz0qpoPgAu/xAr5PieDPyT6HSYYtT
5ocz2e3WkDfG5WR3Y3xuosfsP8y24l1ZAOge/j7JvFJUjYl0sVGNh4Zv3m/f
vHdfpVrg1OJe8JmheoEL6P2yMsiozd6909tV8WFkKO/j02XvCShWo5lcZPr3
LDeFXM2jJvlqh9DtD87+Uh81yutySfcpMF6M6e1wOpY63bGLllkMZhq1Ey5r
XGYo72IzQ+nk9F9Lv95LAcqnYpW+s3/Vmy4jrE+sP/I9nkovKvOgcqbiZPo2
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
hqt/QAEA
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
