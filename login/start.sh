#!/bin/sh

CURRENT_DIR=$(pwd)
cd ${CURRENT_DIR}/bin


nohup ./login ./../config/config.ini > ~/log/login.log 2>&1 &

