#ifndef H_DATAACCESS_H_
#define H_DATAACCESS_H_

#include "io-thread/common.h"
#include "io-thread/io_thread.h"
#include "sharecache/sharecache.h"
#include "eventhandle/event_manage.h"
#include "./../../common/include/servermodule.h"
#include "./../../common/include/shareram.h"


using namespace engine;
using namespace io_thread;
using namespace sharecache;
using namespace dataaccess::event;

namespace dataaccess {
	struct Share {
		int rid;
	};

	class DataAccess : public ServerModule {
	public:
    	~DataAccess(void);

    	virtual void registerHandler(void);
		virtual void createIoThread(void);

    	static DataAccess* getDataAccess(void);
    	
	private:
		DataAccess(void);
		DataAccess(const DataAccess&){}
		DataAccess& operator=(const DataAccess&){ return *this; }

	private:
		Thread m_thread[10];

	private:
		static DataAccess* 	s_pDataAccess;
	};
}

#endif