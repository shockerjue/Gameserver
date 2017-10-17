#!/bin/sh

CURRENT_DIR=$(pwd)
cd ${CURRENT_DIR}/bin

nohup ./gameworld ./../config/config.ini > ~/log/gameworld.log 2>&1 &

