#include "shareram_test.h"

void onSave(ShareRam* share){
	bool ret = share->onReInit(sizeof(struct Share));
	if(!ret){
		Log::Print(__func__, "shareram->onInit fail!");

		return ;
	}

	struct Share temp;
	temp.rid = 123456789;
	void* sret = share->save(12909,(void*)&temp,sizeof(struct Share));
	if(nullptr == sret){
		Log::Perror(__func__, "save error!");

		return ;
	}

	Log::Print(__func__, "shareram->onInit sucess!");
	void* testret = share->getram(12909,false);
	if(nullptr == testret){
		Log::Perror(__func__, "getram error!");

		return ;
	}

	Log::Print(__func__, "shareram->getram success!");
	struct Share* temps = (struct Share*)testret;
	cout<<"Share.rid:"<<temps->rid<<endl;
}

void onLoad(ShareRam* share){
	bool ret = share->onLoad(sizeof(struct Share));
	if(!ret){
		Log::Print(__func__, "shareram->onInit fail!");
	}

	Log::Print(__func__, "shareram->onInit sucess!");
	void* testret = share->getram(12909,false);
	if(nullptr == testret){
		Log::Perror(__func__, "getram error!");

		return ;
	}

	Log::Print(__func__, "shareram->getram success!");
	struct Share* temp = (struct Share*)testret;
	cout<<"Share.rid:"<<temp->rid<<endl;

	// struct Share temp2;
	// temp2.rid = 22222222;
	// void* sret = share->save(12910,(void*)&temp2,sizeof(struct Share));
	// if(nullptr == sret){
	// 	Log::Perror(__func__, "save error!");
	// }
}

void shareram_test(void){
	ShareRam* share = ShareRam::getShareRam(0x123);
	bool ret = share->create(sizeof(struct Share),3000);
	if(!ret){
		Log::Print(__func__, "shareram->create fail!");
	}

	// onSave(share);
	onLoad(share);

	delete share;
	share = nullptr;
}