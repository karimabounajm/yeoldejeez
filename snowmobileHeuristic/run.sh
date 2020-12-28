#!/bin/bash
  
gcc -Wall -Werror main.c -o tempo

if [ $? -ne 0 ]; then
    echo "Failure, you are!"
else
    ./tempo
    # 3
fi
