#!/bin/sh  
SCRIPT_DIR=$(cd "$(dirname "$0")" && pwd)  
cd "$SCRIPT_DIR"  

for file in ./*.out; do  
    if [ -x "$file" ]; then
        valgrind "$file"  
    fi  
done
