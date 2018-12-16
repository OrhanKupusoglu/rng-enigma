#!/bin/bash

# exit immediately on errors
set -e

# read arguments
ACTION="$1"
COMMAND="$2"

# parameter expansion - to clear white space
ACTION=${ACTION/ //}
COMMAND=${COMMAND/ //}

# get working dir
DIR_WORK=$(cd $(dirname "$0") && pwd)

# helper functions
help () {
    echo "USAGE: enter a command, no command defaults to 'build'"
    echo "    build         -- call 'swig cmake; swig make'"
    echo "    clean         -- call 'swig clean'"
    echo "    purge         -- call 'swig purge'"
    echo "    expunge       -- call 'clean; purge'"
    echo "    help          -- print this help"
}

action() {
    local PROJ=$1
    local CMD=$2
    local DIR=""
    local ACTION_ING=""
    local ACTION_ED=""
    local PURGE=""

    case "$PROJ" in
        "swig")     DIR="build" ;;
        *)          echo "ERROR: argument proj '${PROJ}' is not supported" ; exit 1 ;;
    esac

    case "$CMD" in
        "cmake")    ACTION_ING="CMaking" ; ACTION_ED="CMaked" ;;
        "make")     ACTION_ING="Making" ; ACTION_ED="Maked" ;;
        "clean")    ACTION_ING="Cleaning" ; ACTION_ED="Cleaned" ;;
        "purge")    ACTION_ING="Purging" ; ACTION_ED="Purged" ; ;;
        *)          echo "ERROR: argument command '${CMD}' is not supported" ; exit 2 ;;
    esac

    cd $DIR_WORK/$DIR

    echo "++ ${ACTION_ING} the ${DIR} project ..."

    case "$CMD" in
        "cmake")      cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_EXPORT_COMPILE_COMMANDS=1 ../. ;;
        "make")       make ;;
        "clean")      make clean ;;
        "purge")      find . ! -name '.gitignore' -delete ;;
    esac

    echo "++ ${ACTION_ED} the ${DIR} project."
}

# compound commands
build() {
    action swig cmake
    action swig make
}

clean() {
    action swig clean
}

purge() {
    action swig purge
}

expunge() {
    clean
    purge
}

# action
case "$ACTION" in
    "build")        build ; exit 0 ;;
    "clean")        clean ; exit 0 ;;
    "purge")        purge ; exit 0 ;;
    "expunge")      expunge ; exit 0 ;;
    "help")         help ; exit 0 ;;
    "swig")         action "$ACTION" "$COMMAND" ; exit 0 ;;
    "")             build ; exit 0 ;;
    *)              echo "ERROR: command '${ACTION}' is not supported" ; help ; exit 4 ;;
esac
