#include "Buffer.hpp"
#include <iostream>

namespace ws
{
	namespace shared
	{
		Buffer::Buffer(size_t size)
		{
			this->_size = size;
			this->_data = new char[this->_size + 1]();
			std::fill_n(this->_data, this->_size, 0);
		}

		Buffer::Buffer(std::string str)
		{
			this->_size = str.size();
			this->_data = new char[this->_size + 1]();
			std::memcpy(this->_data, str.c_str(), this->_size);
		}

		Buffer::Buffer(char *buff, size_t size)
		{
			this->assign(buff, size);
		}

		void	Buffer::assign(char* buff, size_t size)
		{
//			this->~Buffer();
			this->_size = size;
			this->_data = new char[this->_size + 1]();
			std::memcpy(this->_data, buff, this->_size);
		}

		Buffer::Buffer(const Buffer & rhs)
		{
			*this = rhs;
		}
		
		Buffer::~Buffer()
		{
			delete [] this->_data;
		}

		Buffer & Buffer::operator=(const Buffer & rhs)
		{
			if (this != &rhs)
			{
				this->~Buffer();
				this->_size = rhs._size;
				this->_data = new char[this->_size + 1]();
				std::memcpy(this->_data, rhs._data, this->_size);
			}
			return *this;
		}

		size_t Buffer::size() const
		{
			return this->_size;
		}

		char * Buffer::get_ptr()
		{
			return this->_data;
		}

		const char * Buffer::get_ptr() const
		{
			return this->_data;
		}

		std::string Buffer::to_string()
		{
			return std::string(this->_data, this->_size);
		}

		void Buffer::join(Buffer buff)
		{
			Buffer newb = Buffer(this->size() + buff.size());
		}
	}
}