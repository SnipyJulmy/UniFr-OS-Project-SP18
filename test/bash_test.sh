#!/bin/bash
dirname $0
cd $(dirname $0)
pwd
cd ..
cd ./cmake-build-debug/src/client
rm list-cmd.txt
for i in {1..500}
do
    echo "delete $i">> list-cmd.txt
    echo "add $i">> list-cmd.txt
done
echo "ls" >> list-cmd.txt

for j in {1..10}
do
    start=$(date +'%s%3N')
    ./client 127.0.0.1  < list-cmd.txt
    end=$(date +'%s%3N')
    runtime[j]=$((end-start))
done
echo $runtime
