#!/bin/sh

CURRENT_DIR=$(pwd)
cd ${CURRENT_DIR}/bin

nohup ./chatserver ./../config/config.ini > ~/log/chatserver.log 2>&1 &

