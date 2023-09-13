#!/bin/bash
for ((num = 2; num <= 2; num++))
do
    cp cutCheck.cpp ./copydatacutFiles/cutCheck$num.cpp
    sed -i "s|.*int fileNumber = .*| int fileNumber = $num; |g" ./copydatacutFiles/cutCheck$num.cpp
    sed -i "s|.*void cutCheck().*| void cutCheck$num() |g" ./copydatacutFiles/cutCheck$num.cpp
    bsub root ./copydatacutFiles/cutCheck$num.cpp
done