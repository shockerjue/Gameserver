#!/bin/sh

CURRENT_DIR=$(pwd)
cd ${CURRENT_DIR}/bin

# /bin/pidof -s ./login | xargs kill
killall -9 login

