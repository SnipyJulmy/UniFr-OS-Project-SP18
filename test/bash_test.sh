#!/bin/bash
dirname $0
cd $(dirname $0)
pwd
cd ..
cd ./cmake-build-debug/src/client

start=$(date +'%s%3N')
./client 127.0.0.1  < list-cmd.txt

end=$(date +'%s%3N')
runtime=$((end-start))
echo $runtime
