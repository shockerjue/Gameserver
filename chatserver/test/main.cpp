#include <map>
#include <iostream>
#include "./../src/playerpool/playerpool.h"

using namespace std;
using namespace player;

int main(int argc,const char* argv[]){
	PItem p1;
	p1.rid = 9012;
	p1.netid = 1;
	p1.gulid = 12;

	PItem p2;
	p2.rid = 9013;
	p2.netid = 1;
	p2.gulid = 12;


	PlayerPool::getPlayerPool()->registerPItem(9012,p1);
	PlayerPool::getPlayerPool()->registerPItem(9013,p2);

	map<int,PItem>   pitems = PlayerPool::getPlayerPool()->getAllPlayer();
	map<int,PItem>::iterator it = pitems.begin();
	while(it != pitems.end()){
		cout<<it->second.toString()<<endl;

		it++;
	}


	return 0;
}