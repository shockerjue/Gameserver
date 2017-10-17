#!/bin/sh

LINUX=Linux
APPLE=Darwin
BIT64=x86_64
PLATOM_NAME=$(uname -s)
PLATOM_BITS=$(uname -m)

if [ ${PLATOM_NAME} = ${APPLE} ]; then
	if [ ${PLATOM_BITS} = ${BIT64} ]; then
		cd ./osx64/bin
	else
		cd ./osx32/bin
	fi	
fi

if [ ${PLATOM_NAME} = ${LINUX} ]; then
	if [ ${PLATOM_BITS} = ${BIT64} ]; then
		cd ./linux64/bin
	else 
		cd ./linux32/bin
	fi
fi

current_dir=$(pwd)
root_dir=${current_dir}/../../../../
proto_dir=${root_dir}/proto
proto_path=${proto_dir}/message
export_dir=${proto_dir}/export

pwd 

filelist=`ls ${proto_path}`
for file in $filelist
do 
	./protoc --proto_path=${proto_path} ${proto_path}/${file} --cpp_out=${export_dir}
	echo ${export_dir}/${file}.pb.h
	echo ${export_dir}/${file}.pb.cc
done

cp ${export_dir}/ipc.pb.h 	${root_dir}/common/ipc/
cp ${export_dir}/ipc.pb.cc	${root_dir}/common/ipc/

cp ${export_dir}/msgheader.pb.h 	${root_dir}/common/msgdefine/
cp ${export_dir}/msgheader.pb.cc 	${root_dir}/common/msgdefine/

cp ${export_dir}/gateway_msgheader.pb.h  ${root_dir}/gateway/src/msghandle/
cp ${export_dir}/gateway_msgheader.pb.cc ${root_dir}/gateway/src/msghandle/

cp ${export_dir}/gameserver_msgheader.pb.h  ${root_dir}/gameserver/src/msghandle/
cp ${export_dir}/gameserver_msgheader.pb.cc ${root_dir}/gameserver/src/msghandle/

cp ${export_dir}/gs_gw_ipc.pb.h		${root_dir}/common/ipc/
cp ${export_dir}/gs_gw_ipc.pb.cc	${root_dir}/common/ipc/

cp ${export_dir}/gs_chat_ipc.pb.h		${root_dir}/common/ipc/
cp ${export_dir}/gs_chat_ipc.pb.cc	${root_dir}/common/ipc/

cp ${export_dir}/gs_db_ipc.pb.h		${root_dir}/common/ipc/
cp ${export_dir}/gs_db_ipc.pb.cc	${root_dir}/common/ipc/

cp ${export_dir}/gs_login_ipc.pb.h		${root_dir}/common/ipc/
cp ${export_dir}/gs_login_ipc.pb.cc	${root_dir}/common/ipc/

cp ${export_dir}/chat_db_ipc.pb.h		${root_dir}/common/ipc/
cp ${export_dir}/chat_db_ipc.pb.cc	${root_dir}/common/ipc/

cp ${export_dir}/gw_db_ipc.pb.h		${root_dir}/common/ipc/
cp ${export_dir}/gw_db_ipc.pb.cc	${root_dir}/common/ipc/

cp ${export_dir}/gy_login_ipc.pb.h		${root_dir}/common/ipc/
cp ${export_dir}/gy_login_ipc.pb.cc	${root_dir}/common/ipc/

cp ${export_dir}/login_db_ipc.pb.h		${root_dir}/common/ipc/
cp ${export_dir}/login_db_ipc.pb.cc	${root_dir}/common/ipc/

rm ${export_dir}/*

echo "export finished"
