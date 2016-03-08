#!/bin/bash
if [ ${1} = 0 ]
  then
    echo "Not a quadratic equation."
    exit 0
fi

D=$(((${2})*(${2})-4*(${1})*(${3})))

if [ $D = 0 ]
 then
    echo -n "x = "
    echo "scale=2;-0.5*(${2})/(${1})" | bc
    exit 0
fi
if [ $D -gt 0 ]
 then
   echo -n "x1 = "
   echo  "scale=2;0.5*(-(${2})+sqrt($D))/(${1})" | bc
   echo -n "x2 = "
   echo  "scale=2;0.5*(-(${2})-sqrt($D))/(${1})" | bc
 else
    x1=$(echo "scale=2;-0.5*(${2})/(${1})" | bc)
    x11=$(echo "scale=2;0.5*sqrt(-($D))/(${1})" | bc)
    x2=$(echo "scale=2;-0.5*(${2})/(${1})" | bc)
    x22=$(echo "scale=2;-0.5*sqrt(-($D))/(${1})" | bc)
    echo  "x1 = "$x1" + "$x11"i"
    echo  "x1 = "$x2" + "$x22"i"
fi

