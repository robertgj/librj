#!/bin/sh
#
prog="swTree_lg_test"
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
# the output should look like this
#
cat > test.ok << 'EOF'
n    floor(lg(n+1)) 2^floor(lg(n+1)) 2^ceil(lg(n+1))
   0    0              1                1
   1    1              2                2
   2    1              2                4
   3    2              4                4
   4    2              4                8
   5    2              4                8
   6    2              4                8
   7    3              8                8
   8    3              8               16
   9    3              8               16
  10    3              8               16
  11    3              8               16
  12    3              8               16
  13    3              8               16
  14    3              8               16
  15    4             16               16
  16    4             16               32
  17    4             16               32
  18    4             16               32
  19    4             16               32
  20    4             16               32
  21    4             16               32
  22    4             16               32
  23    4             16               32
  24    4             16               32
  25    4             16               32
  26    4             16               32
  27    4             16               32
  28    4             16               32
  29    4             16               32
  30    4             16               32
  31    5             32               32
  32    5             32               64
  33    5             32               64
  34    5             32               64
  35    5             32               64
  36    5             32               64
  37    5             32               64
  38    5             32               64
  39    5             32               64
  40    5             32               64
  41    5             32               64
  42    5             32               64
  43    5             32               64
  44    5             32               64
  45    5             32               64
  46    5             32               64
  47    5             32               64
  48    5             32               64
  49    5             32               64
  50    5             32               64
  51    5             32               64
  52    5             32               64
  53    5             32               64
  54    5             32               64
  55    5             32               64
  56    5             32               64
  57    5             32               64
  58    5             32               64
  59    5             32               64
  60    5             32               64
  61    5             32               64
  62    5             32               64
  63    6             64               64
  64    6             64              128
  65    6             64              128
  66    6             64              128
  67    6             64              128
  68    6             64              128
  69    6             64              128
  70    6             64              128
  71    6             64              128
  72    6             64              128
  73    6             64              128
  74    6             64              128
  75    6             64              128
  76    6             64              128
  77    6             64              128
  78    6             64              128
  79    6             64              128
  80    6             64              128
  81    6             64              128
  82    6             64              128
  83    6             64              128
  84    6             64              128
  85    6             64              128
  86    6             64              128
  87    6             64              128
  88    6             64              128
  89    6             64              128
  90    6             64              128
  91    6             64              128
  92    6             64              128
  93    6             64              128
  94    6             64              128
  95    6             64              128
  96    6             64              128
  97    6             64              128
  98    6             64              128
  99    6             64              128
EOF
if [ $? -ne 0 ]; then echo "Failed output cat"; fail; fi

#
# run and see if the results match
#
$VALGRIND_CMD $bin/$prog >test.out
if [ $? -ne 0 ]; then echo "Failed running $prog"; fail; fi

diff test.ok test.out
if [ $? -ne 0 ]; then echo "Failed diff"; fail; fi


#
# this much worked
#
pass
