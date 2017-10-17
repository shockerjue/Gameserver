#!/bin/sh

CURRENT_DIR=$(pwd)
cd ${CURRENT_DIR}/bin

# 登录相关的其他服务器启动完成
killall -9 gameserver

if [ 2 -eq $1 ] ;
then
	nohup ./gameserver ./../config/config.ini > ~/log/gameserver.log 2>&1 &
	nohup ./gameserver ./../config/config1.ini > ~/log/gameserver1.log 2>&1 &
elif [ 3 -eq $1 ] ;
then
	nohup ./gameserver ./../config/config.ini > ~/log/gameserver.log 2>&1 &
	nohup ./gameserver ./../config/config1.ini > ~/log/gameserver1.log 2>&1 &
	nohup ./gameserver ./../config/config2.ini > ~/log/gameserver2.log 2>&1 &
else
	nohup ./gameserver ./../config/config.ini > ~/log/gameserver.log 2>&1 &
fi
