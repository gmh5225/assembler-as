#!/bin/bash

if [[ ! -f ./build/as ]]; then
    echo "Error: No assembler built!"
    exit 1
fi

echo "Running all tests..."
echo ""

for file in ./test/*; do
    echo `basename $file`
    
    build/as $file
    ld a.out -o out
    
    as $file -o test.out
    ld test.out -o test.bin
    
    ./out
    R1=$?
    
    ./test.bin
    R2=$?
    
    rm a.out
    rm out
    rm test.out
    rm test.bin
    
    if [[ $R1 == $R2 ]] ; then
        echo "Pass"
        echo ""
    else
        echo "Test failed."
        echo "Expected: $R2"
        echo "Actual: $R1"
        exit 1
    fi
done

echo ""
echo "Done"

