#!/bin/bash

for ((num = 1; num <=5 ; num++))

do 
    mkdir ../../RawFile/cosmicSlab$num
    cp ../runMac/specmuonFullPropagated_no_vis.mac ../../macroCP/specmuonFullPropagated_no_vis$num.mac
    sed -i "s+/run/fname+/run/fname /net/cms18/cms18r0/cms26r0/zheng/slabsim/withoutPhoton/RawFile/cosmicSlab$num/+g" ../../macroCP/specmuonFullPropagated_no_vis$num.mac
    bsub ./MilliQan ../../macroCP/specmuonFullPropagated_no_vis$num.mac 
done
