#! /bin/bash
scriptdir="$(cd "$(dirname "$0")" && pwd -P)"
cd $scriptdir
cd collisions/
echo "Number of error over 11 clients and 28 commands (3 ls, 3 add, 9 read_v, 7 read_k, 3 update, 1 rm_v, 2 rm_k :" $(grep -oi err *.log | wc -l)
echo "Number of Add error :" $(grep -oi "Unable to add" *.log | wc -l)
echo "Number of Read error :" $(grep -oi "Unable to read" *.log | wc -l)
echo "Number of Delete error :" $(grep -oi "Unable to delete" *.log | wc -l)
cd ../no_collisions/
echo "Number of error over 8 clients and 2700 commands ( 900 add, 900 read_v, 900 rm_k :" $(grep -oi err *.log | wc -l)
echo "Number of Add error :" $(grep -oi "Unable to add" *.log | wc -l)
echo "Number of Read error :" $(grep -oi "Unable to read" *.log | wc -l)
echo "Number of Delete error :" $(grep -oi "Unable to delete" *.log | wc -l)
cd ../many_clients/
echo "Number of error over 32 clients and 300 commands (100 add, 100 read_v, 100 rm_k :" $(grep -oi err *.log | wc -l)
echo "Number of Add error :" $(grep -oi "Unable to add" *.log | wc -l)
echo "Number of Read error :" $(grep -oi "Unable to read" *.log | wc -l)
echo "Number of Delete error :" $(grep -oi "Unable to delete" *.log | wc -l)