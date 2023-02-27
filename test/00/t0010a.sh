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
# Test file for splayTree_t

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

# Done
exit;
EOF
if [ $? -ne 0 ]; then echo "Failed input cat"; fail; fi

#
# the output should look like this
#
cat > test.ok.gz.uue << 'EOF'
begin-base64 664 test.ok.gz
H4sICAVOX1QAA3Rlc3Qub3V0AM1cTY/kxg2961dMTrkW65vHJIaBALYPNoKc
F96xPbA9a+yOEye/PiW1VHz9KG1fAx/c7u5pUSyS7/GR8pcvHz+9PX16+fW3
X56f3p4/vS1/f/30/PFt+fLl9f3yxcv71z+/Pf0wXj8tElNevvzw++318t3L
f5+XEpYvnn97+2mJYfnbT8/f/7zI8vXL6xLGv8PyzT+++urptw8vr2/PH/80
/vvrd38sWd3b463cl2+ff/3wr+flm+c/3rY/T0teylKXtvRFFxlvyXpZSYvk
RcoidZG2SF9E16tHWWJcYlpiXmJZYl1iW2Jfoi4pLGn8XlxSWsYtpLKkuqS2
pL6kcelhgCw5LnlcMS+5LLkuua0mDcP++e6Xn/+PzPlinNDHD/9Zvnv+/sM4
h5Nz2924/evl9cenJR73kIb9edhfh/3DzPlb3757ff/h19uP7Me7nvRf3p7G
H22n3EPaj3mYfBzz/tUy7vp42eKMj17rfF27ztdJ519KCJEjrJT7d9p6z/hG
TPNyud6MlGllPayUaeX97+s4jft3sob7d9I4s3kTxW6iN/qilPV6+E6ufD+9
zR/oSsaoxs38eJgv08nhwvw0guDePY1NKPw3w6MzZyPfwrj949M0rjoPyXxc
7Ohq2+xN095+2BvP7W010fVqpPOVJgXcbbGRA92qdig+kX9YO59iYz+0vpmf
D/NXU26lq3FMS6wcJ13NPW1k6rRZ6I4SXzgr33NJ7hzEgq4GvvaaBKvp5TA9
yYNAr52Pgj2mXdmFVTjV+Cutc7h1Pogm04tqYTRuuWy3UOct6OH9en4LKZEb
SiPPZiE3lsC+73lWJIv0zueRxUIrdTvblG9Gt8PoXB4YnaEC5kzWRLZXAt9j
5pIyvuJihVM8JQ5+yCkZ2LLeQz/uocwqU1wpn8UquTKSaoTDdKVwhm/pdug+
8rVBjpX5zbUIrEbqNLJ6R09ogqjikB2XtE9TsJQVrr7jm5wi2aFFsB+rfM9d
044+h9H1yMoB8xfwk8khRSkAUpjhFyGWImdr7gJh2jloGpSTzIcV6y0kZALn
qOwOeY5ImK8auHZg33yt7Jnq4jxz6kpzRU4Tp8K8hxpuSCkTKodrdlfrhatX
dJph0vjTWLhuxTrrQ+zOYxBJJVtytQzFotqJrLC/GTyxslVPoLhUAdfo7kQD
p1KCPGhx2le5GIg0+2LIzFDUVfCQbzgvEyl79IHtg6LYdaKclYMKXEJdzDrA
Xsul4THXxb4DikxQ7OURHTF6kStfPie6QhTnGM867MikMlj2ZoEChEHiDcxl
QqHKo2C+q3nAQziKOdVjtrrsshSg0SFijC4Ay+7vCYbaLqNCDURLsrzi6ieA
u66KDIe67qBOj1ZmIsBkR78RbrYeoKfhkYsTJomVAulAzIp5LDY67S7OHkch
s0++ZvQ73uip6LR5YmC5CIvAfF4LHVuELgyhMcR5YcfphgWO1AEpyhHqnIa9
hTmMFrlEkmLQVvgKGS/gWINw/U5Q3HthDlU5krqjCT3ckjDKNLx5WsS9o4WC
YUC1KqXW2jrmXDk+qpDZnQtxs2gvcsOTeACgxhMAnKnh4YuPOFXuWtTIxgoJ
x2/V6ejOVEVWBjQrCN3w0W3FNC1uD8rzgOn5e8wXSqIvF75g78y2XfWw+O+G
54nPIbU9pg/405Wkc7dlgEoZ1yNEcjentsRJ1c3lazjaH3EbFiDurKDLQZpj
mYa2S9bsGU8p7KDM8NEjx000kaB3Ky2us0ni0NBwQGUXPg4E1Jwe9FUdqICJ
kBZ/6kqrqFq1sRR13XmxCB+0mcvnOPHN0jYtPWHK91kLmLZylJlUJpN1pmjR
2qcEzB3FNPvRwn1L1JsMFifaTVH2WgcTq/8sYIx+wq6WE3QSLLJANGy0Zt4r
VHoUNuKtY4oT4mYvChIMedOxwjZ/uwYuCkySlctGM8uyiZLg3sh/IUF1l72m
1fqIrykTqsiuG4Dg6gFUjqB2l66b8sFendCmsWJubDcwsa7WS5AeVhhLiBYI
2umaCnyrO4NamsFb210AEF9lTCn9FiNpIl0Lj3AjGV5Fy5mUGU+DdS3W1Rbm
/aMiOOIG5adwn9L28JhA12Yfkl0zDTWzZnOMw9aS7us81TZ2W3UNrrIwLGun
RjV1d/UEui4POFC1olpBD9LkdB7gnY6ljuoz/RkB+YIjblHuf2Yzd8LdbPji
VU9dLMddW6wGSF0dXXOkOKIuWC3gDI06x82qMG8WT5zTh7G8doXH7+HwgS2E
QFqTyOqGcmRAlDkfVJOYVvVrM3ZCneZLNoHKbmhAq7h6B6gRVpA45QoPeUoD
bsFMbxU+Nksn3ml/QB+kQNdcgZVpMQe4tqMzZTO4c01T4nKugIf5mJwcwCfh
bKBGVY0pVAF8qFDomuNaYqOSjKoUkMfIHViCbqO0fVJi5jZvrjWElgJuvBah
/KNYBlNAwcETZFlzmBm4RartRn2yTEslPKAUHdo40Oxwtp0rfAUSKIqjEy5m
GuPkvXK4WRvN2vxQx8SZOx9a59yIjYlA9hM3UK1Wqkddk0E+VOljdpam5TFc
NqCjO+ZK2JyfTBdMThwWRRjnT6vlQcPhJBh+A7aczdoTP1/nT3W9Wm4sMnDJ
GoFixggw6AoIFxMTpQLdQtzTrpjZD5mmZBDdYCQQoMbZkYoYkymYBxTW3EsP
ssK3G/sezHVamx7OryOULeiHwENqoNe4HVW7Vz8+qAYZgPoyK0QzM6uvEEe1
LHTJ7A6spvt+cV6ogA40X2boLJgkK5yF7PnVzUp91MN1T/q5NUyeUXEgj3TC
SZLjcek6q6VV6P/SfguGcfnE0ZRWCQQKijktHISpMClztxeAlWaW3JKv1jjS
avuMfdpfHmF0CnC6xc7cDRa5HFogjz6QwiIyO0puGJS48ZW8t03FgLCG6yav
clHFSaefBlSge675TD5F/DlVBAcvz96YfTFctA41Ou6JzRhkeudFHtHAIw8z
IwuSKVAu+Eccbu2ONiScDaqfg2zT+2lsswrhIcetLkQBAimQo3DLvJLUbHJR
yk0yLgaCLV3qmEZyYM2m80i2RJj6umFY0JkCbnejcS5ma8bz7lGDvXawzezH
0Q1vH2YzAdacTtYOWBjiWlZh+pqMYHRurOs+HC0Ge9Y1X1H5BicPW0wwSE7s
6oSrB5xPxe3NdY7bDIFXy600F8PAXn3M3lcha5ISuLbTa+473L4X6G12hhwL
RlDyrlUVA0L9DNHU4FsfbCBYzwYxUp2Ow72Lcv9RXNlNboxcd45fDAb1pHei
mLLZje3HNGhZPXfgeBHtsLzjJjX1filnEqVjKeswVmxQerXDkp3WFCy8N9A/
zHBc1WIapI1iWwHJDR47FsFwUyiqmLXlsvQq46qUAGTD9MlR/kGCErc9YbtU
IB5vZHe+Vpjul93IaEa26wAOPONOVhQSKLFuGzL5/iNaajplfHwKy0S2eHVs
5U00Exvg5vMAKCwxWcjWu6k/H75rBsBtboBUcPmjYTNzg7WazeJrrhC5cloz
nRi/ChwuKESgFmVHxJrT5t0WyCorbQZPeBs3+8DFAzBsAuvqlDUgd3KypfUJ
30StFvb4jD6tAbaZWc3Ma/1PoKUsrkGPrn0W2JxwLiusqsSK4haSM1TP9zho
Zm9/0Cx3VsBHg20odz0/r5ZZTpPK7OtxEHwVxPx1erfZPQFO5ng5XoVDcvp6
czwVmv9SAOZAdMMVGXWFrngNHMriDZarmtVTmnBZV+Ld2MiyyrEDR9kFCL2b
/WWHKKxTbIuM977f12qaQdxc4j0b50PNTW5u0SuUWHzNOwQV8Da5jjs48SJU
iLB959hQLut1J1dcs5Zg7ay6Xiub6gO0606oN/Et8ZaTFMah0cLvDjbAKw91
IAXhoMOMx6uXvLkSeRjWTGzpgICGLd09lFH0VjuaoV7pjzh87O7QwM8gEjdH
zbi1a7x5Qf9Z/aUi+OjWLDcDwHrCgihweZGjK9JkOwuMW5gWiHvuhLdeCixE
2VrLvh7WDPvqLBtXa1ZuS7DarAAfvAj8qJDg0LQ6DqywPBhQz4BGK9zEwmYg
2E42U5AsHnUOnwjJfnYomHT3jMTITebY7q4mZ6Nudd8xboaAs28+eaLlREXK
sEnqkFGBKVEZhgbC7RCBRruNHqab94edmuGe9c0OQe5mQm4f0IvRYrGaIn2o
AL4JR+7wPAg3z/tMvxnc2azZuzYYUjbH/1ytz1CfgzGaZsjTQIvwk/3EUaKh
7E94mLX6oCIoJHewvijCTAAWdzeNmIQmvx0Jz5xkC1KnGe9D8W74pid4cZwm
72uBa1ad8ohDtyIRkaHaUavVPei0sMM4NkO74Zme7BnM0HEdKxAuXoqzj8Bb
uMcPPFlhnGXi3M4euyGXbWG7x3k2gezeAmO5bltBENCygyjYiZYOddY9XRJs
pJX259b6xKpo+oJbjjlZAe+uxosbCjmWFlEhdP0fACnzywjZuO/G9GKWf2a5
ssM4G57gETvl7CV7XIxz+1O8BLo+xmWA7TjQbu3ErTjb+BPRIaIGyYSuuysL
QIF7XAkIA6yubJLlvBxQzrYb2szQz0xmsRtwK3WWyVjMgL9XPvzC0NBdwCXm
m7rr/b2bxZOOeyio7mlQdQ+udNRLyNX8ZXF1Nhm9Aj3KgULeVfU+ISx+5nnX
aPTNj59ALbJKlC2RvBotNhfNwC5jxmFI2p/4M/ua78vuz88UUUyy5jAmW2l3
W/XDCq+c2VYbE6EusOlUbxxGJ3Rt8/yrkYreK/CTn5xufQfstpmtw7ZM4d4g
u2Gb7h2kRjPzZCWRo/IzaD9AhlsUx67vlg/8gyqwOsaPzLol87yPYnWi3KZ/
XxTfesdTyM7ud0tsGRzD0bAP1u7Ucu3kYaSwW2nglh81YhnkbBgHiusKpNg3
M+x9dceHnNqUYPVSjWuuzHQz1xAtn6wYWCdoYWpyUnRoDzd091S81WFQ82AD
14OY7Fq0GoiVR0JphEmdS3Tscb0eXdx2WXGPJHbrDTMjn7JqWfaCZtBmooLj
tRynEfcTYdm/ucWpYL1rhVlXcf2ur8puoUL2jSM1cKvpGo59XehODmruUaPK
jSeIPwly11OeaO7vrILXnfKo4Vs9kcnoB3H85zR32NlqxmE7xKz7/3tEaHqi
seIGKZiKHo+JT0vbSTULF//89d37p3+/++Xnpx9+f/3+7eXD6/7+/wAd+t+U
uEcAAA==
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
