#!/bin/sh

CURRENT_DIR=$(pwd)
cd ${CURRENT_DIR}/bin

# /bin/pidof -s ./gateway | xargs kill

killall -9 gateway
