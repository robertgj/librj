#!/bin/bash

# Run this script with $1 absent or non-zero to plot results for the
# inserted random keys or $1 as 0 to plot results for inserted sorted keys.

# Make plots with octave
cat > benchmark_plot.m << 'EOF'
graphics_toolkit("qt");
set(0,"defaultaxestitlefontweight","normal");
set(0,"DefaultTextFontName", "Monospace Regular");

use_random=ZZZZ;
if use_random
    fsuf="_random";
else
    fsuf="_sorted";
endif

load extime.data
loglog(extime(:,1),extime(:,2:end)./extime(:,1))
legend("bsTree","redblackTree","sgTree","skipList","splayTree","swTree");
if use_random
  lstr="southeast";
else
  lstr="northwest";
  axis([100 1e5 1e-1 1e1]);
endif
legend("box","off","location",lstr,"fontname","Monospace Regular");
xlabel("Number of keys inserted(N)");
ylabel("Average execution time(us)/N");
title("Average execution time(us) normalised to number of keys");
fstr=strcat("extime",fsuf);
print(fstr,"-dsvg");
print(fstr,"-depsc2");

load alloc.data
loglog(alloc(:,1),alloc(:,2:end)./(11*alloc(:,1)))
legend("bsTree","redblackTree","sgTree","skipList","splayTree","swTree");
if use_random
   lstr="northeast";
else
   lstr="southeast";
   axis([100 1e5 1e1 1e2]);
endif
legend("box","off","location",lstr,"fontname","Monospace Regular");
xlabel("Number of keys inserted(N)");
ylabel("Allocated memory(bytes)/N");
title("Allocated memory(bytes) normalised to number of keys");
fstr=strcat("alloc",fsuf);
print(fstr,"-dsvg");
print(fstr,"-depsc2");

load Iref.data
loglog(Iref(:,1),Iref(:,2:end)./(11*Iref(:,1)))
legend("bsTree","redblackTree","sgTree","skipList","splayTree","swTree");
if use_random
   lstr="southeast";
else
   lstr="northwest";
   axis([100 1e5 1e3 1e5]);
endif
legend("box","off","location",lstr,"fontname","Monospace Regular");
xlabel("Number of keys inserted(N)");
ylabel("Instructions/N");
title("Instructions normalised to number of keys");
fstr=strcat("Iref",fsuf);
print(fstr,"-dsvg");
print(fstr,"-depsc2");

load Drd.data
loglog(Drd(:,1),Drd(:,2:end)./(11*Drd(:,1)))
legend("bsTree","redblackTree","sgTree","skipList","splayTree","swTree");
if use_random
   lstr="southeast";
else
   lstr="northwest";
   axis([100 1e5 1e2 1e4]);
endif
legend("box","off","location",lstr,"fontname","Monospace Regular");
xlabel("Number of keys inserted(N)");
ylabel("Data reads/N");
title("Data reads normalised to number of keys");
fstr=strcat("Drd",fsuf);
print(fstr,"-dsvg");
print(fstr,"-depsc2");

load Dwr.data
loglog(Dwr(:,1),Dwr(:,2:end)./(11*Dwr(:,1)))
legend("bsTree","redblackTree","sgTree","skipList","splayTree","swTree");
if use_random
   lstr="northwest";
else
   lstr="northwest";
   axis([100 1e5 1e2 1e4]);
endif
legend("box","off","location",lstr,"fontname","Monospace Regular");
xlabel("Number of keys inserted(N)");
ylabel("Data writes/N");
title("Data writes normalised to number of keys");
fstr=strcat("Dwr",fsuf);
print(fstr,"-dsvg");
print(fstr,"-depsc2");
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
    sed -i -e "s/ZZZZ/true/" benchmark_plot.m
    echo -n "Using random keys";
else
    let use_random=0;
    let maxm=10000;
    sed -i -e "s/ZZZZ/false/" benchmark_plot.m
    echo -n "Using sorted keys";
fi;

# Extract results
rm -f *.data
for i in bsTree redblackTree sgTree skipList splayTree swTree ; do

  echo $i
  let m=10;
  while ((m <= maxm)); do
      let m=m*10;
      for l in 1 2 5 ; do
          let k=l*m;          

          echo -n $k" " >> $i"_extime.data"
          cat "benchmark_"$i"_"$k"_"$use_random | tail -1 >> $i"_extime.data"

          echo -n $k" " >> $i"_alloc.data"
          cat "memcheck_"$i"_"$k"_"$use_random | \
              awk '/usage/ {print $9;}' | tr -d ',' >> $i"_alloc.data"

          echo -n $k" " >> $i"_Iref.data"
          cat cachegrind_$i"_"$k"_"$use_random | \
              awk '/I[ ]*refs/ {print $4;}' | tr -d ',' >> $i"_Iref.data"
          
          echo -n $k" " >> $i"_Drd.data"
          cat cachegrind_$i"_"$k"_"$use_random \
              | awk '/D[ ]*refs/ {print $5 " ";}' \
              | tr -d ',' | tr -d '(' >> $i"_Drd.data"

          echo -n $k" " >> $i"_Dwr.data"
          cat cachegrind_$i"_"$k"_"$use_random | \
              awk '/D[ ]*refs/ {print $8 " ";}' \
              | tr -d ',' | tr -d ')' >> $i"_Dwr.data"
      done
  done
done

for i in extime.data alloc.data Iref.data Drd.data Dwr.data ; do
    cp bsTree_$i $i;
    for k in redblackTree sgTree skipList splayTree swTree ; do
        join $i $k"_"$i > $i".tmp" ;
        mv $i".tmp" $i;
    done
done

# Make plots with octave
octave --no-gui benchmark_plot.m
