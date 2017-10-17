#!/bin/sh

CURRENT_DIR=$(pwd)
cd ${CURRENT_DIR}/bin


nohup ./gateway ./../config/config.ini > ~/log/gateway.log 2>&1 &

