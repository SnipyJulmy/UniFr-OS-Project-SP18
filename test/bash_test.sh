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
    echo "add $i $(((RANDOM % 100) + 1 ))">> list-cmd.txt
done
echo "ls" >> list-cmd.txt
echo "exit" >> list-cmd.txt
start=$(date +'%s%3N')
./client 127.0.0.1  < list-cmd.txt
end=$(date +'%s%3N')
runtime=$((end-start))
echo $runtime
