#pragma once
#include <string>

class Res
{

	typedef struct header
	{
		unsigned int status;
		size_t contentLength;
		std::string contentType;
	} s_header;

private:
	s_header _header;
	std::string _raw;

public:
	Res(void);

	s_header &header(void);
};