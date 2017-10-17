#include "util.h"

namespace util {
	int Util::chars2int(const char* chrs){
		if(nullptr == chrs){
			return 0;
		}

		int value = 0;
		while(chrs != nullptr && *chrs != '\0'){
			int temp = *chrs - '0';
			value = value * 10;
			value = value + temp;

			chrs++;
		}

		return value;
	}

	int Util::str2int(const string str){
		int value = atoi(str.c_str());

		return value;
	}

	string Util::int2str(const int value){
		string ret = to_string(value);

		return ret;
	}

	string Util::genkey(void){
		srand(Datetime::getTimeustamp());

		char key[50] = {0};
		int i = 0;
		while(i < 35){
			int index = rand() % 38;
			key[i++] = temp_key[index];
			if(i % 6 == 0){
				key[i++] = '-';
			}
		}

		long stamp = Datetime::getTimeustamp();
		while(stamp > 0){
			int temp = stamp % 38;
			if(temp == 0){
				temp = 1;
			}

			int index = rand() % temp;
			stamp = stamp / 38;

			key[i++] = temp_key[index];

			if(i % 6 == 0){
				key[i++] = '-';
			}
		}

		return string(key);
	}
}