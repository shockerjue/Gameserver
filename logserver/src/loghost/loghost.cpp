#include "loghost.h"

namespace logserver {

	void LogHost::onRecvLog(NetId netid,const char* buf, const int size){
		if(nullptr == buf || !size){
			return ;
		}

		string data;
		data.assign(buf,size);

		SystemLogIPC _slog;
		_slog.ParseFromString(data);

		int level = _slog.level();
		switch(level){
			case LogLevel::I:{
				Log::Print(_slog.proc(), _slog.func(), _slog.date(), _slog.msg());
				LogRW::write(0,_slog.proc(), _slog.func(), _slog.msg(), _slog.date());
			}break;
			case LogLevel::W:{
				Log::Warning(_slog.proc(), _slog.func(), _slog.date(), _slog.msg());
				LogRW::write(1,_slog.proc(), _slog.func(), _slog.msg(), _slog.date());
			}break;
			case LogLevel::E:{
				Log::Perror(_slog.proc(), _slog.func(), _slog.date(), _slog.msg());
				LogRW::write(2,_slog.proc(), _slog.func(), _slog.msg(), _slog.date());
			};
			default:{
				Log::Warning(__func__, _slog.proc() + "   Can't identify Log!   " + _slog.func());
			}break;
		}
	}
}