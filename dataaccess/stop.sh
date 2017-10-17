#!/bin/sh

CURRENT_DIR=$(pwd)
cd ${CURRENT_DIR}/bin

# /bin/pidof -s ./dataaccess | xargs killall -9
killall -9 dataaccess

