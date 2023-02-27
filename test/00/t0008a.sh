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
# Test file for list_t

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
p = find(l,x);
if (p == 0)
{
  "Didn't find "; print x;
}
else
{
  "Found "; print *p;
}

p = find(l,13);
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

"First";
p=first(l);
print *p;
p=next(l, p);
print *p;
p=first(l);
print *p;
p=previous(l, p);
print *p;

"Last";
p=last(l);
print *p;
p=next(l, p);
print *p;
p=last(l);
print *p;
p=previous(l, p);
print *p;

"Remove";
x=2;
p=find(l,&x);
remove(l, p);

"Next";
x=0;
p=first(l);
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
  y = rand(20000);
  insert(l, y);
}
walk(l, show);
for (x=0;x<1000;x=x+1;)
{
  y = rand(20000);
  p = find(l, y);
  if (p != 0)
  {
     "Found "; print *p;
  }
  else
  {
     "Didn't find "; print y;
  }
}
destroy(l);
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
Found 
13
Size
50
50
First
0
1
0
NULL pointer!
0
Last
49
NULL pointer!
0
49
48
Remove
Next
0
1
2
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
2
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
Random test
10014
17995
777
4745
1392
8643
14476
14514
15998
5035
17466
6204
6435
6663
15108
2194
12450
8320
4350
7579
6690
12443
8629
126
3817
1801
13906
4484
13637
15619
11663
13735
19832
13622
9911
10696
9981
6459
18170
19393
7793
12927
16556
10783
14681
1957
5744
7220
5959
8893
7313
1650
15869
17922
1775
19214
15285
17943
19165
4418
6987
8699
3096
2524
16564
5263
12710
17535
15922
2729
15382
10888
18278
10275
11737
8100
10966
17041
3712
2804
14023
3747
3997
13816
5242
13925
9668
241
11347
1675
9876
4411
6442
17917
12120
3867
9190
6396
8009
3734
13031
15399
4634
218
14535
13343
8904
872
12577
5728
4151
5240
6302
7045
11255
5706
19710
13428
438
13423
1253
13544
13284
9104
14206
5745
10893
16199
13648
13886
14678
14858
16596
4616
7811
16969
19687
9031
15158
19324
17314
19052
8335
1730
5587
14436
12411
18551
15719
2475
2653
11368
1999
19657
6315
4228
19076
9854
121
2786
14787
13755
13651
5423
4167
11465
13579
10150
15949
10472
11735
3793
1348
18854
6066
8953
9140
11942
14330
246
5738
8954
18537
9365
9640
1459
19359
13248
5008
2231
835
7219
10304
16486
5099
12963
12220
18241
19369
19698
12271
10438
7630
6237
3625
14798
4856
13821
9874
6735
14052
14344
7077
17183
17376
7571
5180
16205
7078
15772
19518
7161
13308
14549
1429
14125
13458
10967
1158
17406
604
4405
5989
6730
6867
5800
5827
1892
12897
1390
3835
6439
17587
14081
19194
3545
9426
18122
15259
13090
18671
15662
10028
5818
15361
7721
14511
5837
14628
6050
1568
70
2792
10168
7410
10824
3754
9145
11448
7258
8032
14833
11488
19223
1508
12639
9857
4082
15803
107
9892
8534
7931
15658
1735
8663
17534
2459
13010
17322
16720
17489
6029
19002
9973
10187
7464
9748
10042
11100
7962
11808
9386
111
10427
4387
8865
13525
19107
9798
17291
11693
1264
1241
920
8856
169
6672
3959
4649
298
13524
5749
1102
10127
7648
14143
1701
5169
2558
17063
12837
2613
4785
8645
5847
17430
8812
12359
8384
311
15370
108
18891
11729
3828
4964
2747
2969
16961
9728
16705
16765
8770
5541
2938
15546
8715
4221
2410
2355
2704
16999
134
12956
8966
4862
8505
11404
4811
14175
1728
10934
9834
8951
11709
3533
5644
5087
9216
8474
17310
9506
5134
7052
16303
10178
10428
13506
8804
19510
10722
18217
15397
5257
13700
16654
9000
2467
12960
17634
5307
14937
15180
16518
15213
14013
10692
17256
502
1947
13946
5448
2859
4810
9752
2275
13854
17058
4855
3293
15162
6831
19136
7871
16191
2428
18714
13604
3287
7987
7314
18957
19580
4429
14060
5926
16663
19935
9218
9177
16408
6554
3568
12245
14860
18876
16469
4668
923
9445
1002
6170
17338
7460
7872
16948
5573
2915
4732
10341
19094
12882
10694
4359
15861
3178
19352
18391
18521
316
18266
5729
15352
4753
3751
16286
18380
4473
10906
18188
3040
4590
17096
12644
7507
9400
9568
3256
13314
13997
5796
8955
17699
11747
3936
18788
7974
1679
10928
1758
17326
6694
12131
16308
13891
702
9826
17382
8368
8246
2487
9646
16988
8017
17596
12729
13717
7318
391
8079
3518
17443
13011
1176
17591
11391
19900
6989
13781
12768
11348
9334
7050
8731
5181
7990
18264
482
10676
13480
11397
7746
879
3552
3164
14705
5613
8689
14447
19450
14381
9511
12652
476
7483
17758
8094
11469
2402
3475
2023
5445
4369
7673
13825
17206
527
3097
679
2141
7817
14248
7041
11168
11184
4289
6696
12374
11507
1794
7199
10926
18656
14606
11352
5363
16598
7682
18167
11264
4596
16652
17493
10793
12614
9799
4240
10941
7669
10827
11673
822
6379
2767
3308
5002
13442
11380
16783
14221
13367
15479
567
16150
1584
5096
1111
9651
16076
6305
15610
13215
3861
18066
16587
5394
3144
14789
5875
17887
5226
10173
1485
3977
5960
10851
4849
14669
18079
3732
13465
850
11703
16129
8456
16418
12964
13141
2426
8405
14058
14994
7756
6357
5191
17153
14939
7279
9818
4210
6626
18307
10188
3645
16232
18709
12259
16788
11307
12180
14988
5868
16400
15709
10102
5563
6014
15060
2347
16778
9772
1172
7372
8048
17828
6275
19573
7157
17642
19320
3838
17130
7841
417
10986
12591
2195
17405
4894
18976
1628
15504
12575
9378
6371
15872
9072
17797
3223
18224
6927
4507
7344
10359
8517
352
2264
19282
18738
16051
11536
19528
6879
3874
5722
9861
14799
4949
18677
754
6738
19347
1402
7543
15127
15604
10530
15465
3397
6132
11035
10607
9244
17324
555
9516
10916
6249
6041
6597
12434
16470
11387
11376
6839
10486
8154
2559
17144
386
85
16812
14788
4465
7329
634
11834
19497
12951
5063
851
6088
6800
366
323
15215
13850
291
19336
8374
18553
11319
8411
4581
13223
331
19308
18160
16165
14637
7430
16020
12037
6858
11914
14590
12692
1371
8205
16350
15436
14401
13508
9790
277
683
19785
6765
10137
5415
17599
10469
5670
10787
14205
8389
12694
11115
9260
8717
6200
16905
12845
2763
1941
280
8046
2803
14165
18178
16837
19182
16241
17539
5245
3949
19748
2778
5395
10026
19484
6052
16754
8703
14095
2815
7598
2568
14466
17757
917
9131
18032
10065
18445
14049
2881
3521
10642
14819
17232
133
10134
11178
3359
921
14227
19370
8005
15956
9479
2188
2762
9388
4190
17557
4683
18138
5160
1037
14486
19149
4910
18592
1641
17810
11878
17209
13292
1388
12209
18021
7153
14599
5607
11398
1896
18568
2862
13429
7215
19442
4415
8279
13766
444
13597
7065
9202
13167
15325
15449
16132
569
11303
11451
15697
5664
4399
10021
17217
13780
13084
658
6388
16103
5853
932
6076
5615
2092
4574
7405
7280
6804
8563
2193
14208
6889
3905
19985
19536
12333
14357
11824
6129
8914
16862
16512
11487
17937
15554
17732
9920
8125
11923
7944
12513
4968
17387
14639
15592
11074
1781
16800
18919
19809
14276
13858
14048
492
18631
721
12732
12347
11482
10037
7302
Didn't find 
2342
Didn't find 
5175
Didn't find 
5731
Didn't find 
18678
Didn't find 
16902
Didn't find 
13917
Didn't find 
10032
Didn't find 
12564
Didn't find 
7495
Didn't find 
2375
Didn't find 
4643
Didn't find 
19254
Didn't find 
14911
Didn't find 
3237
Didn't find 
18573
Didn't find 
18708
Didn't find 
15073
Didn't find 
4626
Didn't find 
18773
Didn't find 
8921
Didn't find 
9924
Didn't find 
13491
Didn't find 
7765
Didn't find 
4512
Didn't find 
1552
Didn't find 
12205
Didn't find 
11500
Found 
3308
Didn't find 
1379
Didn't find 
4600
Didn't find 
1677
Didn't find 
7637
Didn't find 
16267
Didn't find 
17159
Didn't find 
18526
Didn't find 
14089
Didn't find 
9844
Didn't find 
12242
Didn't find 
1984
Didn't find 
3273
Didn't find 
15792
Didn't find 
12646
Didn't find 
18913
Didn't find 
17390
Didn't find 
18165
Didn't find 
370
Didn't find 
14961
Didn't find 
15316
Didn't find 
11136
Didn't find 
16055
Didn't find 
17501
Didn't find 
6822
Didn't find 
7234
Didn't find 
9895
Didn't find 
19625
Didn't find 
2397
Didn't find 
17874
Didn't find 
4834
Didn't find 
17121
Found 
9378
Didn't find 
15358
Didn't find 
3346
Didn't find 
5776
Didn't find 
4109
Didn't find 
5072
Didn't find 
18435
Didn't find 
498
Didn't find 
859
Didn't find 
14156
Didn't find 
13890
Found 
13465
Didn't find 
4726
Didn't find 
14477
Didn't find 
2110
Didn't find 
10347
Didn't find 
4065
Didn't find 
10312
Didn't find 
11517
Didn't find 
3389
Didn't find 
3157
Found 
11368
Didn't find 
564
Didn't find 
3520
Didn't find 
13635
Didn't find 
15313
Didn't find 
1531
Didn't find 
5877
Didn't find 
14960
Didn't find 
19728
Didn't find 
12585
Didn't find 
7159
Didn't find 
2380
Didn't find 
15625
Didn't find 
14980
Didn't find 
13014
Found 
11923
Didn't find 
14917
Didn't find 
7421
Didn't find 
19086
Didn't find 
16309
Didn't find 
8937
Didn't find 
19468
Didn't find 
5946
Didn't find 
3089
Didn't find 
2726
Didn't find 
12902
Didn't find 
4821
Didn't find 
13487
Didn't find 
10743
Didn't find 
16415
Didn't find 
12670
Found 
316
Didn't find 
7976
Didn't find 
11508
Didn't find 
19315
Didn't find 
6107
Didn't find 
14785
Didn't find 
17052
Didn't find 
1932
Didn't find 
4041
Didn't find 
6028
Didn't find 
18194
Didn't find 
15717
Didn't find 
12541
Didn't find 
12704
Didn't find 
2805
Didn't find 
12011
Didn't find 
15460
Found 
17493
Didn't find 
13625
Didn't find 
18770
Didn't find 
18477
Didn't find 
18895
Didn't find 
10060
Didn't find 
13378
Didn't find 
17214
Didn't find 
6391
Didn't find 
11777
Didn't find 
10432
Didn't find 
19838
Didn't find 
10481
Didn't find 
7979
Didn't find 
5780
Didn't find 
215
Didn't find 
5878
Didn't find 
16606
Didn't find 
19494
Didn't find 
1272
Didn't find 
12109
Didn't find 
12674
Didn't find 
18363
Didn't find 
7522
Didn't find 
4687
Didn't find 
14371
Didn't find 
2140
Didn't find 
10238
Didn't find 
1961
Didn't find 
11702
Didn't find 
18760
Didn't find 
1382
Didn't find 
11302
Didn't find 
12381
Didn't find 
1501
Didn't find 
17545
Didn't find 
407
Didn't find 
2438
Didn't find 
5309
Didn't find 
6424
Found 
4856
Didn't find 
12290
Didn't find 
10559
Didn't find 
9886
Found 
5395
Didn't find 
2452
Didn't find 
1144
Didn't find 
19316
Didn't find 
16935
Didn't find 
12656
Didn't find 
6865
Didn't find 
1494
Didn't find 
16810
Didn't find 
3779
Didn't find 
10702
Didn't find 
18146
Didn't find 
15427
Didn't find 
2797
Didn't find 
8168
Didn't find 
14654
Found 
14436
Didn't find 
19802
Didn't find 
3283
Didn't find 
10208
Didn't find 
9593
Didn't find 
2667
Didn't find 
14981
Didn't find 
10299
Didn't find 
7867
Didn't find 
19046
Didn't find 
17891
Didn't find 
14219
Didn't find 
18914
Didn't find 
5090
Didn't find 
5041
Didn't find 
4886
Didn't find 
14961
Didn't find 
11724
Didn't find 
3184
Didn't find 
18548
Didn't find 
4610
Didn't find 
18625
Didn't find 
18104
Didn't find 
19238
Didn't find 
1453
Didn't find 
6104
Didn't find 
9916
Found 
391
Didn't find 
16171
Didn't find 
6142
Didn't find 
8834
Didn't find 
7663
Found 
5169
Didn't find 
6810
Didn't find 
12066
Didn't find 
13656
Didn't find 
3921
Didn't find 
19845
Didn't find 
6687
Didn't find 
8950
Didn't find 
18940
Didn't find 
4893
Didn't find 
5785
Didn't find 
15742
Didn't find 
779
Didn't find 
5320
Didn't find 
5901
Didn't find 
8875
Didn't find 
220
Didn't find 
12980
Didn't find 
8004
Didn't find 
3143
Didn't find 
3704
Didn't find 
10490
Didn't find 
8285
Found 
14833
Didn't find 
17347
Didn't find 
18934
Found 
8100
Didn't find 
14820
Didn't find 
2033
Didn't find 
14445
Didn't find 
17166
Didn't find 
12543
Didn't find 
5525
Didn't find 
14411
Didn't find 
8267
Didn't find 
12307
Didn't find 
8870
Didn't find 
12111
Didn't find 
3192
Didn't find 
18399
Found 
9131
Didn't find 
801
Didn't find 
12350
Didn't find 
937
Didn't find 
9428
Didn't find 
11997
Didn't find 
5006
Didn't find 
9382
Didn't find 
15692
Didn't find 
10785
Didn't find 
2317
Didn't find 
7148
Didn't find 
18360
Found 
19182
Didn't find 
8385
Didn't find 
2539
Didn't find 
3735
Didn't find 
120
Didn't find 
18650
Didn't find 
15926
Didn't find 
2950
Didn't find 
19200
Found 
7161
Didn't find 
13449
Didn't find 
14337
Found 
16988
Didn't find 
2309
Found 
11303
Found 
19094
Didn't find 
5139
Didn't find 
16239
Didn't find 
7181
Didn't find 
12795
Didn't find 
6016
Didn't find 
5060
Didn't find 
9319
Didn't find 
7091
Didn't find 
4901
Didn't find 
8648
Didn't find 
2312
Didn't find 
11003
Didn't find 
19865
Didn't find 
2985
Didn't find 
16137
Didn't find 
18289
Didn't find 
10985
Didn't find 
6112
Didn't find 
19428
Didn't find 
16646
Didn't find 
19276
Didn't find 
9428
Didn't find 
18287
Didn't find 
12449
Didn't find 
9828
Didn't find 
9495
Didn't find 
18435
Didn't find 
17331
Didn't find 
670
Didn't find 
16242
Didn't find 
4047
Didn't find 
15841
Didn't find 
13838
Didn't find 
226
Didn't find 
13445
Didn't find 
7022
Didn't find 
7319
Didn't find 
5372
Didn't find 
15653
Didn't find 
14809
Didn't find 
15281
Didn't find 
791
Didn't find 
3865
Didn't find 
16479
Didn't find 
18948
Didn't find 
5341
Didn't find 
17161
Didn't find 
11052
Didn't find 
6721
Didn't find 
19778
Didn't find 
17978
Didn't find 
8035
Didn't find 
6997
Didn't find 
6631
Didn't find 
19368
Didn't find 
19674
Didn't find 
10209
Didn't find 
19109
Didn't find 
2094
Didn't find 
10605
Didn't find 
12167
Didn't find 
10206
Didn't find 
15900
Didn't find 
381
Didn't find 
19405
Found 
9000
Didn't find 
16876
Didn't find 
12887
Didn't find 
13641
Didn't find 
2345
Found 
8645
Didn't find 
8255
Didn't find 
11608
Didn't find 
8569
Didn't find 
19004
Didn't find 
863
Found 
14052
Didn't find 
10998
Didn't find 
6781
Didn't find 
18395
Didn't find 
16733
Didn't find 
7417
Didn't find 
9381
Didn't find 
10784
Didn't find 
808
Didn't find 
7107
Didn't find 
18477
Didn't find 
9370
Didn't find 
5497
Didn't find 
5765
Found 
12897
Didn't find 
3373
Didn't find 
15510
Didn't find 
16827
Didn't find 
19530
Found 
7962
Didn't find 
18339
Didn't find 
301
Didn't find 
7864
Didn't find 
3135
Didn't find 
9302
Didn't find 
14685
Didn't find 
13198
Didn't find 
5291
Didn't find 
16235
Found 
17206
Didn't find 
11847
Didn't find 
4615
Didn't find 
18440
Found 
12037
Didn't find 
3660
Didn't find 
5574
Didn't find 
14255
Didn't find 
8249
Didn't find 
12677
Didn't find 
1482
Didn't find 
13920
Didn't find 
11313
Didn't find 
13889
Didn't find 
17991
Didn't find 
10841
Didn't find 
6747
Didn't find 
8242
Didn't find 
13621
Didn't find 
2341
Didn't find 
14638
Didn't find 
13776
Found 
12120
Didn't find 
7057
Didn't find 
18730
Didn't find 
901
Didn't find 
15456
Didn't find 
18349
Found 
12037
Didn't find 
8903
Didn't find 
2738
Didn't find 
14261
Didn't find 
17972
Didn't find 
13449
Didn't find 
2241
Didn't find 
9109
Didn't find 
2787
Didn't find 
14552
Didn't find 
8762
Didn't find 
18883
Didn't find 
3775
Didn't find 
11927
Didn't find 
13478
Didn't find 
16290
Didn't find 
798
Didn't find 
16748
Didn't find 
9136
Didn't find 
17201
Didn't find 
16345
Didn't find 
14755
Didn't find 
4497
Didn't find 
13802
Didn't find 
11445
Didn't find 
16102
Didn't find 
12326
Didn't find 
1542
Didn't find 
18285
Didn't find 
11419
Didn't find 
9820
Didn't find 
13033
Didn't find 
19253
Didn't find 
11812
Found 
5240
Didn't find 
138
Didn't find 
7883
Didn't find 
16503
Didn't find 
12862
Didn't find 
12234
Didn't find 
1173
Didn't find 
8098
Didn't find 
19572
Didn't find 
7404
Didn't find 
9419
Didn't find 
7048
Didn't find 
16769
Didn't find 
5900
Didn't find 
16750
Didn't find 
4510
Didn't find 
4327
Didn't find 
16392
Didn't find 
10794
Didn't find 
16511
Didn't find 
9912
Didn't find 
4643
Found 
2428
Didn't find 
9807
Didn't find 
5186
Didn't find 
18644
Didn't find 
19925
Didn't find 
3610
Didn't find 
12749
Didn't find 
16221
Didn't find 
1341
Didn't find 
19648
Didn't find 
14553
Didn't find 
17609
Didn't find 
11386
Didn't find 
14319
Didn't find 
1276
Didn't find 
9583
Didn't find 
13575
Didn't find 
1608
Didn't find 
10631
Didn't find 
14172
Didn't find 
3053
Didn't find 
18982
Didn't find 
1173
Didn't find 
3047
Didn't find 
2453
Didn't find 
4617
Didn't find 
7451
Didn't find 
8418
Didn't find 
16131
Didn't find 
2364
Didn't find 
3885
Didn't find 
13023
Didn't find 
14838
Didn't find 
16482
Didn't find 
9079
Found 
14858
Didn't find 
16380
Didn't find 
19433
Didn't find 
3329
Didn't find 
16571
Didn't find 
16622
Didn't find 
14515
Didn't find 
18608
Didn't find 
9813
Didn't find 
15905
Didn't find 
18916
Didn't find 
19017
Didn't find 
6615
Didn't find 
4182
Didn't find 
1630
Didn't find 
19008
Didn't find 
15
Didn't find 
18841
Didn't find 
10312
Didn't find 
15717
Didn't find 
9196
Didn't find 
16084
Didn't find 
12202
Didn't find 
14118
Didn't find 
1399
Didn't find 
18716
Didn't find 
7697
Didn't find 
5568
Didn't find 
779
Didn't find 
9845
Didn't find 
8942
Didn't find 
5209
Didn't find 
19245
Didn't find 
10962
Didn't find 
3344
Didn't find 
7163
Didn't find 
4016
Didn't find 
430
Didn't find 
3799
Didn't find 
18744
Didn't find 
15059
Didn't find 
12955
Didn't find 
19768
Didn't find 
9545
Didn't find 
6466
Didn't find 
13288
Didn't find 
8720
Didn't find 
6477
Didn't find 
17717
Didn't find 
9851
Didn't find 
1456
Didn't find 
13941
Didn't find 
3078
Didn't find 
16577
Didn't find 
5027
Didn't find 
18016
Didn't find 
4310
Didn't find 
16588
Didn't find 
2823
Didn't find 
3060
Found 
18066
Didn't find 
18064
Didn't find 
11835
Didn't find 
16092
Didn't find 
416
Didn't find 
364
Didn't find 
4244
Didn't find 
4427
Didn't find 
19100
Didn't find 
11888
Didn't find 
17913
Didn't find 
8142
Didn't find 
19279
Didn't find 
7250
Didn't find 
9989
Didn't find 
5018
Didn't find 
13694
Didn't find 
16466
Didn't find 
6421
Didn't find 
6719
Found 
7507
Didn't find 
12432
Didn't find 
9938
Didn't find 
19869
Didn't find 
3549
Didn't find 
18680
Didn't find 
14963
Didn't find 
5858
Didn't find 
14723
Didn't find 
10713
Didn't find 
10468
Found 
311
Didn't find 
11079
Found 
569
Didn't find 
15605
Didn't find 
3035
Didn't find 
18206
Didn't find 
14021
Didn't find 
9125
Didn't find 
15022
Didn't find 
12328
Didn't find 
16307
Didn't find 
10186
Didn't find 
11157
Didn't find 
7090
Didn't find 
10344
Didn't find 
10993
Didn't find 
11525
Didn't find 
10930
Didn't find 
3460
Didn't find 
3277
Didn't find 
14092
Didn't find 
3494
Didn't find 
13256
Didn't find 
16019
Didn't find 
13235
Didn't find 
12583
Found 
352
Didn't find 
5994
Didn't find 
716
Didn't find 
684
Didn't find 
5396
Didn't find 
11023
Didn't find 
12993
Didn't find 
12106
Didn't find 
5107
Didn't find 
12733
Didn't find 
19270
Didn't find 
2570
Didn't find 
346
Didn't find 
3238
Didn't find 
588
Didn't find 
10830
Didn't find 
16835
Didn't find 
4890
Didn't find 
17114
Didn't find 
1700
Didn't find 
17961
Didn't find 
17744
Didn't find 
6282
Didn't find 
7383
Didn't find 
3865
Didn't find 
16965
Didn't find 
7445
Didn't find 
8431
Didn't find 
19334
Didn't find 
15897
Didn't find 
4087
Didn't find 
1250
Didn't find 
15478
Didn't find 
12342
Didn't find 
11425
Didn't find 
2110
Didn't find 
15125
Didn't find 
2623
Didn't find 
17137
Found 
19076
Didn't find 
16476
Didn't find 
8982
Didn't find 
10225
Didn't find 
319
Found 
6804
Didn't find 
6260
Didn't find 
7991
Didn't find 
4450
Didn't find 
16982
Didn't find 
19851
Didn't find 
15918
Didn't find 
13365
Didn't find 
17012
Didn't find 
1815
Didn't find 
15543
Didn't find 
19492
Didn't find 
11502
Didn't find 
19193
Didn't find 
1599
Didn't find 
1684
Didn't find 
10126
Didn't find 
5251
Didn't find 
9824
Didn't find 
10838
Didn't find 
16913
Didn't find 
11899
Didn't find 
17180
Didn't find 
4318
Didn't find 
1875
Didn't find 
8674
Didn't find 
19290
Didn't find 
10281
Didn't find 
13210
Didn't find 
18661
Didn't find 
16658
Didn't find 
16153
Didn't find 
6673
Didn't find 
13754
Didn't find 
10054
Didn't find 
10656
Didn't find 
7961
Didn't find 
15864
Didn't find 
13757
Didn't find 
16508
Didn't find 
14243
Didn't find 
4248
Didn't find 
7107
Didn't find 
18937
Found 
3793
Didn't find 
15090
Didn't find 
14393
Didn't find 
19610
Didn't find 
18172
Didn't find 
1734
Didn't find 
14073
Didn't find 
2160
Didn't find 
7905
Didn't find 
8261
Didn't find 
1984
Didn't find 
8626
Didn't find 
15975
Didn't find 
12846
Didn't find 
1057
Didn't find 
16782
Didn't find 
17944
Didn't find 
1076
Didn't find 
7531
Didn't find 
23
Didn't find 
6850
Didn't find 
10267
Didn't find 
1703
Didn't find 
18272
Didn't find 
8986
Didn't find 
1791
Didn't find 
18683
Didn't find 
13439
Found 
11488
Didn't find 
785
Didn't find 
8558
Didn't find 
6126
Found 
8904
Didn't find 
12456
Didn't find 
6919
Didn't find 
7390
Didn't find 
10557
Didn't find 
15339
Didn't find 
16551
Didn't find 
19662
Didn't find 
6078
Didn't find 
19488
Didn't find 
16053
Didn't find 
10193
Didn't find 
9387
Didn't find 
18470
Didn't find 
12816
Didn't find 
4249
Didn't find 
3729
Didn't find 
4154
Didn't find 
8600
Didn't find 
14586
Didn't find 
6763
Didn't find 
13889
Didn't find 
13410
Didn't find 
1774
Didn't find 
10952
Didn't find 
320
Didn't find 
4957
Didn't find 
5529
Didn't find 
19423
Didn't find 
14257
Didn't find 
19934
Didn't find 
11305
Didn't find 
14964
Didn't find 
6498
Didn't find 
11440
Found 
3293
Didn't find 
943
Didn't find 
15379
Didn't find 
19003
Didn't find 
8512
Didn't find 
4792
Didn't find 
10688
Didn't find 
7700
Didn't find 
1040
Didn't find 
14757
Didn't find 
10071
Didn't find 
7281
Didn't find 
7470
Didn't find 
16723
Didn't find 
13158
Didn't find 
9054
Didn't find 
9435
Didn't find 
10015
Didn't find 
2236
Didn't find 
15398
Didn't find 
16221
Didn't find 
7619
Didn't find 
19262
Didn't find 
1143
Didn't find 
6745
Didn't find 
17263
Didn't find 
3379
Didn't find 
467
Found 
14858
Didn't find 
5153
Didn't find 
12248
Didn't find 
15997
Didn't find 
9986
Didn't find 
3453
Didn't find 
12730
Didn't find 
8096
Didn't find 
8542
Didn't find 
18488
Didn't find 
102
Didn't find 
3487
Didn't find 
9261
Didn't find 
3381
Didn't find 
11434
Didn't find 
948
Didn't find 
9179
Didn't find 
9917
Didn't find 
18747
Didn't find 
3315
Didn't find 
8650
Didn't find 
11064
Didn't find 
14529
Didn't find 
2699
Didn't find 
11839
Didn't find 
16414
Didn't find 
10548
Didn't find 
3497
Found 
19308
Didn't find 
18564
Didn't find 
11408
Didn't find 
9208
Didn't find 
1296
Didn't find 
18775
Didn't find 
3469
Didn't find 
17182
Didn't find 
2066
Didn't find 
7043
Didn't find 
18927
Didn't find 
13088
Didn't find 
1255
Didn't find 
4020
Didn't find 
1432
Didn't find 
16229
Didn't find 
17820
Didn't find 
1368
Didn't find 
17964
Didn't find 
7935
Didn't find 
12855
Didn't find 
14090
Didn't find 
8815
Didn't find 
11948
Found 
3949
Didn't find 
3673
Didn't find 
11609
Didn't find 
16576
Didn't find 
12240
Didn't find 
4733
Didn't find 
11895
Didn't find 
19868
Didn't find 
988
Didn't find 
9437
Didn't find 
1277
Didn't find 
16777
Didn't find 
5298
Didn't find 
8308
Didn't find 
13071
Didn't find 
9836
Didn't find 
16158
Didn't find 
3970
Didn't find 
18826
Found 
683
Didn't find 
1165
Didn't find 
11615
Didn't find 
13131
Didn't find 
7087
Didn't find 
3153
Didn't find 
4438
Didn't find 
2553
Didn't find 
2365
Didn't find 
3572
Didn't find 
3176
Didn't find 
4466
Didn't find 
5024
Didn't find 
11788
Didn't find 
4796
Didn't find 
12408
Didn't find 
15742
Didn't find 
5508
Didn't find 
3082
Didn't find 
17748
Didn't find 
14458
Didn't find 
8840
Didn't find 
12499
Didn't find 
13302
Didn't find 
6783
Didn't find 
8178
Didn't find 
10632
Didn't find 
7910
Didn't find 
16593
Didn't find 
3229
Didn't find 
8095
Didn't find 
10689
Didn't find 
10271
Didn't find 
3913
Didn't find 
5843
Didn't find 
4571
Didn't find 
19959
Didn't find 
18900
Didn't find 
16219
Didn't find 
19733
Didn't find 
647
Didn't find 
19828
Didn't find 
13017
Didn't find 
10218
Didn't find 
9314
Didn't find 
4263
Didn't find 
14181
Didn't find 
6402
Didn't find 
16235
Didn't find 
8776
Didn't find 
11005
Didn't find 
403
Didn't find 
15066
Didn't find 
8567
Didn't find 
9129
Didn't find 
6926
Didn't find 
12080
Didn't find 
4196
Didn't find 
15716
Didn't find 
14252
Didn't find 
15201
Didn't find 
4288
Didn't find 
18383
Didn't find 
4980
Didn't find 
13667
Didn't find 
19766
Didn't find 
7701
Didn't find 
18635
Didn't find 
8536
Didn't find 
2406
Didn't find 
1200
Didn't find 
17394
Didn't find 
18283
Didn't find 
2628
Didn't find 
7020
Didn't find 
17958
Didn't find 
1360
Didn't find 
2287
Didn't find 
8839
Didn't find 
18989
Didn't find 
15403
Didn't find 
15987
Didn't find 
15493
Didn't find 
18482
Didn't find 
4942
Didn't find 
9118
Didn't find 
533
Didn't find 
226
Didn't find 
2506
Didn't find 
16655
Didn't find 
17760
Didn't find 
10082
Didn't find 
6885
Found 
15554
Didn't find 
1021
Didn't find 
11576
Didn't find 
11279
Didn't find 
16218
Didn't find 
18094
Didn't find 
17077
Didn't find 
16897
Didn't find 
9613
Didn't find 
3394
Didn't find 
17818
Didn't find 
12485
Didn't find 
8281
Didn't find 
6942
Didn't find 
1236
Didn't find 
19221
Didn't find 
17454
Didn't find 
17555
Didn't find 
8020
Didn't find 
14631
Didn't find 
2253
Didn't find 
12839
Found 
7673
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
