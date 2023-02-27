#!/bin/sh
#
prog="splayTree_interp"
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
# Aggregate splayTree_t tests

# From t0010a.sh
# First simple test 
"First simple test";
l=create();
"Insert";
for(x=0; x<50; x=x+1;)
{
  insert(l, x);
}

"Find";
x=1234;
p = find(l, x);
if (p == 0)
{
  "Didn't find ";print x;
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
p=find(l,2);
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

# Test for NULL pointers
print find(0,0);
print insert(0,0);
print remove(0,0);
s=create();
print remove(s,0);
print next(s,0);
print previous(s,0);
x=insert(s,1);
print next(s,x);
print previous(s,x);
destroy(s);
print min(0);
print max(0);
print depth(0);
print size(0);
print walk(0,0);
print check(0);
print balance(0);
clear(0);
destroy(0);

# From t0012a.sh 
#Test file for splayTree_t balance function

d=3;
for (a=16;a<=2048;a=a*2;)
{
  d = d+1;

  l=create();
  for (x=0;x<a-2;;)
  {
    y = rand(20000);
    p = find (l, y);
    if (p == 0)
    {
      insert(l, y);
      x = x+1;
    }
  }

  "Balance"; x=balance(l); print x;

  "Size"; x=size(l); print x;
  "Check"; x=check(l); print x;
  "Depth"; x=depth(l); print x;
  if (x != d) "Bad depth!";

  destroy(l);

  l=create();
  for (x=0;x<a-1;;)
  {
    y = rand(20000);
    p = find (l, y);
    if (p == 0)
    {
      insert(l, y);
      x = x+1;
    }
  }

  "Balance"; x=balance(l); print x;

  "Size"; x=size(l); print x;
  "Check"; x=check(l); print x;
  "Depth"; x=depth(l); print x;
   if (x != d) "Bad depth!";

  destroy(l);

  l=create();
  for (x=0;x<a;;)
  {
    y = rand(20000);
    p = find (l, y);
    if (p == 0)
    {
      insert(l, y);
      x = x+1;
    }
  }

  "Balance"; x=balance(l); print x;

  "Size"; x=size(l); print x;
  "Check"; x=check(l); print x;
  "Depth"; x=depth(l); print x;
   if (x != d+1) "Bad depth!";

  destroy(l);

  l=create();
  for (x=0;x<a+1;;)
  {
    y = rand(20000);
    p = find (l, y);
    if (p == 0)
    {
      insert(l, y);
      x = x+1;
    }
  }

  "Balance"; x=balance(l); print x;

  "Size"; x=size(l); print x;
  "Check"; x=check(l); print x;
  "Depth"; x=depth(l); print x;
   if (x != d+1) "Bad depth!";

  destroy(l);
}

# From t0013a.sh
# Some more splay tree tests
# More tests for splayTree_t balance function

  l=create();

  "Size";x=size(l);print x;"Balance"; x=balance(l); print x;
  "Check";x=check(l); print x;"Depth"; x=depth(l); print x;

  x = rand(2000);
  insert(l, x);
  "Size";x=size(l);print x;"Balance"; x=balance(l); print x;
  "Check";x=check(l); print x;"Depth"; x=depth(l); print x;


  x = rand(2000);
  insert(l, x);
  "Size";x=size(l);print x;"Balance"; x=balance(l); print x;
  "Check";x=check(l); print x;"Depth"; x=depth(l); print x;

  x = rand(2000);
  insert(l, x);
  "Size";x=size(l);print x;"Balance"; x=balance(l); print x;
  "Check";x=check(l); print x;"Depth"; x=depth(l); print x;

  for(x=0;x<15;x=x+1;)
  {
    insert(l, x);
  }

  "Check";x=check(l); print x;
  "Size";x=size(l);print x; 
  "Depth"; x=depth(l); print x;
  "Balance"; x=balance(l); print x;

  for(x=0;x<15;x=x+1;)
  {
    insert(l, x);
  }

  "Check";x=check(l); print x;
  "Size";x=size(l);print x; 
  "Depth"; x=depth(l); print x;
  "Balance"; x=balance(l); print x;
  "Check";x=check(l); print x;
  "Size";x=size(l);print x; 
  "Depth"; x=depth(l); print x;

  "Find";
  for(x=0;x<15;x=x+1;)
  {
    p=find(l,x); print *p;
  }

  "Check";x=check(l); print x;
  "Size";x=size(l); print x;
  "Depth"; x=depth(l); print x;
  "Walk"; walk(l, show);
  "Balance"; x=balance(l); print x;
  "Check";x=check(l); print x;
  "Size";x=size(l); print x;
  "Depth"; x=depth(l); print x;
  "Walk"; walk(l, show);
  "Check";x=check(l); print x;
  "Size";x=size(l);print x;
  "Depth"; x=depth(l); print x;

  destroy(l);

# From t0014a.sh
# Another splayTreeBalance() test
for(x=0; x<513; x=x+1;)
{
  l=create();
  for (y=0;y<x;y=y+1;)
  {  
    insert(l, y);
  }
  balance(l);
  z=size(l);
  "Size"; print z;
  z=depth(l);
  "Depth"; print z;
  destroy(l);
}

# From t0016a.sh
# Test splay tree clear()
l=create();
for(x=0;x<15;x=x+1;)
{
  y = rand(2000);
  insert(l, y);
  "Size";y=size(l);print y;"Balance"; y=balance(l); print y;
  "Check";y=check(l); print y;"Depth"; y=depth(l); print y;
}
walk(l,show);
"Clear";clear(l);
"Size";y=size(l);print y;
destroy(l);

# Done
exit;
EOF
if [ $? -ne 0 ]; then echo "Failed input cat"; fail; fi

#
# the output should look like this
#
cat > test.ok.gz.uue << 'EOF'
begin-base64 664 test.ok.gz
H4sICPJPX1QAA3Rlc3Qub3V0AM2dTa/luHGG9/oVnVW2YrH4tUw8GCCA7YWN
IOvGzHXcmJ7uQU87cfLrI50jseo8dU5rsgkCL+aKzSu9Iov1VBWp6+8/fPn1
67tfP/z8y8e3d1/ffv26/MunX9++fF2+//Dpx+W7Dz9++sev7/6y/fxuSZJ1
+f7z3+4/L3/+8N9vS1mX795++frXRdbld399++GnJS1/+PBpWbf/rssf//X3
v3/3y+cPn76+ffmH7foP7/++6AjNW5P25U9vP3/+j7flj29//3r79bzoUpa6
tKUvY0lbU9ofm/KSdEllSXVJbUl9SWN/uqRFZJG8iC5SFqmLtEX6ImPJ65K3
+8mS87K9Qi5LrktuS+5L3h69CUiLyqLbE3XRsmhdtO2SNmH/9v7jT/+P5Hy3
zdCXz/+1/Pnth8/bPDyZt2MYb//58Onf3y1yvkPe9Oumv276N5nzXn96/+nH
zz/fb3JM7z7T//T13fZLt1nuaz6meZN8TvPRtWxvff7YZNpHr3X+XPuYP+cx
fzOtq9DCSnlsafs7+wbJ83Fa7yLTVFlPlWmqfLz/2GbjsUXH+tiStzmbL1Hs
JXpDx1T25/kWrXyf3uYN+oCYMeQmX075aQ7y+kJ+3ozgcXgaJRT+zjaic80K
X2F7/fNf8/bUOUk2xsWmrrab3jz19lOvPNfbasbzqmB+U0vFDbfZhq541dGd
8xHeeHTOYuM4tH6Tr6f8XcrddTXadJJKO+nDhqdtK3VqTnijzAfr4DuXHOYh
mdHVlc/eF8EuvZzSc7ow9No5FRyx0QeHsCYuNXZpnebWOREtzVEcZkbbK5fb
K9T5CuMc/fr8FXLGMJSGkdWEYSwrx77r9Ehm6Z3zoclMK3eb26x30e0UreVC
tDoPqAo1Qr1p5TsqXcrWJdgKl3jONH63ptLGlv0d+vkOZXqZElz5dFY5uJFc
xU1mcIXTfEu3SY+WP5pbY2X23J3ALnJMkTUO9ESTsyqa7PZI+9e82pJN9L5b
Ty4RDbRY7WaV79xHPuhziq7nqtww/wI/igEpAwaQ12l+4mxJuFq1J2emnUbT
nDtRTpbUu0mkCc7NswfynJYwf2puaDf2zZ8HR6YGO1cu3dSCkxuZS2G+Q13v
pEwTldvQHEM9Xgz1TqdpJo3/KoV+S+r0D9LDiDlLKmqLq6lzFtVmZMf+TfBk
ZasxgKKrcrFGDzO6cilltw6aTH2VziClZh1XZYQyggdf9c75NEnZJRp2NIpi
z5H0zB1UF0uMYLMB2Lu7NB7TL/YDKGlCsZercMTCC618vGY8QVIYmBh12JSl
Slj2ZobiAoYkd5inicKRroz5wee5OIRWzKUuan45rFKHxkBEkWCA5RjvCcPR
XlrFMIiWbOuK3i857gYvsg1oyA7qHNHKSMRFslu+sd61ntAb69UQZ79IzBWk
7gKzYiMmDbPdU9ATQkiNi69Z+C338DSNqXkysLwwi5Xx/CiYNnFZmEfjKvPB
IabbFISgzgVFKs7PjfVIYU7RKb0kSTG0FT5B/QNC1JDov7Nz7r0whqq0pB7C
hL7eF6GkKbzFsIi5o5mCMaCalxqW2obIudI+aoLsTkfczNpLuvNETgAOeQLA
uTQivjjFuTJrGRZs7Eg471XnQHeGKmmPgKYHwQuf2ZbkqbhduOcN0/N+jBdK
RufCB/bOaDt4D7P/bjzPnIfcDps+8Tf2IJ3ZlgEVK66Ls+Rug9oyF1W3Id/N
0X6Jadjq7M4cejqDZilTaHsZNceIpxQOkBIfXWg3YkWC3s21hMwmp0BD48BI
R+HjJODQfJFXdRcKWBHS7G8E15rGMG9jSzRk58UsfAub6T63Gb8pbVPpk0j5
cdU6pu0xylxUVibrDNHE0qfsIndfTLObFuYtMu5lMJm0m0XZ13WwZP6fBYwt
n7CnaXaZBIsszhpuYc18V+fpfWFD7hmTTMTNXNSVYDCaISps8951pVNgkDzo
NpopUytKuuEV/kZaxzjKXlP1uIrXBgMq4dBtQAj+wHmOddhbhmwqGnsNhbYh
1a+N2wtM1tX6EtKbCosSxAxhdDxzuHirB0EtT+Ot7cEAEK+SKaXfbSRP0rX1
ihvZeCW2ZrKSp6tlLZbVFsb9m0cIgZtzP4V5SjvMY4KuzTxEQzLtfGZVG5jA
1pIf/Tx8G4ethgR3sDCc9kwNPvUY6gm6ni5ioGpOtbp60MihzuPizhClbt5n
jqc48q0hcJP0eJub3Im7mfDJq5y62BoPafEwIPURwrUQFIuvC1YzOKNRp93s
Feab4sm5cWnLe1Z43s9vPlChM6R9EZnfGLQMZ2VhDKqVmPbq103sRN3Ql9GE
r+yuzYVV9N6r8xHmkLjkCjd5SnOxBSO9vfBxUzp5N/pF+JCKy5qri8pGsQEI
aUdnyGa4C0lTpjsfjod67pyc4Evrsw01eDWGUMXxoTpH10KslWyrRH1VygWP
wgwsu2yjtGOnxOS2KNcSQlsCYXtNnPv3xTK3C5j8xpNbZS0wc2WKVNs99NE0
lab1IqToLo1zNTu/t63VdXELSFIIJ4LNNHLysXJ4UyumVi/rmH7PnZPWuTak
MRDQuOPmqlZ7qIesyZDvvPS5d5ancllfJqBbdkxP2MI4WV0wh+JwGh7j/Ndq
66D5zUkn/A42VVP7ZJxfr58acjVtLDLQZW2GYmKSi6CrI5xkBkrFZQtyLLti
si8jzaSu6Oa2BFbn42xKU7JIpvh1ALNmLr0FK3xd6Ycx16k2X+5fi3NbLh9y
IzQMeo3p6LB3jdsH1ZDhqJ+mh2gms0YPcXrLgkdqmLCaH/PF+aDi6kDzR3WZ
BYPk4eYiHeurm8pxlcP1GPQzNcwxoqIhb8vJ7ySFOC6/XtWpVZf/5eMVjHH6
ZKCxrLIrUMDmRqER5sKgLLze6qJSZcktR2/tt7Tascc+9ZcrRufVzW6xOQ8b
i3SHZshbHgizEEZHOWwGZSa+SY+0qRgI6/o6yat0qn6nM+4GVBfuheQzxyUS
56l6OMTy7D2yL8ZFy1AlxJ4+GXMrvfMgTxortzxMhiYfTLnKBW8SuHUMtJFw
JqhxH+S2ez/FNvMQETnh6IIkF0Amt0bdK/NIUrOdi1LuJeNiEGz5ZR3Tghx3
zKZzS7aI2/UNm2HrmEsgnN1oXItqybgeI2rYa2e0qXE7uvnXd3szqzvm9OTY
AQtD9GXV7b5mCzA6E+t6bI4Ww55lza9C+eZm3p1ichvJmUOd/dEDrqcSzs11
2q06w6vl7pqLMbDXaLOPXsiSpOyGtuNn5h3hvJert9kc0hYsQNGjVlUMhOMb
geZYY+rjEwjWs10xcoQ6DnOXwfyjBLebwzZyPWL8YhgcT3In2JTt3dj5mOZS
1hg70F7S6O7wTtipqY+HcmagdB7KOsUm2yh9dYZFQ61pNfO+Qf+UEWJVs2lX
2ih2KiCHjcfuneB6r1DUZGrLS9c7yNVUVhdsWH1yc/+uBJXC6Qk7S+WKx7dg
d/483O5+OUSKiWyvDXjlHnc2p5BdJTachswx/xBbmqEyvv2rO0xkB6/OU3mT
Zsk2cPW5ARSWmMxk68OuPyc/JANu2MIGUvGHP5pPZu5Yq2qKX8cKQs9pyXQm
v4qbXFchctUiDYFYC7X5cApkLyvdBE+8bS97McQbMGwHNvgpS0Aeysm2rJ/E
m75W687xWfi0G9hNZjWZr+t/yaWUJSToEtLn5E5OhCErrKpI9cUtH5z56vlh
B8309otkubMCviXYRrnX++fVVlaoSSnHepsIPsUzf9+9u+megEtze1lemUMO
9fUW4lSX/JfiMOeKbv6IzAiOrsQauHOLdyzXYapnaSKsuiIP20a2qkJ0EEL2
5AL6sPengSisU9wOMj6O/XGsphni5iHeZ9v5zufmsG/Rq3Ox/meeIaiOtzlk
3GsoXqzVWdhx5tgop+N1JldCspbdsbMaci21qo8Lux4K9VZ8yzzllAo5tKXw
xwAb8MplHWi4wkF3ezyxesmTK8LNsGbFlu4IaGzp4aOMMu6+oxn1Sr+K4aWH
SXPj7IrELYRmTO0aT17gssZHiRuje7LcDID1SRQEw+VBjj58mGxz4e3W7Rak
8N0JT70UdyDKjrUcx8Oasa9Ot/HqmFU4JVhtr8B/eLHyU6HkN01riIGHOzy4
+nqGS7TWe7GwGQTbk5MpPlg8/Zz/IkTj3mHyi+4xIrHgRmnbPfhktdCtHmeM
mxFw5s1Pvmh5UkVSd5I0kHG4SAlu2CUQ4QyRq9Heth7mMB8fOzXjnuXNgSAP
e0LhPGAsRiez1Sz4x+Hgm/2Wu/sehMnzsaffDHe21xyHdjVSthD/BV+vzj+v
FtE0I09ztYi4s59pJWMtxxcepnZceIThFvdqeZG4PQF3cPdWI0ahKZ6OdN+c
qBlpqBkfm+Ld+Dae8OKcTZ7XckOz1ylPOwxHJMRHqDbVw/yey7R8hnGeDO3G
s/HknME0nZCxuoCLh+Lsn9xo+XP8Lk4ebjvLinNH9NiNXHYKO3zOcyuQPSqw
KDecVkgeaBoQ5c5Ep+78bPi6ZLUtrXx8t9Ynq8TqC+FwzJMj4D34+BQ2hUKU
Jr5CGPI/B1LGl+JW43E2phdT/o3Dld1tZ7sveJLNssaSvT8YF85P8RDo/hmX
ATvEQIfayS2ZafyTooP4GiQDuh6enBwKwudKLmBwR1duJcv5OBdytkNoM6Hf
2Jn12UA4Umcr2TszF79XTn4hGnowuMx4cxz1/t5N8QzHIwpq+Bp0hA9Xuq+X
YKjZOQU/my28cvWoAAU9qup9Iky+8b2rWPgWt59ctcg8kdpCitXoZPui6qJL
Ub8Zko8v/kxfi3nZ4/xZRdQvshYYo+baw6n6TUWsnNmpNgZCPbmTTvUew4yJ
rtt+/qstlfFYgZ/xydNT36vPthmtu9MyhbmBhs22cWSQQ0zmkyOJtMpv0H6D
DFOUEF0/HD6IH6q4o2P8ZDYcMtdjK3ZMyt3q3y+cb32IU6Czx7Mldhjcm6Ox
zx27G7bWnnyMtB4qDW56lYipK2e77cAUsoJUrKe6c189xEOh2pTd0cthseYe
md7kGtH0yREDywTNTK2cJIH27oUevoo3P+yqee4EboRYOmrRwyBWrgql4nbq
wkL3OW6sR5dwuqyETxK75YZK8g1WLcvh0AxtVlQIcS3tVPz5RHfYv4WDU6vl
rtXtdZWQ70avHA5UpOPE0TC41fwax9Ev9FAOauFTo8rE0xV/slu7MeQRG/7O
Kng9Qp5hfKtPymS4od/+CzV3d2arWQzbnc2Gv+8hLukRi4qbW4K5jPMz8am0
PfFm64v//fP7H9/95/uPP737y98+/fD1w+dPs/3j+08/bHc7bupWyH3rPPQo
lz0qepTQo1318GP/oke67CHoUUOPfNWjXt6jXt+DY9pCD45p6JGEg/qkC0f1
SZeOLj12GVddpPCFnnThGz3pwjcasQvfKHTZ9/cuu9BSnnThNKewMvYvkK76
3DaCfkOn33QnDvL2GrEThzl2kn0r/Td04lA/7UTz2cYtdqIBnX/jy17ynLPz
SMbp0dzNcIvj5i87HI/I3+hwXt8fdTI1lSe/8r/uUu5/5mz/+17y7b/w9fIR
8w+EffMGsmEg7SxJeUuwkv4GZbcbJ/d3u/a/2lVv98y324Z7bk98NVzlnEpM
HKYJk/JwOb+CPdbV42V9vGyPl6cMPb7YeLycZ/bP64RrwXXGteK64Lo+jsL8
1rs8H6WZ4ZdzbeA64VpwnXGtuC64hj6BPoE+gb4MfRn6ZpZaJz4frxXXBdcV
1w3XHdfj8VpXXCdcQ59Cn0KfQp9Cn0KfQp9CX4G+An0F+gr0Fegr0Fegr0Bf
gb4CfRX6KvRV6KvQV099bYYtj9cV1w3XHdfj8XqegD2vE64F1xnX0Negr0Ff
g74GfQ36OvR16OvQ16GvQ1+Hvg59Hfo69HXoG9A3oG9A34C+AX0D+gb0Degb
0Degb6+ZoCGxQdiQ2aBsKGyobGhs6Gyg0kSliUoTlSYqTVSaqDRRaaLSRKWJ
SoVKhUqFSoVKhUqFSoVKhUonXPrZMNAw8TIbEhuEDZkNyobChsqGxgYqzVSq
VKpUqlSqVKpUqlSqVKpUqlSqVFqotFBpodJCpYVKC5UWKi1UWqi0UGml0kql
lUorlVYqrVRaqbRSaaXSSqWNShuVNiptVNqotFFpo9JGpY1KG5V2Ku1U2qm0
U2mn0k6lnUo7lXYq7VQ6qHRQ6aDSQaWDSgeVDiodVDqodECprCsbEhuEDZkN
yobChsqGxobOBipNVJqoNFFpotJEpYlKE5UmKk1UmqhUqFSoVKhUqFSoVKhU
qFSolIwSMkrIKCGjhIwSMkrIKCGjhIwSMkrIKCGjhIwSMkrIKCGjhIwSMkrI
KCGjhIwSMkrIKCGjhIwSMkrIKCGjZDJqnA2NDZ0NAw2TUbMhsUHYkNmgbChs
oNJKpZVKK5U2Km1U2qi0UWmj0kaljUoblTYqbVTaqbRTaafSTqWdSjuVdirt
VNqptFPpoNJBpYNKB5UOKh1UOqh0UOmg0gGleV3ZkNggbMhsUDYUNlQ2NDZ0
NlBpotJEpYlKE5UmKk1Umqg0UWmi0kSlQqVCpUKlQqVCpUKlQqVCpUKlQqWZ
SjOVZirNVJqpNFNpptJMpZlKM5UqlSqVKpUqlSqVKpUqlSqVKpUqlRYqLVRa
qLRQaaHSQqVkVCajMhmVyahMRmUyKpNRmYzKZFQmozIZlcmoTEZlMiqTUZmM
ymRUJqMyGZXJqExGZTIqk1GZjMpkVCajMhmVyahMRmUyKpNRmYzKZFQmozIZ
lcmoTEZlMiqTUZmMymRUJqMyGZXJKCWjlIxSMkrJKCWjlIxSMkrJKCWjlIxS
MkrJKCWjlIxSMkrJKCWjlIxSMkrJKCWjlIxSMkrJKCWjlIxSMkrJKCWjlIxS
MkrJKCWjlIxSMkrJKCWjlIxSMkrJKCWjlIxSMkrJKCWjlIxSMkrJKCWjlIxS
MkrJKCWjlIxSMkrJKCWjlIxSMkrJKCWjlIxSMkrJKCWjlIxSMkrJKCWjlIxS
MkrJKCWjlIxSMkrJKCWjlIxSMkrJKCWjlIxSMkrJKCWjlIxSMkrJKCWjlIxS
MkrJKCWjlIxSMkrJKCWjlIxSMkrJqEJGFTKqkFGFjCpkVCGjChlVyKhCRhUy
qpBRhYwqk1Hp/+Q0zX1EX3aYhzsuOtSrDu2qQ3/ZYR4SuejwcPrqeY/XY2lH
S656vB5NO35y1eP1eOptV2IvE+7VxT1336m051d7YXD/huX25zz3D+7SXgpK
bWNU2r397f+kbF/Wv/v49v7L/UHr8j97LG+G0HEAAA==
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
