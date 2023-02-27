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

x=1000;
s=create();
for (i=0;i<x;i=i+1;)
{
  "Inserting: " ; print i; p=insert(s,i);
  if (p == 0)
  {
     "ERROR: insert() failed for "; print i; i=x;
  }
  else
  {
    y=rand(x); 
    "Removing: ";print y; z=remove(s,&y);
    if (z == 0)
    {
      "Did not remove "; print y;
    }
    z=find(s,y);
    if (z != 0)     
    {
      "Found "; print y;
    }
  }
}
"Walk: "; walk(s,show);
"Size: "; print size(s);
"Destroy";
destroy(s);
"Exit";
exit;
EOF
if [ $? -ne 0 ]; then echo "Failed input cat"; fail; fi

#
# the output should look like this
#
cat > test.ok.gz.uue << 'EOF'
begin-base64 644 test.ok.gz
H4sICHlx5GMAA3Rlc3Qub3V0AJWdu64tyXFE/fsV/IRd7yralEFXMmQL0EAY
SCIBaiBI+np1DJ0bqwI4fay5032qdz2zMiMjM//8l//65W+//fqXf/vjH358
fvzjL//51//++/+Mz+fHn3791z/85a+//eFvevzL35/9+acG5ecG++yrgZ79
3KD+3KDdf9/8z9vPf17buv5ez35u0H9uMM/198+jn/98/PznvZXr7/Xs5wbz
5wartquBnv3cYHmDERoMa7CtwbmHoGc/Nzg2R+Meg57Zotky71XvVXueWQtb
5/a5O6Vn1sIXut690jNrYWvd2t0rPbMWtthr9HuqnmfWwta7hHEUjsMWfIZx
TI7DVryXe8X1zFrYktcSdnnxXV5szdsK52jh3H18du+50jNrUb478lq/e5qq
rfn0d7a65dzCSM+sxXBpFISR/72v7bnnRM+sha1trXcLPbMWtrZzh1ncmBNb
27XvceuZCUkX2/s+OXpmLXxt9wy98tVotrYnrMbBajRf2zCOyXHYivcTTs7x
k9NsxUfY1QO7uk3/jfvkdNxYbbkkC3KpYnZtzc/nPs96Zi1szc+8e6Vnds/5
eQ6bvWG3d1/zfs+unlkLW/Nd73HombVovh73OAYu+O4yvIULtfmF2m3NT5hc
9MlWvIa9XrHXu6/4vHeunlkLl+ChU5W9shXv/d65emaqiq/4vNdPz6yFa2fh
mvd9O2y917kb6Jm1aC5x7z5tnNhh672Dwuj7Y/hqj3uf65m1cBVthh01fUeN
5aO4d4ieWQs/4SNswuHrPVxLq/ee0jPTNH29ezjh3Uc+bb17kIYd0nDaipeg
QRVoUNNWvNR7rvTMWvTvSpFpa17nvQ/1zFr4TR5GPjlyX/MVdu7ynTuhmAdN
Hnt92pqXFuaq+Vytj+/d+3gs3APL1nz0e7frmbXwm7zcLfTMWrhUDy0GW9ia
jyBzB2TuckvsE/bux1dwTR/5va/0zFr4mgdLacNSWrbmZd9rrmfWwte8B621
+znfvubB6F4wunfxXt0j1zNr4ec8GJUFRuV27S3oexP63u5+aoMdOn129/Df
CBoJTtR27S1caR132l4ur/ylLe+YYZtO36bblnf7rj+2kv3c50TPrEX57q1w
vm1Xn2/f0cdBlE9Ylw9auF0dRl44cl/JcK933OvHtbIS7sOCFttlyj27HSfr
+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
zZKDx23sjeRNx+1ploQ4bk9vUIQO/M7cHogMf0HZO25Pd95psJ1DclG6bA48
z9cEuH5a2RqUJXQXKeYmXyN7RbjUYf6fDa4lvwiMHKuNpP1sjMBLyAI3gg+3
qRu8Jzh8Dk0ReJqp39HTjI2PQPBAsSNac9zgDbUyeHrgaUbc8XFzFyDhQYq4
6zWcxS/0qYNIiQDgUJrB2n1RhvSw8iAn0ZHTp4///C//8e/PPz8/yo/6Y/04
Px4j61FYH3Xq2UOPuvNcG8+1/xyO56p+Ls9nVz6X23MbPffDI++fDzxC8xGC
Tz+eA/wcyOfYPDv1Waxn+p8pfibqGfjTq/JoX+UxfsujNZTn7JfH3CyPBVke
O7A8llt5bK3ymE/lmYRS9PzpynNky6Oal6dD5TlN5dFJy6N4lkfPKl3/fb7x
9Ks8ekJ5Olee3pXnci1P/8rTwfL0sDxXQnk6WJ4DXB6JWR6xWB7ZVx4JVh4R
UJ4Ol+folafX5TlF5dnY5dm95dmi5dkF5Zmy+vS9PlNZn/7XR0epT//rcxPU
Zwz1GUN9+l+fE1Cf/ten//UxWOqzHWudv9e4V415FXFXVXZVRVdVchUXV21v
1clW4WuVpVbRaFVOVpFilSJW+WAV91VtXVW+VSlb1aZVoVfVRVVFUdX7VO1M
FcNU3UkVklRpRBUvVO1AFQNUET2VylOJOtWcU2E5VY9TjTgVfVNlNxVjU6k0
VTxTWTMVHVMdMFXuUk0rVZ1SGSkVflItJhVXUrUklT9SwSLVC1JRIFXWUWEb
FYlRRRaVWFFRFFU+UXkTFR9RNREV+FDFDhXHUOkK1aJQpQiVIlChAKXxV5J9
JctX9nuluFcee+WEV8Z2pWVXInWlPlcucyUnVwZypRRXjnDl4FZSbWXO7r/v
4/J7AmtlpFaKaSWAVhpn5WVWomVlTlYqZGU0VopiZRpW6mAl+1X+XSXZVdZc
5bRVSlnljVUiWKVpVRpVJT5VJlOlJlVmUKX6VO5OJehUFk5luVSuSiWfVLpI
5X9UkkdlbVQGRGUoVFJBpQFUXj8l6lM6PuXcUxI9ZcpTOjwltVOWOqWiU245
pXFTsjalXFMONaU9U1Iy5QRTki9l7VJWLOWxUrYqJYtSgidlbFI+JWVFUs4i
pQxSYiBl/1G+HqXLUYoa5ZxRphfla1ECFmVZUSoV5UtRUhRlIFEiEWULUUoQ
ZeRQDg1luFAaC+WlUKIJZY5QrgcldFDWBqVmUK4FJVRQlgOlMlBCAmUYUBoB
Bfkral9h+Aq2V3y8At4Vw66oc4WWK/Bb0d0K11b8tYKsFRatQGZFKyskWYHH
Ch9WjLACgRXZq8BaRcoqHlZBr4piVdypgksVLar4TgVsKsRScZQKjJy/y7n5
ewCiIgoVIqigPkXpKRRPgXKKhlPIm2LYFImm0DLFiin4S9FcCtlSXJbCohTn
pOAjRRgpZEhxQQr0UTSPYm0UCaNwF0WtKDRFwSSK5VDEhuIpFDShKAjFLSiq
QKEDYvOLsi9evsj3Ir+Lmi7+uQjlYo2LGi7+txjb4lOLNS1qtLjOYjSLtiwe
sojFYg+LDizOr4i9Yu+Koisersi2Ys+KIyvSq2iq4qKKcCpWqTifInGKlSnq
pfiVYj+K0CjeooiIog6K7SdKnzh6IuKJbSeGnGhwIruJ0ibemshpYpuJQCZG
mChe4myJmCV6lThU4jiJtCQWkshCYgSJ9iNujwg84uGIbCP2jCgy4sGI7CL2
ijgqYqKIWiL+iEgionWIuyGChlgYolqITyGChBgPojWIuyCCgigF4g3I0y/H
urzncpHLDy7HtrzXckfL5ywnsrzCcvPKbytHrDyrcp/KHyoXpXyOcizKUyh3
oHx+cuzJ8Sbvmtxl8onJySVPltxV8j/JiyRXkfw/cujIQyM3jDwr8oXIuSEP
hnwOciLIKyDoX/i+AHtB7ALShZYL3xZgLVha2LMAZqHIgoqF/griFY4rYFbo
q+BUYaaCQYVrCqgUGinIURiiQEEhf4L3BOIJqRP0JnxNIJqQMmFbgquEPwlk
EpIkuEiYkEAeoTaCYYS1CFARbCJsRGiGAAqhEIIVhB0IDJDFLxNeNrkMb1nS
Mo1l/8rIldUqM1SWpcxH2YgyBGXZyXyTjSZDTNaWTCrZTTKOZAHJpJHdIkNE
1oZMCtkNMg6k7Uull94u7VwquPRsKc7SjqUCS6eVmipdVBqn1ErpiVL8pMlJ
XfunX//vlz/+PXXHn375r9/+9tf//fEP//Prbz/+H1LirGCLsgAA
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
