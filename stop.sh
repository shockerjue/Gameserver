#!/bin/sh

CURRENT_DIR=$(pwd)

# 在启动服务器以前，先启动日志服务
# cd ${CURRENT_DIR}/logserver
# sh start.sh


cd ${CURRENT_DIR}/dataaccess
sh stop.sh


cd ${CURRENT_DIR}/gameworld
sh stop.sh


cd ${CURRENT_DIR}/chatserver
sh stop.sh


cd ${CURRENT_DIR}/login
sh stop.sh


cd ${CURRENT_DIR}/gateway
sh stop.sh


cd ${CURRENT_DIR}/gameserver
sh stop.sh
