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
# Test file for trbTree_t

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

"Size";
x=size(l);
print x;

"Depth";
x=depth(l);
print x;

"Check";
x=check(l);
print x;

"Min";
p=min(l);
print *p;
p=next(l, p);
print *p;
p=min(l);
print *p;
p=previous(l, p);
print *p;

"Max";
p=max(l);
print *p;
p=next(l, p);
print *p;
p=max(l);
print *p;
p=previous(l, p);
print *p;

"Remove";
x=2;
p=find(l,x);
remove(l, p);

"Next";
x=0;
p=min(l);
while( x<size(l) ) {
  x=x+1;
  print *p;
  p=next(l, p);
}

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

"Remove";
x=2;
p=find(l,x);
if (p != 0) 
{
  "Removing "; print x; 
  remove(l, p);
}

"Walk";
walk(l, show);

"Destroy";
destroy(l);

# Third simple test
"Third simple test";
l=create();
"Insert";
for(x=0; x<10; x=x+1;)
{
  insert(l, x);
}

"Remove";
for(x=0; x<10; x=x+2;)
{
  remove(l, &x);
}

"Upper/Lower";
for(x=1; x<10; x=x+2;)
{
  "For "; print x;
  y = lower(l, &x);
  "Lower is "; print *y;
  y = upper(l, &x);
  "Upper is "; print *y;
}

"Upper/Lower";
for(x=0; x<=10; x=x+2;)
{
  "For "; print x;
  y = lower(l, &x);
  "Lower is "; print *y;
  y = upper(l, &x);
  "Upper is "; print *y;
}

"Destroy";
destroy(l);

# Fourth simple test
"Fourth simple test";
l=create();
"Insert";
for(x=0; x<10; x=x+1;)
{
  insert(l, x);
}

"Remove";
for(x=0; x<10; x=x+2;)
{
  remove(l, &x);
}

"Next/Previous";
for(x=0; x<10; x=x+1;)
{
  "For "; print x;
  y = previous(l, &x);
  "Previous is "; print *y;
  y = next(l, &x);
  "Next is "; print *y;
}

"Destroy";
destroy(l);

# Random test
"Random test";
l=create();
for (x=0;x<1000;x=x+1;)
{
  y = rand(2000);
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

    "Size"; y=size(l); print y;

    "Depth"; y=depth(l); print y;

    "Check"; y=check(l); print y;
    if(y == 0)
    {
      "!!!Check FAILED!!!"; x=1000000;
    }
  }
}
destroy(l);

# Test NULL pointers
print find(0,0);
print insert(0,0);
print remove(0,0);
t=create();
print remove(t,0);
insert(t,1);
print next(0,0);
print next(t,0);
print previous(0,0);
print previous(t,0);
print lower(0,0);
print lower(t,0);
print upper(0,0);
print upper(t,0);
destroy(t);
print size(0);
print depth(0);
print min(0);
print max(0);
print walk(0,show);
print check(0);
clear(0);
destroy(0);

# Done
exit;
EOF
if [ $? -ne 0 ]; then echo "Failed input cat"; fail; fi

#
# the output should look like this
#
cat > test.ok.gz.uue << 'EOF'
begin-base64 644 test.ok.gz
H4sICHRRpGMAA3Rlc3Qub2sAzVxNjyTFEb3Xr1iffCQjv/NoGSFZAmSBkc/I
tL0jYGY1O2DsX+/6yMp49aK6C2kvFoft6aquioyMiPfiI/ni6fXj27uPTz9/
+On27u328W36y/PH2+vb9MXT8w/T508/PP/x7d0/58/vJvEhTl+8/LJ9nr59
+u9tSm76/Pbh7f3Upj+/v/3jx0mmr56eJzf/66avv/vyy3cfXp6e326vf5j/
/ur736bYzNfzV7FO39x+fvn1Nn19++1t/XmY4pSmPJWpzg+X+StZ3iphkjhJ
miRPUiapk7TJu8nL5P3kw+Tj5NPk8+TL5Ovk2xTcFObn+SmEaV5BSFPIUyhT
qFOYXz0LIFP0U5zfGKeYppinWBaRZsH+/v1PP/4fifP5vEGvL/+Zvr3942Xe
hpNt62pc/3l6/te7yd9bw/6sv71/en30qO8+fLi9fvbly79vr/Puv857P61/
vHv6uHxeL/fP6+UAlwNcDtvlBJcTXE7b5QKXC1wu2+UGlxtcblZMB7eyyRmh
/Z01daHjnTV1ofOdNXWh6501ta5Pd2dRLPW+Y7MPvr69f7BlixN99tfX269P
L7983LWx/3366OUXpxf6jv++H4ddmZ/wrnD8sejtad+LT3h6Ov446O1l38pP
eHo5/jjp7W23hE94ejv+uNy/fTeUb75//uHl581CeuBeYvif3hZ7WON3daEH
8DmY7RG835qkjI/Fj8hfcx6fc23jc2jjl+KcZ+xI6fhNWcIZfuHDeF3Mm5Ay
pMxWyuPz2xxoj9/E5o7fBB90EUkXUQvdKGl5H34TM6+nlvGA2kiY1vwqvt/F
lxMlHx8X5vh+VE9hERL/JiVFY89LmJe/Xw3B6SapjpNuXS6rvGHIWy/kLTnQ
+7Kn/ZUiCdStthEdLbVVoBWeH9wq72JhPZS6ih938RdR7ti0+Mx2Upuqp8wg
PGQWWlHgF8fGa07B7IOo0WXH716cYBE97aLPzOCx5nPlrWCNtdpYhVnY1fiW
UtncKm9EkaHFpmY0LzmtS8hjCe1iCSGQGlIhzUYhNSbHuq9xRCS19Mr7EUVN
K1Td2xA3ocsudDwxmaNIEAFjJGk8yyuO1xg5pMy3GFthFw+BjR98SmbauKyh
7mtID0L5CFbBhJGQPWymCYXDfFPVTbeW3wr4WBp3LkFgEbINIU8i+YAmsCo2
2fmVejU4dVnh6DvfyS4SDVo4fVjmNdcWOvrsQucrr5QWSSGpkQEEN8zPgy15
9tZYBcy0stEUCCeRN8vnzSRkAOcc2e+pO4xPBVQ7Y9/43Fgz2dh5ZNeVYoJc
C+wKYw3ZbUgpAypn1VyoekGnYSaFr/rEccvnER98NRoDS0pRnatECBZZd2SB
/VXggZXlkprUCFyjmh117EoB/KD4IV/mYCBS9EYXmaE0E8Fd3HBeBlLWE21b
o0j6Hi9n4SADl2jGZg1gL+FS8ZjjYu2AIgMU61VwLkovYubXx0Bv8GIUY1mH
bplkBsta1FCAMIjfwFwGFLbLuHGIecBD2IrZ1X3UuGy8FKDRIKL3xgBT1/cA
w3Y/aDQF0RTUrzj6CeCuiSKzQk12kIdGMzMRYLJzvuE2WXfQa+5KxQGdREOB
VCBmSTXmC+12FSOPoZDROl9R+u03eiptyHwZMhzz+ZZo2zxkYQiNzo8XG043
S2BIHZCi6CHONddTmF1oOVH0bgkKbYnfEPEFhjUIx+8Awb0m5lCZLakamlDd
5oRehuAn1sy5o5qCYkDWKNU0tTXMObN9ZCGxKwfiotaeZMMTvwNg8/dDcj6B
L97ikDlraUo2FkjYn5WHoitTFVkY0IggtOA92/JhSHyp4KSshflCCnRz4hfW
ymzbRA+1/6p4HngfQuk2vcNfW0j6vUTRMY5VD5ZcVaklsFNVVflijvojTsMc
2J0GdNlJs09D0PsR2TKelFhBkeGjerYbr0WCWjW0mMwmiEFDxYEmvfCxI2CL
V5WPClRA2wtqf82EVmlNo426qMnOk1r4TJs5fM5mt0pahqRXVlwA0xaOMpxK
y2SVKZrX9CkAc8dimj40cd7i21YG8wPtRrvlfiwTjf9cwJjzCX1bDJBJcJEF
rGGlNWOtEOmxsOG3jMkPiDvLRUmbhhWW8ezsOCgwSW4cNopKFrUoCer1/Atx
rfWy15D6qnQxR9DEe8R8Kht6iZHDNV2lyaassWdTaGs+o2+sCxhYl+9n17MU
yhK8GkKr9M4GfKsagUoYxpvLwQCIrzKmpLrZSBhIVy6NOSheefWZEBlPnWYt
mtUm5v1zRDDEDcJP4jyldPMYQFfu1xUDxMwcVTEGW1M4xnmKbay2bBLcxoVh
WTI1iqld1QPo6mVZUYNqhnpQC6bOA7zTsNQ5+gx9ekA+Z4ibl+NjVnEH3F0m
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
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
