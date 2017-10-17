#!/bin/sh

CURRENT_DIR=$(pwd)

# 在启动服务器以前，先启动日志服务
# cd ${CURRENT_DIR}/logserver
# sh start.sh
sh stop.sh

cd ${CURRENT_DIR}/dataaccess
sh start.sh

sleep 10

cd ${CURRENT_DIR}/gameworld
sh start.sh


cd ${CURRENT_DIR}/chatserver
sh start.sh


cd ${CURRENT_DIR}/login
sh start.sh

sleep 2

cd ${CURRENT_DIR}/gateway
sh start.sh

sleep 2

cd ${CURRENT_DIR}/gameserver
sh start.sh 3
