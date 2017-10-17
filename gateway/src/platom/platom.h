#ifndef H_PLATOM_H_
#define H_PLATOM_H_

#ifdef _cplusplus
extern "C"{
#endif
	#include <stdio.h>
	#include <fcntl.h>
	#include <unistd.h>
	#include <sys/types.h>
#ifdef _cplusplus
}
#endif

#include <iostream>

#include "./../../../common/util/util.h"
#include "./../../../3tdarty/json/json.hpp"

using namespace std;
using namespace util;
using json = nlohmann::json;

namespace platom {
	struct PlatomItem {
		string 		m_platomid;

		inline string toString() {
			char temp[128] = {0};

			sprintf(temp,"\n{\n\tm_platomid: %s\n}\n",this->m_platomid.c_str());

			return string(temp);
		}
	};


	class Platom {
	public:
		~Platom(void);
		void parse(string config);
		bool verification(string platomid);
		
		static Platom* getPlatom(void);
	private:
		Platom(void);

	private:
		static Platom* s_pPlatom;

		vector<struct PlatomItem> m_platoms;
	};
}

#endif