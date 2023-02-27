#!/bin/sh
#
prog="splayCache_interp"
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
# Test file for cache_t

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

# Test NULL pointers
print find(0,0);
print insert(0,0);
print depth(0);
print size(0);
print walk(0,0);
print check(0);
clear(0);
destroy(0);

#Done
exit;
EOF
if [ $? -ne 0 ]; then echo "Failed input cat"; fail; fi

#
# the output should look like this
#
cat > test.ok.gz.uue << 'EOF'
begin-base64 664 test.ok.gz
H4sICCFLX1QAA3Rlc3Qub3V0AJWdS48tSXWF5/kreuZpRsZ7aBsheWoGHreg
kVtAN4JGlv3rvb+Vp6q69ipunRKCOjdPZGTs99qPPPz2x7/9/Zfv/v7jX/76
5x++++WHv/9y/MdPf//hb78cv/3xpz8cv/nxDz/9yy/f/TE+f3eUq7bjtz//
4/58/OaHv/7y38d1Hr/78f9+OPp5/Pt///D7Px3l+K/v//wnFrR9tHW0ebRx
tH60drR6tLhejnYedR91HXUedRy1H7F1rUe9jlqOeh7XPq51XPO4xnH142rH
VY/rOq7CE8s+yjrKPMo4Sj9KO0o9SjnKeexjHfMYRzzviDviOHGwP//w/d/u
Y/0mKPzbz/97/O6H3/8cdHxAt5Z9tMvLrf/5/U9/+Pkv9z1xlPOM/4mz7Dju
MYOcoDgojRPtuH2NGtuUFiyI07bgUpy77x1P6GflwGUGb+LsA7quYwSPgpgx
hsjqrAj6oTiuXMHEIHsFk4IBLZgWX8w+YcoYC/bEGnFqjQuexZbBvLoKbCzr
hKFxtBPethBPcLnUOGQwvPQh3pcyOmIodV5IpOx1IZxYV5DTjrtDYuUcG+Ht
PRHjCDJCoGXFOYP0suMfIeY5NwIv176QfWwtNSjn5PlB45goR9kdNemwbh4z
6IizdS6GKNZCvWYcqcdOox09NuorllcYH5fhyjw6+wQnOloR9AVf4ut69DjW
Di72OEqcZQSvQlpBzAoKgk31jD/Bgg77OWI/ggM9OIUUrhnr2YmvJb3YItQz
mDMRTtA+Ub04ZVwqKx48IZC/sVuJf8xQhHjGjN3OPdCSMkP6E/0vx0TXQzFi
s3bGoYLJcSM8Dz1Z8D0uLaiOqx3pcfJjx+HWRLrx5FJCMAslmUeIZ8cXsaqF
APatU1usin/Gka5Qt4202CTWrSNEOEJowZUV6hyPrSIhTlTPoClOFXRuiAop
8U2Vqp0IEGGcHAsVOeNMG6uYnC2YEk9DDEF9hzuhzg2LjRN0tBONR/VC5Se7
ofMFTUVcU7oYstOH2KWiQVL8+LP5N5qF3XSoigtoRCh/qcH98FdBHN9yzMcy
NKxgA8irsGqw020GfMW2HB1DQDkLltDEd6kvC8PGB09rIUJ4FF+OjV1xWN2D
FDgG+lIkMyyCnbB47R3/2GJg2CqsgyhYFFbReQpW0bjYZPjcEjsvdL2xLUoX
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
s+yZn/OhWs+n9dWw68ilyWJ4gyHQ1/q41xMzg4eVPXZufRQr9jCKIVregnz/
DHrlQDCsp7PdK1s+bvlB2dlYLov6p4FiXsh6YRJbipjnK/nFsJTl2TsH4JUZ
UDwpvLyenc38V2+EW3hhzESUPB/jS8m8+aBBl0/uyez5FltPE7RlqRZby8hl
T2q0ImV+y1pe3J1BCN51fuFSom+ac7Xi2c4G37Of62YjVqaluCYK1hfUyuoq
w3DvznByWuK4cqzPYNKS4poD1Lb43166kW/BvX3qiDNS6Rb2hnnoaWC25O5j
85zWULshe0vsmDK/25HPm0vL3e5l7W9eIMzBIfPFmhTF28PmC6bhitPS0XnD
yVaeFtKyzP5XxWhD/JrOSMvNvq/cbpxWel7Z4PPMgMrmIuUrVf1f/1xGlv/K
FnoZgG/eLbeqq0HrK2uAR6SXvvdb6P8sXOpN3/fHt9qu5e8uiWKlyWoIgsH4
18K7+WWj7Y787Zs9/Df8YRtaGtLyI5qBymrnKpbfDIv6VwaeJbNUmFfU9OcF
U3L3cOeagn7BITlt8xIlx6XudpidtknOOMWbfSJoPB90LvPAlgsbb3cGCTOX
NHZmkzf2Rg6ioKrX1ODFk82nzb/l6NlMA7ydZaUFzbu9JyX7JquGXznr2Sbd
8nABr2ggxP2ZN/MUzwqrjmyzOZVhtWxz6owEvtahzUqmPYS2kqj5AhZolmzn
BGnnyYFSDYvaUU5LI1quLVePPm8NRpT9HuN52mJq1uTrrRZuyGRmT7Vs+mDk
wtGV8ecHcy45ay3tkSj3t9D/aSXJ0kkfhPAm3DC0bdWJ6obnkvTBDkO/TIOL
pm8m/y9ZgKfgNk61DGbsXI+0czXruvxqfsZuNyN+EUt93lR8dKjnGUmrAZZh
ortsTcvArFhnxqYxZ24p8naVKHo++c+4xMYJrUfSPc+3+HjmxMcm0mZ2BS2b
TXtIp39LwRLIcpZZi9U8zvXB5FSuYmbHO2yoo+b0aeXqzXjMWfQvAABeM3q1
IcP7VlrOomL6/TVqZzPvBoWshdS8od7v8NKfr+7zsymPQ3zg7s+3Ud1l3xYb
Vhm51J1ddvbPlhW2RwG2P9/M1y+g5JN5CpirNVZZ3lZ3tPwk55Hdgkm1Fsx4
pGN9P61YI7PNao3TPL4jqaxuZVvxwnG9p+DZ/vp6mXd93lCa1UpPm10/s9Ea
7CnZUVto7HmOqVqXcrlvP++K2Xg+9d/WsXJN8CxqWGnotAhZcuF5ZpPSL15k
f27b9AdNXyj+nznlrdmF+WC6zbtXz0GtNGB9qVK9gZ4h7cuU9fMz/T3XU83l
fzbOVIpL1VhtPeVudVKfTaXlJHKeb/ZfNp+cBwhynM8DQPr1gSSKfKW55zRy
bFSKQqqoeasGfNr8mzn2D3O/NuTtXZ3snD5IGrxZYrZrg8LnHUbHV0b9rP9m
9aLLDLfYRJkxu2fkeVneNs0bbRvOARKJpLeqwGdJzvpgRuwNK3w2IGTvz1gR
tnkpyp7osIpBABHyBYBQrT82LfuwWlS3IWafifVhxO3j0d69sihwY57xfPe/
GzQx07YSdxmWwxXL82yswKsPuZrmmGE/5hfnF4YBTost1TKZlRsR+vWqdCXP
VQ07XrVKz2kltjNv/OLj5hcmA7qVAKrVoq1oVpr19rOwP+jEWey2ZMuGKkt7
FKPm9TxJ28ckbebVuwPZHV2GcgyeG2rI7mOZH+R3e0TPF+b/L3MwluZ4G3Ya
tM7IdOZeYx7l8MfaJNtjHnB+4aU/G4dbBlc8CXf7sJagNeJy7XflN2atNPcy
BjzfMMKndVwbJ+cXZ146Ll6Qzmco3Ue3LOPZNnFu5aDtnZ/zrrHP8XQ0day/
/R1Mq1tcb68smReoGVW2bFXLYk/LsHo83pmZz7/39+HLg/Zag2EHe4Mrn94S
TRv/tNbIB33Hl3eY5xf6Bd5DtsN4x6hkK6k5vbOB5m4FYJ8QsVbMY/ppvr0L
8Gmn7cwQw0qRNs1UmkWmM3vtaQjd9NWHouzNTH4s434x82kvvc0hnTkx87eq
PZuzfs60Svo2fTI4YeBhPoaHVnnahOz9MWOkdQ+6TaRZ+7xYxmsFK0vpPTd9
eclhPY8KlkHl5a9X5oNkb3i9lhr97bry9rJzNiqbP96PBGF95S3ADFlqRr0+
kexAoVnQN7P7oGRqrvPMMao+Xphf7Xkf7S9bWYguxVrPhritYrVy67ma6loK
Ya+KzMcU5PrC4KD9WEWxV4BLNl5rWpjP/miKyqcSfKzT4OuDoDc88IGTfh2w
8V5BJmXZIYrFT7tgQM6bvt5NNMfJz+CKli8gAs8jbbb7bUjbvbmlfj7gkBVx
+dhX1rL9aCCu9bwjGPYrGDbGZKKxfN4SomJxpmZ4bGVei5vt0XFb3/w5gPeG
l03e++NWZrX8P9u3N6b4daEXkGrpxWX+rT8UbJ9PIzQb73TbnxaXW4519mZc
uT6oVn82hLvsxQsmKUTQ8wMElu+qdZfg5PU2ifs23mPYzjNHG5LsNgZqbmA/
yhv7+kLU/PXUkc3K7AwsLIJ+NP/lZaGMBXNkbpY4t8dkyn7+rUB+EvWNlIyI
fDrQWrGm5Bkd2HS49ZA+eBv6fBDSniakWXvLJiB8FLf0fFezceNlkNV/TcRe
OrBWPYmIKPpK5M+FHoOb7mOMCx/84FGOOlZY5wWX14FMG0N4NKT28yWA621e
1F2RV2o8gem+xnD2ysWK5ojZXtO5KXm+NZAt5PLxe7P2+UGCmYsRVljoVq/x
6GPDb+UxnLq/+QLBP/dcy4OL91PyUa0wWn/lUhxTXllMpo7jgSn3W+L/2SxH
8Z++MOKsQ6bfx3x/FIfN2TQuy5Mvf+3chnD6fvkloG9q2uP/vPB8/Offvv/D
d//z/Z//9N0f//HT73/58eefdPX/Aa/cNHgNcgAA
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
