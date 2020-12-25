#!/bin/bash
  
gcc main.c -o tempo

if [ $? -ne 0 ]; then
    echo "Failure, you are!"
else
    ./tempo
fi