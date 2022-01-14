#include "Buffer.hpp"

namespace ws
{
	namespace shared
	{

		Buffer::Buffer(std::string str)
		{
			this->_size = str.size();
			this->_data = new char[this->_size]();
			std::fill_n(this->_data, this->_size, 0);
		}

		Buffer::Buffer(char *buff, size_t size)
		{
			this->_size = size;
			this->_data = new char[this->_size]();
			std::memcpy(this->_data, buff, this->_size);
		}
		
		Buffer::~Buffer()
		{
			delete [] this->_data;
		}

		size_t Buffer::size()
		{
			return this->_size;
		}

		char * Buffer::get_ptr()
		{
			return this->_data;
		}

		std::string Buffer::to_string()
		{
			return std::string(this->_data, this->_size);
		}

	}
}