#ifndef DATA_H_
#define DATA_H_

namespace network{
typedef struct Data{
		bool empty;
		char*  response;
		size_t length;
		Data(bool e = true, char* r = nullptr, size_t l = 0) : empty(e), response(r), length(l) {};
	}Data;


}

#endif
