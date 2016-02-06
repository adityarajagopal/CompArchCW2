#!/bin/bash
TEST_NUM=
TEST_FILE=
INPUT='test.input'; 
PARAMS='test.params';
OUTPUT='test.output'; 
GOT='test.got';

TEST_FILE="$1"; 
shift
TEST_NUM="$1"; 

sed "/${TEST_NUM})/,/\/\/\*/!d
	/INPUT/,/PARAMS/!d
	/INPUT/d
	/PARAMS/d
	/^$/d" ./${TEST_FILE} > ${INPUT}

sed "/${TEST_NUM})/,/\/\/\*/!d
	/PARAMS/,/OUTPUT/!d
	/PARAMS/d
	/OUTPUT/d
	/^$/d" ./${TEST_FILE} > ${PARAMS}

sed "/${TEST_NUM})/,/\/\/\*/!d
	/OUTPUT/,/\/\/\*/!d
	/OUTPUT/d
	/\//d
	/^$/d" ./${TEST_FILE} > ${OUTPUT}

g++ src/mem_sim*.cpp -o mem_sim
cat ${INPUT} | ./mem_sim $(cat ${PARAMS}) > ${GOT}

diff -w -B ${GOT} ${OUTPUT}