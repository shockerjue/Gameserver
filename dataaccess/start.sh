#!/bin/sh

CURRENT_DIR=$(pwd)
cd ${CURRENT_DIR}/bin

nohup ./dataaccess ./../config/config2.ini > ~/log/dataaccess2.log 2>&1 &

sleep 7

nohup ./dataaccess ./../config/config.ini > ~/log/dataaccess1.log 2>&1 &
