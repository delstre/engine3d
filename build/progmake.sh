#!/bin/bash
r_flag=false
l_flag=false

while getopts "r" opt; do
    case $opt in
        r)
            r_flag=true
            ;;
        l)
            l_flag=true
            ;;
    esac
done

if [ "$l_flag" = true ]; then
    make -C .. clean
    make -j16 -C .. lib
fi

if [ "$r_flag" = true ]; then
    make -j16 -C .. all && gdb ./proj
    exit
fi
