#ifndef RETURN_CODE_HPP 
# define RETURN_CODE_HPP

enum return_code
{
    UNDIFND = -1,
	MOVED_PERMANENTLY = 301,
    MOVED_TEMPORARLY,
    SEE_OTHER, 
    TEMPORARY_REDIRECT = 307
};

# endif