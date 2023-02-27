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
# Aggregate redblackTree_t tests
# From t0009a.sh
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

# From t0017a.sh
# Test red-black tree clear()

  l=create();

  for(x=0;x<15;x=x+1;)
  {
    y = rand(2000);
    insert(l, y);
    "Size";y=size(l);print y;
    "Check";y=check(l); print y;
    "Depth"; y=depth(l); print y;
  }

  walk(l,show);
  "Clear";clear(l);
  "Size";y=size(l);print y;

  destroy(l);

# From t0020a.sh
# Test file for redblackTree_t remove()

"Test redblackTree remove()";

l=create();
"Insert";
for(x=0; x<2500; x=x+1;)
{
  y = rand(500);
  insert(l,y);
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

# From t0028a.sh
# Test file for redblackTree_t
l=create();
"Insert";
for(x=0; x<10; x=x+1;)
{
  insert(l, x);
}

"Next";
x = 12;
p = next(l, &x);
print *p;

"Destroy";
destroy(l);

# From t0056a.sh
r=create();
print find(0,0);
print find(r,0);
print insert(0,0);
print remove(0,0);
print remove(r,0);
print clear(0);
print depth(0);
print size(0);
print min(0);
print max(0);
print next(0,0);
print next(r,0);
print previous(0,0);
print previous(r,0);
print upper(0,0);
print upper(r,0);
print lower(0,0);
print lower(r,0);
print walk(0,0);
print walk(0,show);
print walk(r,0);
print check(0);
print destroy(0);
print destroy(r);
exit;
EOF
if [ $? -ne 0 ]; then echo "Failed input cat"; fail; fi

#
# the output should look like this
#
cat > test.ok << 'EOF'
First simple test
Insert
Find
Didn't find 
1234
Found 
12
Size
50
50
Depth
9
9
Check
1
1
Min
0
1
0
NULL pointer!
0
Max
49
NULL pointer!
0
49
48
Remove
Next
0
1
3
4
5
6
7
8
9
10
11
12
13
14
15
16
17
18
19
20
21
22
23
24
25
26
27
28
29
30
31
32
33
34
35
36
37
38
39
40
41
42
43
44
45
46
47
48
49
Walk
0
1
3
4
5
6
7
8
9
10
11
12
13
14
15
16
17
18
19
20
21
22
23
24
25
26
27
28
29
30
31
32
33
34
35
36
37
38
39
40
41
42
43
44
45
46
47
48
49
Destroy
Second simple test
Insert
Remove
Removing 
2
Walk
0
1
3
4
5
6
7
8
9
Destroy
Third simple test
Insert
Remove
Walk
1
3
5
7
9
Upper/Lower
For 
1
Lower is 
1
Upper is 
1
For 
3
Lower is 
3
Upper is 
3
For 
5
Lower is 
5
Upper is 
5
For 
7
Lower is 
7
Upper is 
7
For 
9
Lower is 
9
Upper is 
9
Upper/Lower
For 
0
Lower is 
NULL pointer!
0
Upper is 
1
For 
2
Lower is 
1
Upper is 
3
For 
4
Lower is 
3
Upper is 
5
For 
6
Lower is 
5
Upper is 
7
For 
8
Lower is 
7
Upper is 
9
For 
10
Lower is 
9
Upper is 
NULL pointer!
0
Destroy
Random test
Found 
234
At 
0
Size
803
Depth
12
Check
1
Found 
517
Found 
572
Found 
1866
Found 
1689
Found 
1391
Found 
1002
Didn't find 
1255
Didn't find 
749
Didn't find 
237
Found 
464
At 
10
Size
806
Depth
12
Check
1
Didn't find 
1924
Didn't find 
1490
Didn't find 
323
Found 
1856
Found 
1870
Didn't find 
1506
Didn't find 
462
Didn't find 
1876
Found 
891
Didn't find 
992
At 
20
Size
813
Depth
12
Check
1
Didn't find 
1348
Didn't find 
776
Didn't find 
451
Didn't find 
155
Found 
1220
Didn't find 
1149
Found 
330
Found 
137
Found 
459
Found 
167
At 
30
Size
818
Depth
12
Check
1
Didn't find 
763
Didn't find 
1625
Didn't find 
1715
Found 
1851
Found 
1408
Didn't find 
984
Found 
1223
Didn't find 
198
Didn't find 
327
Didn't find 
1578
At 
40
Size
825
Depth
12
Check
1
Found 
1264
Didn't find 
1890
Found 
1738
Found 
1815
Didn't find 
37
Didn't find 
1495
Didn't find 
1530
Didn't find 
1113
Found 
1604
Didn't find 
1749
At 
50
Size
831
Depth
12
Check
1
Didn't find 
681
Didn't find 
723
Didn't find 
989
Didn't find 
1961
Didn't find 
239
Didn't find 
1786
Didn't find 
483
Didn't find 
1711
Found 
937
Found 
1535
At 
60
Size
839
Depth
12
Check
1
Didn't find 
334
Didn't find 
577
Didn't find 
410
Didn't find 
507
Didn't find 
1842
Found 
49
Found 
85
Didn't find 
1414
Found 
1388
Found 
1345
At 
70
Size
845
Depth
12
Check
1
Didn't find 
472
Found 
1447
Didn't find 
210
Didn't find 
1034
Didn't find 
406
Didn't find 
1030
Didn't find 
1151
Didn't find 
338
Didn't find 
315
Found 
1136
At 
80
Size
853
Depth
12
Check
1
Found 
56
Found 
351
Didn't find 
1362
Found 
1530
Didn't find 
153
Found 
587
Found 
1495
Didn't find 
1971
Found 
1257
Found 
715
At 
90
Size
856
Depth
12
Check
1
Found 
237
Found 
1561
Didn't find 
1497
Found 
1300
Found 
1191
Didn't find 
1491
Didn't find 
741
Didn't find 
1907
Found 
1630
Didn't find 
893
At 
100
Size
861
Depth
12
Check
1
Didn't find 
1945
Didn't find 
594
Didn't find 
308
Found 
272
Found 
1289
Didn't find 
481
Found 
1348
Didn't find 
1073
Found 
1640
Didn't find 
1266
At 
110
Size
867
Depth
12
Check
1
Found 
31
Found 
797
Found 
1150
Found 
1930
Didn't find 
610
Didn't find 
1477
Didn't find 
1704
Didn't find 
193
Didn't find 
403
Found 
602
At 
120
Size
872
Depth
12
Check
1
Didn't find 
1818
Found 
1571
Didn't find 
1253
Didn't find 
1269
Found 
280
Didn't find 
1200
Found 
1545
Found 
1748
Found 
1361
Found 
1876
At 
130
Size
876
Depth
12
Check
1
Didn't find 
1846
Found 
1888
Didn't find 
1005
Didn't find 
1337
Found 
1720
Found 
638
Didn't find 
1177
Found 
1042
Didn't find 
1983
Didn't find 
1047
At 
140
Size
882
Depth
12
Check
1
Found 
797
Found 
577
Found 
21
Found 
587
Found 
1659
Found 
1948
Didn't find 
127
Didn't find 
1210
Found 
1266
Didn't find 
1835
At 
150
Size
885
Depth
12
Check
1
Didn't find 
751
Found 
468
Didn't find 
1436
Didn't find 
213
Didn't find 
1023
Didn't find 
196
Found 
1169
Didn't find 
1875
Found 
138
Found 
1129
At 
160
Size
891
Depth
12
Check
1
Didn't find 
1237
Found 
150
Found 
1753
Didn't find 
40
Didn't find 
243
Found 
530
Didn't find 
642
Found 
485
Didn't find 
1228
Didn't find 
1055
At 
170
Size
897
Depth
12
Check
1
Found 
988
Found 
539
Found 
245
Didn't find 
114
Found 
1930
Didn't find 
1692
Didn't find 
1265
Found 
686
Didn't find 
149
Found 
1680
At 
180
Size
901
Depth
12
Check
1
Didn't find 
377
Found 
1069
Found 
1813
Found 
1542
Found 
279
Didn't find 
816
Didn't find 
1464
Didn't find 
1442
Didn't find 
1979
Found 
328
At 
190
Size
906
Depth
12
Check
1
Didn't find 
1020
Didn't find 
958
Didn't find 
266
Found 
1497
Found 
1029
Found 
786
Didn't find 
1903
Didn't find 
1788
Found 
1421
Found 
1890
At 
200
Size
911
Depth
12
Check
1
Found 
508
Found 
503
Didn't find 
488
Found 
1495
Didn't find 
1171
Didn't find 
318
Found 
1854
Didn't find 
460
Didn't find 
1861
Didn't find 
1809
At 
210
Size
917
Depth
12
Check
1
Didn't find 
1923
Found 
145
Found 
610
Found 
991
Found 
39
Didn't find 
1616
Didn't find 
614
Didn't find 
883
Didn't find 
765
Found 
516
At 
220
Size
922
Depth
12
Check
1
Found 
680
Didn't find 
1206
Didn't find 
1365
Didn't find 
392
Found 
1983
Found 
668
Found 
894
Didn't find 
1893
Found 
489
Didn't find 
578
At 
230
Size
927
Depth
12
Check
1
Didn't find 
1573
Found 
77
Didn't find 
531
Didn't find 
589
Didn't find 
887
Didn't find 
22
Didn't find 
1297
Found 
800
Found 
314
Didn't find 
370
At 
240
Size
934
Depth
12
Check
1
Found 
1048
Didn't find 
828
Found 
1482
Found 
1733
Didn't find 
1892
Found 
809
Found 
1481
Didn't find 
203
Found 
1443
Found 
1715
At 
250
Size
937
Depth
12
Check
1
Found 
1253
Didn't find 
552
Didn't find 
1440
Didn't find 
826
Didn't find 
1230
Found 
886
Found 
1210
Didn't find 
319
Didn't find 
1839
Found 
912
At 
260
Size
943
Depth
12
Check
1
Didn't find 
80
Found 
1234
Found 
93
Found 
942
Didn't find 
1199
Found 
500
Found 
937
Didn't find 
1568
Found 
1078
Didn't find 
231
At 
270
Size
947
Depth
12
Check
1
Didn't find 
714
Found 
1835
Found 
1917
Found 
838
Didn't find 
253
Found 
373
Found 
12
Found 
1864
Found 
1591
Didn't find 
294
At 
280
Size
950
Depth
12
Check
1
Didn't find 
1919
Found 
715
Didn't find 
1344
Found 
1433
Found 
1698
Didn't find 
230
Found 
1129
Found 
1908
Found 
513
Found 
1623
At 
290
Size
953
Depth
12
Check
1
Didn't find 
717
Didn't find 
1278
Found 
601
Didn't find 
505
Didn't find 
931
Didn't find 
708
Found 
489
Found 
864
Found 
231
Didn't find 
1099
At 
300
Size
959
Depth
12
Check
1
Didn't find 
1985
Didn't find 
298
Didn't find 
1613
Didn't find 
1828
Found 
1098
Found 
610
Didn't find 
1942
Didn't find 
1663
Didn't find 
1926
Found 
942
At 
310
Size
966
Depth
12
Check
1
Found 
1827
Found 
1244
Found 
982
Didn't find 
949
Found 
1842
Didn't find 
1732
Found 
67
Found 
1623
Didn't find 
404
Didn't find 
1583
At 
320
Size
970
Depth
12
Check
1
Didn't find 
1383
Found 
22
Found 
1343
Didn't find 
701
Found 
731
Found 
536
Didn't find 
1564
Didn't find 
1480
Found 
1527
Didn't find 
79
At 
330
Size
975
Depth
12
Check
1
Found 
386
Found 
1647
Found 
1894
Didn't find 
533
Found 
1715
Didn't find 
1104
Didn't find 
671
Didn't find 
1976
Didn't find 
1797
Didn't find 
803
At 
340
Size
981
Depth
12
Check
1
Didn't find 
699
Found 
662
Found 
1935
Didn't find 
1966
Found 
1020
Didn't find 
1910
Found 
209
Found 
1060
Didn't find 
1216
Found 
1020
At 
350
Size
985
Depth
12
Check
1
Didn't find 
1589
Found 
38
Didn't find 
1939
Found 
899
Didn't find 
1686
Didn't find 
1288
Found 
1363
Found 
234
Found 
864
Didn't find 
825
At 
360
Size
990
Depth
12
Check
1
Didn't find 
1160
Found 
856
Found 
1899
Didn't find 
86
Found 
1404
Found 
1099
Didn't find 
677
Found 
1838
Didn't find 
1672
Found 
741
At 
370
Size
994
Depth
12
Check
1
Found 
937
Found 
1077
Found 
80
Didn't find 
710
Found 
1846
Found 
936
Didn't find 
549
Didn't find 
576
Found 
1289
Didn't find 
337
At 
380
Size
998
Depth
12
Check
1
Didn't find 
1550
Found 
1682
Found 
1952
Found 
795
Didn't find 
1833
Didn't find 
30
Found 
786
Didn't find 
313
Didn't find 
929
Found 
1467
At 
390
Size
1003
Depth
12
Check
1
Didn't find 
1319
Didn't find 
528
Found 
1622
Found 
1719
Didn't find 
1184
Found 
461
Found 
1843
Found 
1203
Didn't find 
365
Found 
557
At 
400
Size
1007
Depth
12
Check
1
Found 
1424
Found 
824
Didn't find 
1267
Found 
148
Found 
1391
Found 
1130
Found 
1388
Found 
1798
Didn't find 
1083
Didn't find 
674
At 
410
Size
1010
Depth
12
Check
1
Didn't find 
823
Found 
1361
Found 
234
Found 
1463
Found 
1377
Found 
1211
Didn't find 
705
Didn't find 
1872
Didn't find 
90
Found 
1545
At 
420
Size
1014
Depth
12
Check
1
Didn't find 
1834
Found 
1203
Didn't find 
889
Didn't find 
273
Didn't find 
1425
Didn't find 
1796
Found 
1344
Didn't find 
224
Found 
910
Found 
278
At 
430
Size
1020
Depth
12
Check
1
Found 
1454
Didn't find 
875
Didn't find 
1887
Found 
377
Didn't find 
1192
Found 
1347
Didn't find 
1628
Found 
79
Found 
1674
Found 
913
At 
440
Size
1024
Depth
12
Check
1
Found 
1719
Didn't find 
1633
Didn't find 
1474
Didn't find 
449
Didn't find 
1379
Found 
1144
Found 
1609
Found 
1232
Didn't find 
154
Found 
1827
At 
450
Size
1029
Depth
12
Check
1
Didn't find 
1141
Found 
981
Found 
1302
Found 
1924
Found 
1180
Found 
523
Found 
13
Didn't find 
787
Didn't find 
1649
Didn't find 
1285
At 
460
Size
1033
Depth
12
Check
1
Didn't find 
1222
Found 
117
Found 
809
Found 
1956
Found 
740
Didn't find 
941
Found 
704
Didn't find 
1676
Found 
589
Found 
1674
At 
470
Size
1036
Depth
12
Check
1
Found 
450
Didn't find 
432
Didn't find 
1638
Found 
1078
Found 
1650
Found 
990
Found 
464
Found 
242
Didn't find 
980
Found 
518
At 
480
Size
1039
Depth
12
Check
1
Didn't find 
1863
Didn't find 
1991
Didn't find 
360
Didn't find 
1274
Didn't find 
1621
Found 
134
Didn't find 
1963
Found 
1454
Didn't find 
1760
Found 
1138
At 
490
Size
1046
Depth
12
Check
1
Didn't find 
1431
Found 
127
Didn't find 
957
Didn't find 
1356
Didn't find 
160
Didn't find 
1062
Found 
1416
Didn't find 
305
Didn't find 
1897
Found 
117
At 
500
Size
1053
Depth
12
Check
1
Didn't find 
304
Found 
245
Found 
461
Didn't find 
744
Didn't find 
841
Found 
1612
Didn't find 
236
Didn't find 
388
Didn't find 
1301
Didn't find 
1483
At 
510
Size
1060
Depth
12
Check
1
Found 
1647
Didn't find 
907
Found 
1485
Didn't find 
1637
Found 
1942
Didn't find 
332
Didn't find 
1656
Didn't find 
1661
Found 
1450
Didn't find 
1860
At 
520
Size
1066
Depth
12
Check
1
Found 
980
Found 
1589
Found 
1890
Didn't find 
1900
Didn't find 
661
Found 
418
Found 
162
Found 
1900
Didn't find 
1
Didn't find 
1883
At 
530
Size
1070
Depth
12
Check
1
Found 
1030
Found 
1570
Found 
919
Didn't find 
1607
Didn't find 
1219
Found 
1411
Found 
139
Found 
1870
Didn't find 
769
Found 
556
At 
540
Size
1073
Depth
12
Check
1
Found 
77
Found 
984
Found 
893
Didn't find 
520
Found 
1923
Didn't find 
1095
Found 
334
Didn't find 
716
Didn't find 
401
Found 
43
At 
550
Size
1077
Depth
12
Check
1
Found 
379
Found 
1873
Found 
1505
Found 
1295
Didn't find 
1975
Didn't find 
954
Didn't find 
646
Found 
1328
Found 
871
Didn't find 
647
At 
560
Size
1081
Depth
12
Check
1
Didn't find 
1770
Found 
984
Found 
145
Found 
1393
Didn't find 
307
Found 
1656
Didn't find 
502
Didn't find 
1800
Didn't find 
430
Found 
1658
At 
570
Size
1086
Depth
12
Check
1
Didn't find 
282
Found 
305
Found 
1805
Found 
1805
Didn't find 
1183
Didn't find 
1608
Found 
41
Found 
36
Didn't find 
424
Found 
442
At 
580
Size
1090
Depth
12
Check
1
Found 
1909
Didn't find 
1188
Found 
1790
Didn't find 
813
Found 
1927
Didn't find 
724
Didn't find 
998
Didn't find 
501
Didn't find 
1368
Didn't find 
1645
At 
590
Size
1097
Depth
12
Check
1
Didn't find 
641
Found 
671
Found 
750
Found 
1242
Didn't find 
993
Didn't find 
1986
Found 
354
Didn't find 
1867
Found 
1495
Found 
585
At 
600
Size
1101
Depth
12
Check
1
Didn't find 
1471
Didn't find 
1070
Found 
1046
Found 
31
Didn't find 
1107
Found 
56
Found 
1559
Found 
303
Didn't find 
1819
Found 
1401
At 
610
Size
1105
Depth
12
Check
1
Found 
912
Didn't find 
1501
Found 
1232
Found 
1629
Found 
1018
Didn't find 
1115
Found 
708
Found 
1033
Found 
1098
Found 
1151
At 
620
Size
1107
Depth
12
Check
1
Found 
1092
Didn't find 
345
Found 
327
Found 
1408
Didn't find 
349
Didn't find 
1325
Found 
1601
Didn't find 
1322
Found 
1257
Found 
35
At 
630
Size
1111
Depth
12
Check
1
Didn't find 
599
Didn't find 
71
Found 
68
Found 
539
Didn't find 
1101
Didn't find 
1298
Found 
1210
Didn't find 
510
Found 
1272
Found 
1926
At 
640
Size
1116
Depth
12
Check
1
Found 
256
Didn't find 
34
Found 
323
Didn't find 
58
Found 
1082
Found 
1682
Found 
488
Didn't find 
1710
Didn't find 
169
Didn't find 
1795
At 
650
Size
1121
Depth
12
Check
1
Didn't find 
1773
Found 
627
Didn't find 
738
Found 
386
Found 
1695
Found 
744
Didn't find 
842
Found 
1932
Found 
1589
Found 
408
At 
660
Size
1124
Depth
12
Check
1
Found 
124
Found 
1547
Didn't find 
1233
Didn't find 
1142
Found 
210
Didn't find 
1511
Didn't find 
262
Found 
1712
Found 
1906
Found 
1646
At 
670
Size
1128
Depth
12
Check
1
Didn't find 
897
Didn't find 
1022
Found 
31
Found 
680
Didn't find 
625
Found 
798
Didn't find 
444
Didn't find 
1697
Didn't find 
1984
Found 
1591
At 
680
Size
1134
Depth
12
Check
1
Didn't find 
1335
Didn't find 
1700
Didn't find 
181
Found 
1553
Found 
1948
Found 
1149
Found 
1918
Didn't find 
159
Didn't find 
168
Found 
1012
At 
690
Size
1139
Depth
12
Check
1
Found 
524
Found 
982
Found 
1083
Didn't find 
1690
Didn't find 
1189
Found 
1717
Didn't find 
431
Didn't find 
187
Didn't find 
867
Didn't find 
1928
At 
700
Size
1145
Depth
12
Check
1
Found 
1027
Found 
1320
Didn't find 
1865
Found 
1665
Found 
1614
Didn't find 
667
Found 
1374
Didn't find 
1004
Didn't find 
1065
Found 
795
At 
710
Size
1149
Depth
12
Check
1
Found 
1585
Didn't find 
1375
Found 
1650
Didn't find 
1423
Found 
424
Found 
710
Found 
1892
Found 
379
Didn't find 
1508
Didn't find 
1438
At 
720
Size
1153
Depth
12
Check
1
Didn't find 
1960
Found 
1816
Found 
173
Didn't find 
1406
Didn't find 
215
Didn't find 
790
Found 
825
Found 
198
Found 
862
Didn't find 
1596
At 
730
Size
1158
Depth
12
Check
1
Didn't find 
1284
Didn't find 
105
Found 
1677
Found 
1793
Didn't find 
107
Didn't find 
752
Didn't find 
2
Didn't find 
684
Didn't find 
1026
Found 
170
At 
740
Size
1165
Depth
12
Check
1
Didn't find 
1826
Didn't find 
898
Found 
179
Found 
1867
Found 
1343
Found 
1148
Didn't find 
78
Didn't find 
855
Found 
612
Found 
890
At 
750
Size
1169
Depth
12
Check
1
Didn't find 
1245
Didn't find 
691
Found 
738
Found 
1055
Didn't find 
1533
Found 
1654
Didn't find 
1965
Found 
607
Found 
1947
Found 
1604
At 
760
Size
1173
Depth
12
Check
1
Found 
1018
Found 
938
Found 
1846
Didn't find 
1281
Found 
424
Didn't find 
372
Found 
415
Didn't find 
859
Didn't find 
1457
Found 
676
At 
770
Size
1177
Depth
12
Check
1
Found 
1388
Didn't find 
1340
Found 
177
Didn't find 
1094
Found 
32
Didn't find 
495
Found 
552
Didn't find 
1941
Found 
1425
Found 
1992
At 
780
Size
1181
Depth
12
Check
1
Found 
1130
Found 
1495
Didn't find 
649
Didn't find 
1143
Found 
329
Didn't find 
94
Found 
1537
Found 
1899
Found 
850
Didn't find 
479
At 
790
Size
1185
Depth
12
Check
1
Found 
1068
Found 
769
Didn't find 
104
Didn't find 
1475
Found 
1006
Found 
727
Found 
746
Found 
1671
Didn't find 
1315
Didn't find 
905
At 
800
Size
1189
Depth
12
Check
1
Didn't find 
943
Found 
1001
Found 
223
Found 
1539
Found 
1621
Didn't find 
761
Didn't find 
1925
Found 
114
Found 
674
Didn't find 
1725
At 
810
Size
1193
Depth
12
Check
1
Found 
337
Didn't find 
46
Found 
1485
Found 
515
Didn't find 
1224
Found 
1599
Found 
998
Found 
345
Found 
1272
Found 
809
At 
820
Size
1195
Depth
12
Check
1
Found 
853
Didn't find 
1848
Found 
10
Didn't find 
348
Found 
925
Found 
337
Found 
1142
Found 
94
Found 
917
Found 
991
At 
830
Size
1197
Depth
12
Check
1
Found 
1873
Didn't find 
331
Found 
864
Didn't find 
1105
Found 
1452
Didn't find 
269
Found 
1183
Found 
1640
Didn't find 
1054
Found 
349
At 
840
Size
1201
Depth
12
Check
1
Found 
1930
Didn't find 
1855
Didn't find 
1140
Didn't find 
920
Didn't find 
129
Found 
1876
Didn't find 
346
Found 
1717
Didn't find 
206
Found 
703
At 
850
Size
1207
Depth
12
Check
1
Found 
1891
Found 
1308
Found 
125
Found 
401
Didn't find 
143
Found 
1622
Didn't find 
1781
Didn't find 
136
Found 
1795
Didn't find 
793
At 
860
Size
1211
Depth
12
Check
1
Found 
1284
Found 
1408
Didn't find 
881
Didn't find 
1194
Found 
394
Didn't find 
367
Found 
1160
Found 
1656
Found 
1223
Found 
473
At 
870
Size
1214
Depth
12
Check
1
Found 
1189
Found 
1985
Didn't find 
98
Found 
943
Found 
127
Found 
1676
Didn't find 
529
Didn't find 
830
Didn't find 
1306
Didn't find 
983
At 
880
Size
1219
Depth
12
Check
1
Found 
1615
Didn't find 
396
Didn't find 
1881
Found 
68
Didn't find 
116
Didn't find 
1161
Didn't find 
1312
Found 
708
Found 
315
Didn't find 
443
At 
890
Size
1225
Depth
12
Check
1
Found 
255
Found 
236
Didn't find 
357
Found 
317
Found 
446
Found 
502
Didn't find 
1178
Found 
479
Found 
1240
Found 
1573
At 
900
Size
1227
Depth
12
Check
1
Didn't find 
550
Found 
308
Found 
1774
Didn't find 
1445
Found 
883
Didn't find 
1249
Didn't find 
1329
Found 
677
Didn't find 
817
Found 
1062
At 
910
Size
1232
Depth
12
Check
1
Found 
790
Found 
1658
Found 
322
Found 
809
Found 
1068
Found 
1026
Didn't find 
391
Found 
584
Didn't find 
456
Didn't find 
1995
At 
920
Size
1235
Depth
13
Check
1
Didn't find 
1889
Found 
1621
Didn't find 
1972
Didn't find 
64
Didn't find 
1981
Found 
1301
Didn't find 
1021
Found 
931
Didn't find 
426
Didn't find 
1417
At 
930
Size
1242
Depth
13
Check
1
Found 
639
Found 
1622
Didn't find 
877
Didn't find 
1100
Found 
40
Found 
1505
Found 
856
Found 
912
Found 
692
Didn't find 
1207
At 
940
Size
1245
Depth
13
Check
1
Didn't find 
419
Found 
1570
Found 
1424
Didn't find 
1519
Found 
428
Found 
1837
Didn't find 
497
Didn't find 
1366
Found 
1975
Found 
769
At 
950
Size
1249
Depth
13
Check
1
Found 
1862
Found 
853
Found 
240
Didn't find 
119
Found 
1738
Found 
1827
Found 
262
Found 
701
Found 
1795
Didn't find 
135
At 
960
Size
1251
Depth
13
Check
1
Didn't find 
228
Found 
883
Didn't find 
1898
Found 
1539
Didn't find 
1598
Didn't find 
1548
Didn't find 
1847
Found 
494
Didn't find 
911
Didn't find 
53
At 
970
Size
1258
Depth
13
Check
1
Found 
22
Didn't find 
250
Found 
1664
Found 
1775
Didn't find 
1007
Found 
688
Found 
1554
Didn't find 
102
Didn't find 
1157
Didn't find 
1127
At 
980
Size
1263
Depth
13
Check
1
Found 
1621
Didn't find 
1808
Didn't find 
1706
Didn't find 
1688
Didn't find 
960
Found 
339
Found 
1781
Didn't find 
1247
Found 
827
Didn't find 
693
At 
990
Size
1269
Depth
13
Check
1
Didn't find 
123
Found 
1921
Didn't find 
1744
Found 
1754
Found 
801
Found 
1462
Didn't find 
225
Found 
1283
Found 
766
Found 
1359
At 
1000
Size
1272
Depth
13
Check
1
Size
1
Check
1
Depth
0
Size
2
Check
1
Depth
2
Size
3
Check
1
Depth
2
Size
4
Check
1
Depth
3
Size
5
Check
1
Depth
3
Size
6
Check
1
Depth
4
Size
7
Check
1
Depth
4
Size
8
Check
1
Depth
4
Size
9
Check
1
Depth
4
Size
10
Check
1
Depth
4
Size
11
Check
1
Depth
5
Size
12
Check
1
Depth
5
Size
13
Check
1
Depth
5
Size
14
Check
1
Depth
5
Size
15
Check
1
Depth
5
397
479
554
628
632
936
971
1032
1160
1173
1198
1274
1331
1470
1864
Clear
Size
0
Test redblackTree remove()
Insert
Remove
Destroy
Insert
Next
NULL pointer!
0
Destroy
0
0
0
0
0
0
0
0
0
0
0
0
0
0
0
0
0
0
Bad walk function
0
0
Bad walk function
0
0
0
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
