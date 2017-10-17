#!/bin/sh

CURRENT_DIR=$(pwd)
cd ${CURRENT_DIR}/bin

# 登录DB服务器启动完成

./logserver ./../config/config.ini
