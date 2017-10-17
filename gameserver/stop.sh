#!/bin/sh

CURRENT_DIR=$(pwd)
cd ${CURRENT_DIR}/bin

# 登录相关的其他服务器启动完成
# /bin/pidof -s ./gameserver ./../config/config.ini | xargs kill
# /bin/pidof -s ./gameserver ./../config/config1.ini | xargs kill
# /bin/pidof -s ./gameserver ./../config/config2.ini | xargs kill

killall -9 gameserver