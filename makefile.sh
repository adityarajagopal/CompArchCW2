#!/bin/bash
INPUT='test/test.input'; 
OUTPUT='test/test.output'; 
PARAMS='test/test.params';
GOT='test/test.got'; 

g++ src/mem_sim*.cpp -o mem_sim
cat ${INPUT} | ./mem_sim $(cat ${PARAMS}) > ${GOT}

#remove empty lines 
sed -i.bak '/^$/d' ./${GOT} 
sed -i.bak '/^$/d' ./${OUTPUT} 
#remove trailing and leading white spaces 
sed -i -e 's/^[ \t]*//;s/[ \t]*$//' ./${GOT} 
sed -i -e 's/^[ \t]*//;s/[ \t]*$//' ./${OUTPUT} 
# remove comments
sed -i -e '/#.*/d' ./${GOT}

pr -m -t ${OUTPUT} ${GOT}

diff ${GOT} ${OUTPUT}