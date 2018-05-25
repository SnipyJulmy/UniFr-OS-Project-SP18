#!/usr/bin/env bash

# clear previous file
rm -f *.log
rm -f *.cmd

NB_CLIENT=32
NB_VALUE=100

for (( i=1; i<(($NB_CLIENT + 1));i++ ))
do
    FILE="client_$i.cmd"
    touch ${FILE}

    for (( j=100; j<(($NB_VALUE + 100));j++ ))
    do
        KEY="$i$j"
        VALUE="$j"
        echo "add $KEY $VALUE" >> ${FILE}
    done
    for (( j=100; j<(($NB_VALUE + 100));j++ ))
    do
        KEY="$i$j"
        echo "read_v $KEY" >> ${FILE}
    done
    for (( j=100; j<(($NB_VALUE + 100));j++ ))
    do
        KEY="$i$j"
        echo "rm_k $KEY" >> ${FILE}
    done
done

#genereate the running script
RUN_SCRIPT="run.sh"

if [[ -e ${RUN_SCRIPT} ]]
then
    rm -f ${RUN_SCRIPT}
fi

touch ${RUN_SCRIPT}
echo '#!/usr/bin/env bash' >> ${RUN_SCRIPT}
for (( i=1; i<(($NB_CLIENT + 1));i++ ))
do
    FILE="client_$i.cmd"
    LOG="client_$i.log"
    LOG_PROC="client_${i}_proc.log"
    LOG_ERR="client_${i}_err.log"
    echo "../../cmake-build-debug/src/client/client -f ./$FILE -l ./$LOG 127.0.0.1 > $LOG_PROC 2> $LOG_ERR &" >> ${RUN_SCRIPT}
done

echo "wait" >> ${RUN_SCRIPT}
echo "echo \"end of simulation\"" >> ${RUN_SCRIPT}

chmod +x ${RUN_SCRIPT}