#include "dataaccess.h"

namespace dataaccess {
	DataAccess* DataAccess::s_pDataAccess = nullptr;

	DataAccess::DataAccess(void){
		
	}

	DataAccess::~DataAccess(void){
    	
	}

	DataAccess* DataAccess::getDataAccess(void){
		if(nullptr == DataAccess::s_pDataAccess){
			DataAccess::s_pDataAccess = new DataAccess();
		}

		return DataAccess::s_pDataAccess;
	}

	void DataAccess::registerHandler(void){
		EventManage::registerMsgHandle();
		EventManage::registerThreadMsgHandle();
	}

	void DataAccess::createIoThread(void){
		ConfigModule* config = Engine::getConfigModule();
		if(nullptr == config){
			return ;
		}

		bool enable_thread = config->getBoolByKey("enable_thread");
		if(!enable_thread){
			return ;
		}

		Common::RegisterCall();

		bool share_enable = config->getBoolByKey("share_enable");
		int share_ram_n = config->getIntByKey("share_ram_n");
		int share_ram_id = config->getIntByKey("share_key_id");
		if(share_enable) {
			ShareCache::getShareCache()->set_use();
			ShareCache::getShareCache()->initShareCache(share_ram_n,share_ram_id);
		}

		int i = 0;
		int j = 0;
		bool enable_read = config->getBoolByKey("enable_read");
		bool enable_sync = config->getBoolByKey("enable_sync");
		bool enable_write = config->getBoolByKey("enable_write");
		bool enable_read_ipc = config->getBoolByKey("enable_read_ipc");
		bool enable_write_ipc = config->getBoolByKey("enable_write_ipc");

		if(enable_write) {
			int write_number = config->getIntByKey("write_number");
			if(0 < write_number) {
				for(i = 0; i < write_number && j < 10; ++i,++j){
					char t_name[50] = {0};

					sleep(1);
					sprintf(t_name, "io_write%d",i);
					this->m_thread[j].run(string(t_name),Thread_Call(io_write));

					if(!enable_write_ipc){
						continue ;
					}

					this->addThreadMsgEvent(this->m_thread[j]);
				}
			}
		}


		if(enable_read){
			int read_number = config->getIntByKey("read_number");
			if(0 < read_number) {
				for(i = 0; i < read_number && j < 10; ++i,++j){
					char t_name[50] = {0};

					sleep(1);
					sprintf(t_name, "io_read%d",i);
					this->m_thread[j].run(string(t_name),Thread_Call(io_read));

					if(!enable_read_ipc){
						continue ;
					}

					this->addThreadMsgEvent(this->m_thread[j]);
				}
			}
		}

		if(enable_sync && share_enable) {
			this->m_thread[j++].run("io_sync",Thread_Call(io_sync));
		}
	}
}