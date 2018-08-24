#!/bin/bash

if [ "$#" -lt 4 ]
then
    echo "<ERR>: "$0" <executable> <delimeter> <iterations> <output> <recreate>!"
    exit 1
fi

executable="$1"; delimeter="$2"; iterations="$3"; output="$4"

if [ ! -x "$executable" ]
then
    echo "<ERR>: "$executable" is not an executable file!"
    exit 2
fi

if [ "$#" -ge 5 ]
then
    touch "$output"; rm "$output"; touch "$output"

    for ((i = 0; i < "$iterations"; i++))
    do
        "$executable" >> "$output"
    done
fi

if [ ! -w "$output" ]
then
    echo "<ERR>: "$output" is not a writeable file!"
    exit 3
fi

if ! [[ "$iterations" =~ ^[0-9]+$ ]] || [ "$iterations" -le 0 ]
then
    echo "<ERR>: The number of iterations must be a positive integer!"
    exit 4
fi

total=0
count=0
ratios=$(grep "$delimeter" "$output" | cut -f2 -d ":")

if [ -z "$ratios" ]
then
    echo "<ERR>: "$delimeter" does not appear anywhere inside "$output"!"
    exit 4
fi

for ratio in $ratios
do
    total=$(echo $total + $ratio | bc); ((count++));
done

echo -n "Average of \"${delimeter}\" flagged values read from ${output}: "
echo "scale = 2; $total / $count" | bc
