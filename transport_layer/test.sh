#!/bin/sh

make
echo "Starting Test Execution - $1 runs"
for ((i=0; i < $1; ++i)) do
    echo "run $i"
    cd bin
    time ./transport
    cd ../
    total=0
    num=0
    while read -r line; do
	total=$(bc <<< "$total+${line:6}")
	let num+=1
    done <<< "$(tail -n 10000 results.log | grep Cost)"
    avg=$(bc <<< "$total/$num")
    echo "$avg"
    echo "end run $i"
done

