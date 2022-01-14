#pragma once
#include <string>

class Req
{
	enum e_method
	{
		GET,
		POST,
		PUT,
		DELETE
	};

	struct s_header
	{
		e_method		method;
		std::string		path;
		std::string		host;
		bool			keepAlive;
		std::string		
	};

	typedef struct s_header s_header;

private:
	std::string	_header;
	std::string	_body;
public:
	Req(const s_buff& buff);
	
	const std::string&	header(const std::string& field) const;
	const std::string&	body(const std::string& field) const;
};