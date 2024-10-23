#!/bin/bash
r_flag=false
l_flag=false
c_flag=false

while getopts "rlfc" opt; do
    case $opt in
        r)
            r_flag=true
            ;;
        l)
            l_flag=true
            ;;
        c)
            c_flag=true
            ;;
        f)
            f_flag=true
            ;;
        \?)
            echo "Invalid option: -$OPTARG" >&2
            ;;
    esac
done

if [ "$f_flag" = true ]; then
    make -j16 -C .. clean
    make -j16 -C .. lib
    cp libengine.so projects/new
    cp libengine.so projects/new2
    make -j16 -C .. clean
    make -j16 -C .. all
fi

if [ "$l_flag" = true ]; then
    make -C .. clean
    make -j16 -C .. lib
    cp libengine.so projects/new
    cp libengine.so projects/new2
fi

if [ "$r_flag" = true ]; then
    make -j16 -C .. all && gdb ./proj
fi

if [ "$c_flag" = true ]; then
    make -j16 -C .. clean
fi
