#!/bin/sh

CURRENT_DIR=$(pwd)
cd ${CURRENT_DIR}/bin

# /bin/pidof -s ./gameworld | xargs kill

killall -9 gameworld