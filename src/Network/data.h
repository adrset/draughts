#ifndef DATA_H_
#define DATA_H_

namespace Network{
typedef struct data{
		bool empty;
		char*  response;
		size_t length;
		data(bool e = true, char* r = nullptr, size_t l = 0) : empty(e), response(r), length(l) {};
	}data;


}

#endif
