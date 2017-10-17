#include "thread_test.h"

Thread_Func(thread_test){
	Thread* th = (Thread*)_thread;

	char data[128] = {0};
	sprintf(data,"HelloWorld!---------[%d]",1);

	Message message;
	message.setwhat(9032);
	message.setsize(strlen(data));
	message.setobject(data,strlen(data));
	// bool ret = th->send_message_to_thread(message);
	bool ret = Thread::ssend_message_to_thread(*th,message);

	Log::Print(__func__,ret);
	sleep(5);

	return nullptr;
}

void thread_test(void){
	Log::Print(__func__, "Start thread test ..... ");

	Thread thread1("thread_test");
	thread1.run("thread1",Thread_Call(thread_test));

	int s = 0;
	// vector<Message> msgs = thread1.recv_message_in_thread(&s);
	vector<Message> msgs = Thread::srecv_message_in_thread(thread1,&s);
	Log::Print(__func__,s);
	vector<Message>::iterator it = msgs.begin();
	while(it != msgs.end()){
		Log::Print(__func__,(*it).getwhat());

		it++;
	}

	thread1.wait();
}