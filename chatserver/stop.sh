#!/bin/sh

CURRENT_DIR=$(pwd)
cd ${CURRENT_DIR}/bin

# /bin/pidof -s ./chatserver | xargs kill
killall -9 chatserver

