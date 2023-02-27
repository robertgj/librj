#!/bin/sh
#
prog="redblackTree_interp"
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
# Test file for redblackTree_t

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
print size(l);

"Depth";
x=depth(l);
print x;
print depth(l);

"Check";
x=check(l);
print x;
print check(l);

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
p=find(l, x);
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
  p = find(l, x);
  remove(l, p);
}

"Walk";
walk(l, show);

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
EOF
if [ $? -ne 0 ]; then echo "Failed input cat"; fail; fi

#
# the output should look like this
#
cat > test.ok.gz.uue << 'EOF'
begin-base64 664 test.ok.gz
H4sICNG2NVQAA3Rlc3Qub3V0AM1cTY/kthG961dMTjmGxW8egxgGAtg+2DFy
NuJJPLA9s5idJE5+fSiJYr1+pW5dgzWw2la3VCxW1Xv1QX/58v754+nzy6+f
fnl++nj+/LH8+fXz8/vH8uXL64/LFy8/vv7+4+nv/fppER/i8uXbP/fr5buX
/z4vya3/ffH86eOnpfU/f/rp+W8/L9L/fP3yurj+t1u++f6rr54+vb28fjy/
/67/++sffltiMx/3j2Jdvn3+9e1fz8s3z799bD8PS1zSkpey1P546R/J+nIJ
i8RF0iJ5kbJIXaQt3i1eFu8XHxYfF58WnxdfFl8X35bgltCf55cQlr6QkJaQ
l1CWUJfQX90FkCX6JfY3xiWmJeYlllWkLthff/jl5/8jcb7o+/T+9p/lu+e/
vfXdONm9ocbtr5fXfzwt/t4ajmf95aeX90eP2n6+/jj1H7bl+0+fnt//8NXb
v5/fu028d4tYtn88vXxer7fb43q7HeB2gNthv53gdoLbab9d4HaB22W/3eB2
g9snYjr4KlugEdrfWdMQOt5Z0xA631nTELreWVMb+nR3FsVSHxv47Q+vP779
um/d8NLVYf/4sS56c9bqwnDVbrKHp46vJinzsvjp5jXneZ1rm9ehzV+Kc54D
RUq3n5TVaPEDH+brYt6FlClltlLePr91d7r9JDZ3+0nwQReRdBG10Bclre/D
T2Lm9dQyH1AbCdOa38T3h/hyouTbx4XuxbfqKSxC4t+kpKHX8xL68o+7oceV
uUmq46Rbl8smb5jy1gt5Sw70vuxpf6VIAnWrbURHS20VMMTzg1vlXSysh1I3
8eMh/irKHZsWn9lOalP1lB5qp8xCKwr84th4zSmYfRA1uuz43asTrKKnQ/Qe
/x9rPlfeCtZYq41VmIVdjb9SKptb5Y0oMrXY1Iz6ktO2hDyX0C6WEAKpIRXS
bBRSY3Ks+xpnRFJLr7wfUdS0QtW9DXEXuhxCxxOTuRUJImCMJI1necXxGiOH
lP4VYyvs4iGw8YNPSScH6xrqsYb0IJTPYBVMGAnZw2aaUDjNN1XddGv5rYCP
pfnNNQisQrYp5Ekkn9AEVsUm21+pd4NTlxWOvv2b7CLRoIXTh2Vec21hoM8h
dL7ySmmRFJIaGUBw0/w82JJnb41VwEwrG02BcBJ5s3zeTUImcPbIfk/dYV4V
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
