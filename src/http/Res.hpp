#pragma once
#include <string>
#define HTTPVER "HTTP/1.1 "

class Res {

	typedef struct header {
		unsigned int	status;
		size_t			contentLength;
		std::string		contentType;
	}	s_header;

private:
	s_header	_header;
	std::string	_raw = HTTPVER;

public:
	Res(void);

	s_header&	header(void);
};