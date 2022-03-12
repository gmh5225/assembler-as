#!/bin/bash

if [[ ! -f ./build/as ]]; then
    echo "Error: No assembler built!"
    exit 1
fi

echo "Running all tests..."
echo ""

total=0

for d in ./test/*; do
    for file in $d/*; do
        echo "`basename $d` `basename $file`"
        
        build/as $file
        ld a.out -o out
        
        as $file -o test.out
        ld test.out -o test.bin
        
        ./out
        R1=$?
        
        ./test.bin
        R2=$?
        
        NM1=`nm a.out`
        NM2=`nm test.out`
        
        rm a.out
        rm out
        rm test.out
        rm test.bin
        
        if [[ $R1 == $R2 ]] ; then
            if [[ $NM1 == $NM2 ]] ; then
                echo "Pass"
                echo ""
                
                total=$((total+1))
            else
                echo "Test failed- symbol check."
                echo "Expected":
                echo "$NM2"
                echo "Actual:"
                echo "$NM1"
                echo ""
                exit 1
            fi
        else
            echo "Test failed."
            echo "Expected: $R2"
            echo "Actual: $R1"
            exit 1
        fi
    done
done

echo ""
echo "$total tests passed!"

echo ""
echo "Done"

