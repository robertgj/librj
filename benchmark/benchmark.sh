#!/bin/bash

# Notes:
#   1. Run this script with $1 absent or non-zero to insert random keys or
#      $1 as 0 to insert sorted keys.
#   2. With the current values of depth_factor (in swTree_private.h) and
#      sgTree_alpha (in sgTree_private.h) the run-times when inserting
#      sorted keys are O(N^2) rather than O(NlgN).

# Disable CPU frequency scaling:
 for c in `seq 0 7` ; do
   echo "4500000">/sys/devices/system/cpu/cpu$c/cpufreq/scaling_min_freq ;
   echo "performance">/sys/devices/system/cpu/cpu$c/cpufreq/scaling_governor ;
 done ;

# Show system information
lscpu
ulimit -s unlimited # For recursive insert or remove functions
ulimit -l -s -m

# Make a program to invalidate the CPU caches
let n=200000
echo "#include <stdint.h>" > cache_thrash.c
echo "#define SIZE_D "$n >> cache_thrash.c
echo "uint64_t d[SIZE_D+1];" >> cache_thrash.c
echo "int main(void)" >> cache_thrash.c
echo "{" >> cache_thrash.c
echo "uint64_t i,x;" >> cache_thrash.c
let k=0;
while ((k <= $n)); do
  echo "i="$k";x=i;d[i]=x;x=d[i]*2;" >> cache_thrash.c
  let k=k+1;
done
echo "return 0;" >> cache_thrash.c
echo "}" >> cache_thrash.c

gcc -O0 -Wall -o cache_thrash cache_thrash.c

# Save the benchmark template. Use "y=x;" to inserted sorted keys.
cat > test.in.template << 'EOF'
k=KKKK;
"k="; print k;
r=10;
u=0;
for (s=0;s<=r;s = s+1;)
{
  t=time(0);

  h=create();
  for(x=0; x<k; x=x+1;)
  {
    ZZZZ;
    insert(h,y);
  }
  for(x=0; x<k; x=x+1;)
  {
    y=rand(k);
    remove(h,&y);
  }
  destroy(h);

  t=time(t);

  if (s != 0)
  {
    u=u+t;
  }
} 
u = u/(r*1000);
"Average elapsed time (us):"; 
uprint u;
EOF

# Non-zero for inserting random keys, zero for sorted keys
if test $# -ne 0; then
    let use_random=$1;
else
    let use_random=1;
fi;
if [[ $use_random -ne 0 ]] ; then
    let use_random=1;
    let maxm=100000;
    sed -i -e "s/ZZZZ/y=rand\(k\)/" test.in.template ;
    echo "Using random keys";
else
    let use_random=0;
    let maxm=10000;
    sed -i -e "s/ZZZZ/y=x/" test.in.template ;
    echo "Using sorted keys";
fi;

for i in bsTree redblackTree sgTree skipList splayTree swTree ; do
    echo $i
    strip bin/$i"_interp"
    ls -l bin/$i"_interp"
    let m=10;
    while ((m <= maxm)); do
        let m=m*10;
        for l in 1 2 5 ; do
            let k=l*m;          
            # Set up the test input
            echo $k ;
            cat test.in.template | sed -e "s/KKKK/$k/" > test.in

            # Run the test
            bin/$i"_interp" < test.in \
                     > "benchmark_"$i"_"$k"_"$use_random 2>&1
            
            valgrind --leak-check=full bin/$i"_interp" < test.in \
                     > "memcheck_"$i"_"$k"_"$use_random 2>&1

            # Thrash the CPU caches
            ./cache_thrash

            # Check cache performance
            valgrind --tool=cachegrind bin/$i"_interp" < test.in \
                     > "cachegrind_"$i"_"$k"_"$use_random 2>&1
        done;
    done;
done;
