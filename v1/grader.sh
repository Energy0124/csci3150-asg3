#!/bin/bash
#################################
# CSCI3150 Multi-threading grader
# Usage:
# ./grader.sh help
#
#################################

INPUT_DIR=./testcases
OUTPUT_DIR=./res

BD="\033[1m"   # bold
BDED="\033[21m"
RED="\033[31m" #red
GREEN="\033[92m" #green
YELLOW="\e[33m" #yellow
ED="\e[0m"

DIFF_FLAG="-Z -b -B"
#################################
# main
function main {

    # clean the output directory
#    rm -rf ${OUTPUT_DIR}/*

    arg1=$1
    if [ "$arg1" == "help" ]
    then
        helpmessage
    elif [ "$arg1" == "PartA" ]
    then
        EXCUTABLE=$2
        TESTCASE=$3
        parta $EXCUTABLE $TESTCASE
    elif [ "$arg1" == "PartB" ]
    then
        EXCUTABLE=$2
        partb $EXCUTABLE $3 $4
    else
        helpmessage
    fi
}

function helpmessage {
    echo -e "${BD}${GREEN}Usage:${ED}"
    echo -e "${BD}${GREEN}  ./grader.sh help${ED}  -- print out this help message"
    echo -e "${BD}${GREEN}  ./grader.sh PartA [executable] [testcase]${ED}  -- run [testcase] using [executable] with 4 threads, output is in ${GREEN}./res${ED}"
    echo "      eg. : ./grader.sh PartA asg3-pthread C0"
    echo "      eg. : ./grader.sh PartA asg3-openmp C1"  
    echo -e "${BD}${GREEN}  ./grader.sh PartA [executable]${ED} -- show you the mark you get for [executable] after test all testcases"
    echo "      eg. : ./grader.sh PartA asg3-pthread"
    echo "      eg. : ./grader.sh PartA asg3-openmp"
    echo -e "${BD}${GREEN}  ./grader.sh PartB [executable] scaleout${ED} -- tell you whether your [executable] can scale-out"
    echo -e "${BD}${GREEN}  ./grader.sh PartB [executable] [testcase] [ThreadNum]${ED} -- run testcase using [executable], tell you the running time"
    echo "      eg. : ./grader.sh PartB asg3-pthread S1 2"
    echo "      eg. : ./grader.sh PartB asg3-openmp S2 4"
}

function parta {
    EXCUTABLE=$1

    # check EXCUTABLE 
    if [ ! -f ${EXCUTABLE} ]
    then
        echo -e "${BD}${RED}Cannot find ${EXCUTABLE} !${ED}"
        exit -1
    fi

    if [ -n "$2" ]
    then
        TESTCASE=$2
        if [ ! -d ${INPUT_DIR}/${TESTCASE} ]; then
            echo -e "${BD}${RED}Testcase ${INPUT_DIR}/${TESTCASE} not exist!${ED}"
            exit -1
        fi

        echo ""
        echo -e "${BD}${YELLOW}Checking $EXCUTABLE Correctness of Testcase $TESTCASE ${ED}"
        echo ""

#        cat ${INPUT_DIR}/${TESTCASE}/README
#        echo ""
        
        # excute
        ./$EXCUTABLE ${INPUT_DIR}/${TESTCASE}/input1.txt ${INPUT_DIR}/${TESTCASE}/input2.txt ${OUTPUT_DIR}/output${TESTCASE}.txt 4

        # diff
        if diff ${OUTPUT_DIR}/output${TESTCASE}.txt ${INPUT_DIR}/${TESTCASE}/expected.txt >/dev/null 2>&1
        then
            echo -e "${BD}${GREEN}Testcase $TESTCASE Passed!${ED}"
        else
            echo -e "${BD}${RED}""Failed Testcase $TESTCASE! ${ED}"
            echo -e "${RED}unmatched:     yours             vs          expected${ED}"
            echo -e "${RED}               ${OUTPUT_DIR}/output${TESTCASE}.txt           ${INPUT_DIR}/${TESTCASE}/expected.txt${ED}"
#            echo -e "${RED}right res:${INPUT_DIR}/${TESTCASE}/output.txt${ED}" 
#            echo -e "${RED}your res:./res/output${TESTCASE}.txt${ED}"
#           diff -y ${diff_flags} ${OUTPUT_DIR}/output.txt ${INPUT_DIR}/${TESTCASE}/output.txt;
        fi
    else
        total=12
        pass=0
        for i in $(seq 1 12)
        do
#            rm -rf ${OUTPUT_DIR}/*
            TESTCASE=C${i}
            ./$EXCUTABLE ${INPUT_DIR}/${TESTCASE}/input1.txt ${INPUT_DIR}/${TESTCASE}/input2.txt ${OUTPUT_DIR}/output${TESTCASE}.txt 4

            if diff ${OUTPUT_DIR}/output${TESTCASE}.txt ${INPUT_DIR}/${TESTCASE}/expected.txt >/dev/null 2>&1
            then
                pass=`expr ${pass} + 1`
            else
                echo -e "${BD}${RED}Failed Testcase $TESTCASE! ${ED}"
#                echo -e "${RED}right res:${INPUT_DIR}/${TESTCASE}/output.txt${ED}"
#                echo -e "${RED}your res:./res/output${TESTCASE}.txt${ED}"
#                echo ""
            fi
        done
        echo ""
        MARK=`expr 2 \* ${pass}`
        echo -e "${BD}${GREEN}[Result] $EXCUTABLE ${pass}/${total} test cases passed${ED}"
        echo -e "${BD}${GREEN}[Mark] $EXCUTABLE: ${MARK} ${ED}"
        echo ""
    fi
}

function partb {
    EXCUTABLE=$1

    # check EXCUTABLE 
    if [ ! -f ${EXCUTABLE} ]
    then
        echo -e "${BD}${RED}Cannot find ${EXCUTABLE} !${ED}"
        exit -1
    fi

    if [ "$2" == "scaleout" ]
    then
        echo ""
        echo -e "${BD}${YELLOW}Checking scale-out${ED}"
        echo ""

        TESTCASE="S0"

        { time ./$EXCUTABLE ${INPUT_DIR}/${TESTCASE}/input1.txt ${INPUT_DIR}/${TESTCASE}/input2.txt ${OUTPUT_DIR}/output${TESTCASE}.txt 4;} 2>${OUTPUT_DIR}/tempperf4;
        if diff ${OUTPUT_DIR}/output${TESTCASE}.txt ${INPUT_DIR}/${TESTCASE}/expected.txt >/dev/null 2>&1
        then
            echo
        else
            echo -e "${RED}unmatched:   yours               vs          expected${ED}"
            echo -e "${RED}             ${OUTPUT_DIR}/output${TESTCASE}.txt             ${INPUT_DIR}/${TESTCASE}/expected.txt${ED}"
            exit -1
        fi

        { time ./$EXCUTABLE ${INPUT_DIR}/${TESTCASE}/input1.txt ${INPUT_DIR}/${TESTCASE}/input2.txt ${OUTPUT_DIR}/output${TESTCASE}.txt 3;} 2>${OUTPUT_DIR}/tempperf3;
        { time ./$EXCUTABLE ${INPUT_DIR}/${TESTCASE}/input1.txt ${INPUT_DIR}/${TESTCASE}/input2.txt ${OUTPUT_DIR}/output${TESTCASE}.txt 2;} 2>${OUTPUT_DIR}/tempperf2; 
        { time ./$EXCUTABLE ${INPUT_DIR}/${TESTCASE}/input1.txt ${INPUT_DIR}/${TESTCASE}/input2.txt ${OUTPUT_DIR}/output${TESTCASE}.txt 1;} 2>${OUTPUT_DIR}/tempperf1;

        before=0
        for i in 1 2 3 4
        do
            perf=`cat ${OUTPUT_DIR}/tempperf${i} | grep "real"`
            perf=`echo "$perf" | cut -f2`
            midx=`echo "${perf}" | grep -aob 'm' | grep -oE '[0-9]+'`
#            echo $midx
            min=${perf:0:${midx}}
#            echo $min
            sidx=`echo "${perf}" | grep -aob 's' | grep -oE '[0-9]+'`
#            echo $sidx
            midx=`expr ${midx} + 1`
            sec=${perf:${midx}:`expr ${sidx} - ${midx}`}
#            echo $sec
            calc=$(echo ${min} ${sec} | awk '{ printf "%.3f", $1 * 60 + $2 }')
#            echo $calc

            comp=`echo $before'<'$calc | bc -l`
            if [ "$comp" == "1" ]
            then
                before=$calc
            else
                echo -e "${BD}${RED}Cannot scale out!${ED}"
                exit -1
            fi 
        done

        echo -e "${BD}${GREEN}Scale out!${ED}"
    else
        TESTCASE=$2
        if [ ! -d ${INPUT_DIR}/${TESTCASE} ]; then
            echo -e "${BD}${RED}Testcase ${INPUT_DIR}/${TESTCASE} not exist!${ED}"
            exit -1
        fi
        echo ""
        echo -e "${BD}${YELLOW}Checking $EXCUTABLE Performance of Testcase $TESTCASE ${ED}"
        echo ""
        
#        cat ${INPUT_DIR}/${TESTCASE}/README
#        echo ""
        
        { time ./$EXCUTABLE ${INPUT_DIR}/${TESTCASE}/input1.txt ${INPUT_DIR}/${TESTCASE}/input2.txt ${OUTPUT_DIR}/output${TESTCASE}.txt $3;} 2>${OUTPUT_DIR}/tempperf;

        if diff ${OUTPUT_DIR}/output${TESTCASE}.txt ${INPUT_DIR}/${TESTCASE}/expected.txt >/dev/null 2>&1
        then
            cat ${OUTPUT_DIR}/tempperf | grep "real"
            rm ${OUTPUT_DIR}/tempperf
        else
            echo -e "${BD}${RED}""Failed Testcase $TESTCASE! ${ED}"
            echo -e "${RED}unmatched:     yours             vs          expected${ED}"
            echo -e "${RED}               ${OUTPUT_DIR}/output${TESTCASE}.txt           ${INPUT_DIR}/${TESTCASE}/expected.txt${ED}"
#            echo -e "${RED}right res:${INPUT_DIR}/${TESTCASE}/output.txt${ED}" 
#            echo -e "${RED}your res:./res/output${TESTCASE}.txt${ED}"
        fi 
    fi

}

#################################
main $1 $2 $3 $4
