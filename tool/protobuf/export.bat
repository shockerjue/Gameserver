cd  %cd%\win32\bin
set current_dir=%cd%
set root_dir=%current_dir%\..\..\..\..\
set proto_dir=%root_dir%proto\message
set export_dir=%root_dir%proto\export
set proto_path=E:\GameHollyWood\temp\game\proto\message

for /R %proto_dir% %%s in (*.proto) do ( 
	protoc.exe --proto_path=%proto_path% %%s --cpp_out=%export_dir%
)

copy %export_dir%\msgheader.pb.h 	%root_dir%\common\msgdefine\
copy %export_dir%\msgheader.pb.cc 	%root_dir%\common\msgdefine\

copy %export_dir%\ipc.pb.h 	%root_dir%\common\ipc\
copy %export_dir%\ipc.pb.cc 	%root_dir%\common\ipc\

copy %export_dir%\gateway_msgheader.pb.h  %root_dir%\gateway\src\msghandle\
copy %export_dir%\gateway_msgheader.pb.cc %root_dir%\gateway\src\msghandle\

copy %export_dir%\gameserver_msgheader.pb.h  %root_dir%\gameserver\src\msghandle\
copy %export_dir%\gameserver_msgheader.pb.cc %root_dir%\gameserver\src\msghandle\

copy %export_dir%\gs_gw_ipc.pb.h	%root_dir%\common\ipc\
copy %export_dir%\gs_gw_ipc.pb.cc	%root_dir%\common\ipc\

copy %export_dir%\gs_chat_ipc.pb.h	%root_dir%\common\ipc\
copy %export_dir%\gs_chat_ipc.pb.cc	%root_dir%\common\ipc\

copy %export_dir%\gs_db_ipc.pb.h	%root_dir%\common\ipc\
copy %export_dir%\gs_db_ipc.pb.cc	%root_dir%\common\ipc\

copy %export_dir%\gs_login_ipc.pb.h	%root_dir%\common\ipc\
copy %export_dir%\gs_login_ipc.pb.cc	%root_dir%\common\ipc\

copy %export_dir%\chat_db_ipc.pb.h	%root_dir%\common\ipc\
copy %export_dir%\chat_db_ipc.pb.cc	%root_dir%\common\ipc\

copy %export_dir%\gw_db_ipc.pb.h	%root_dir%\common\ipc\
copy %export_dir%\gw_db_ipc.pb.cc	%root_dir%\common\ipc\

copy %export_dir%\gy_login_ipc.pb.h		%root_dir%\common\ipc\
copy %export_dir%\gy_login_ipc.pb.cc	%root_dir%\common\ipc\

copy %export_dir%\login_db_ipc.pb.h		%root_dir%\common\ipc\
copy %export_dir%\login_db_ipc.pb.cc	%root_dir%\common\ipc\

::protoc.exe --proto_path=%proto_dir% %proto_dir%struct.proto --cpp_out=%export_dir%

del %export_dir%\*


@ping 127.0.0.1 -n 4 -w 1000 > nul