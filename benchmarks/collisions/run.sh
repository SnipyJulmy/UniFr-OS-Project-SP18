#!/usr/bin/env bash

# start server
# ./cmake-build-debug/src/server/server &

# sleep 2s

#start clients

../../cmake-build-debug/src/client/client -f ./client1.cmd -l ./client1.log 127.0.0.1 &
../../cmake-build-debug/src/client/client -f ./client2.cmd -l ./client2.log 127.0.0.1 &
../../cmake-build-debug/src/client/client -f ./client3.cmd -l ./client3.log 127.0.0.1 &
../../cmake-build-debug/src/client/client -f ./client4.cmd -l ./client4.log 127.0.0.1 &
../../cmake-build-debug/src/client/client -f ./client5.cmd -l ./client5.log 127.0.0.1 &
../../cmake-build-debug/src/client/client -f ./client6.cmd -l ./client6.log 127.0.0.1 &
../../cmake-build-debug/src/client/client -f ./client7.cmd -l ./client7.log 127.0.0.1 &
../../cmake-build-debug/src/client/client -f ./client8.cmd -l ./client8.log 127.0.0.1 &
../../cmake-build-debug/src/client/client -f ./client9.cmd -l ./client9.log 127.0.0.1 &
../../cmake-build-debug/src/client/client -f ./client10.cmd -l ./client10.log 127.0.0.1 &
../../cmake-build-debug/src/client/client -f ./client11.cmd -l ./client11.log 127.0.0.1 &
