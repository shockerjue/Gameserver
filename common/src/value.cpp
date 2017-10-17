#include "./../include/value.h"


namespace engine {
	Value::Value(void){
		this->m_value = "";
	}

	Value::~Value(void){

	}

	Value::Value(const string value){
		this->m_value = value;
	}

	Value::Value(const Value& value){
		this->m_value = value.m_value;
	}

	Value& Value::operator=(const Value& value){
		if(this == &value){
			return *this;
		}

		this->m_value = value.m_value;

		return *this;		
	}

	Value& Value::operator=(const string value){
		if(0 == this->m_value.compare(value)){
			return *this;
		}

		this->m_value = value;

		return *this;		
	}

	bool Value::operator==(const string value){
		if(0 == this->m_value.compare(value)){
			return true ;
		}

		return false;
	}

	bool Value::operator==(const Value& value){
		if(0 == this->m_value.compare(value.m_value)){
			return true ;
		}

		return false ;
	}

	int Value::toInt(void){
		if(0 == this->m_value.size()){
			return 0;
		}

		const char* p = this->m_value.c_str();
		if(nullptr == p){

			return 0;
		}

		int value = 0;
		while(nullptr != p && '\0' != *p){
			if('0' > *p || '9' < *p){

				break ;
			}

			value = (value * 10) + (*p - '0');

			p++;
		}

		return value;
	}

	float Value::toFloat(void){
		if(0 == this->m_value.size()){
			return 0.0f;
		}

		const char* p = this->m_value.c_str();
		if(nullptr == p){

			return 0.0f;
		}

		bool new_s = false;
		float value = 0.0f;
		float fenmu = 10.0f;

		while(nullptr != p && '\0' != *p){
			if(('0' > *p || '9' < *p) && '.' != *p){

				break;
			}

			if('.' == *p){
				p++;
				new_s = true;

				continue ;
			}

			if(!new_s){
				value = (value * 10.0f) +  float(*p - '0');

			}else if(new_s) {
				value = (float(*p - '0') * (1.0f / fenmu)) + value;
				fenmu = fenmu * 10.0f;
			}

			p++;
		}

		return value;
	}

	char Value::toChar(void){
		if(0 == this->m_value.size()){
			return 0;
		}

		const char* p = this->m_value.c_str();
		if(nullptr == p){

			return 0;
		}

		return *p;
	}

	bool Value::toBool(void){
		if(0 == this->m_value.size()){
			return false;
		}

		if(0 == this->m_value.compare("True") || 
			0 == this->m_value.compare("true") || 
			0 == this->m_value.compare("1") || 
			0 == this->m_value.compare("YES") || 
			0 == this->m_value.compare("Yes") ||
			0 == this->m_value.compare("yes")){

			return true;
		}else if(0 == this->m_value.compare("False") ||
			0 == this->m_value.compare("false") ||
			0 == this->m_value.compare("0") || 
			0 == this->m_value.compare("NO") || 
			0 == this->m_value.compare("No") || 
			0 == this->m_value.compare("no")){

			return false;
		}else {
		}

		return false;
	}

	string Value::toString(void){
		if(0 == this->m_value.size()){
			return string("");
		}

		return this->m_value;
	}

	void Value::SetValue(string value){
		if(0 == value.size()){
			return ;
		}

		this->m_value = value;
	}

	bool Value::isEmpty(void){
		if(0 == this->m_value.size()){
			return true;
		}

		return false;
	}
}