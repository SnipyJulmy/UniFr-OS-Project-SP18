#!/usr/bin/env bash

rm -f *.log
rm -f *.cmd

for i in {1..10}
do
    FILE="client_$i.cmd"
    touch ${FILE}

    # add 1000 element in the format $i_$j and value $j
    for j in {0..10}
    do
        KEY="$i$i$j"
        VALUE="$j"
        echo "add $KEY $VALUE" >> ${FILE}
    done

    for j in {0..10}
    do
        KEY="$i$i$j"
        VALUE="$j"
        echo "read_v $KEY" >> ${FILE}
    done

    for j in {0..10}
    do
        KEY="$i$i$j"
        VALUE="$j"
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
for i in {1..10}
do
    FILE="client_$i.cmd"
    LOG="client_$i.log"
    echo "../../cmake-build-debug/src/client/client -f ./$FILE -l ./$LOG 127.0.0.1 &" >> ${RUN_SCRIPT}
done

chmod +x ${RUN_SCRIPT}