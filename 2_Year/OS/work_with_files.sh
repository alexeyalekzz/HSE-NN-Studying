#!/bin/bash
clear

function help() {

echo -e "-------------------------------------
Print:
- 'copy' to copy files or directories

- 'del' to delete files or directories

- 'move' to move files or directories

- 'create' to create files or directories

- 'dir' to list directory contents

- 'exit' to exit programm

- 'help' to show this message
-------------------------------------\n"
}

function Done() {

    echo -en "\e[32mDone!\n\n\e[0m"
}

function wrong_input() {

    echo -en "\e[31mWrong input!\n\e[0m"
}

function scan() {

local inv=">>"

    echo -en "\e[1;33m$inv\e[0m"
    read $1
}

function copy() {

    local err="", from="", to=""

    echo "From:"
    scan from
    echo "To:"
    scan to

    err=`sudo cp -r $from $to 2>&1 >/dev/null`
    if [[ "$?" = 1 ]] ; then
        wrong_input
        echo -en "\e[31m$err\e[0m\n\n"
        return
    fi

    Done
}

function del() {

    local err="", name=""

    echo "Write filename or directory name that you want to delete"
    scan name

    err=`find $name 2>&1 >/dev/null`
    if [[ "$?" = 0 ]] ; then
        sudo rm -rf $name
        Done
    else
        wrong_input
        echo -en "\e[31m$err\e[0m\n\n"
    fi
}

function move() {

    local err="", from="", to=""

    echo "From:"
    scan from
    echo "To:"
    scan to

    err=`sudo mv $from $to 2>&1 >/dev/null`
    if [[ "$?" = 1 ]] ; then
        wrong_input
        echo -en "\e[31m$err\e[0m\n\n"
        return
    fi

    Done
}

function create() {

    local name="", ans=""

    echo "Write newfile name"
    scan name

    find $name >/dev/null 2>/dev/null
    if [ "$?" = 0 ] ; then
        echo "File is already existed, do you want to overwrite it? (y / n)"
        scan ans

        while [ "$ans" != "y" ] && [ "$ans" != "n" ]
        do
            wrong_input
            echo -en "Type \e[32m'y'\e[0m or \e[31m'n'\e[0m\n"
            scan ans
        done

        if [[ "$ans" = "y" ]] ; then
            sudo rm "$name"
            touch "$name"
        else
            echo
            return
        fi
    fi

    touch "$name"
    Done
}

function main() {

    local act=""
    help

    while true
    do
    scan act

    case "$act" in
    "copy" )
    copy
    ;;

    "del" )
    del
    ;;

    "move" )
    move
    ;;

    "create" )
    create
    ;;

    "dir" )
    ls --color=always
    echo
    ;;

    "help" )
    help
    ;;

    "exit" )
    return 0
    ;;

    "" )
    ;;

    * )
    echo "There's no such command, try again"
    ;;

    esac

    done
}

main