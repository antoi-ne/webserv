#include "Buffer.hpp"
#include <iostream>

namespace ws
{
	namespace shared
	{
		Buffer::Buffer(size_t size)
			: _size(size), _cursor(0)
		{
			this->_data = new char[this->_size + 1]();
			std::fill_n(this->_data, this->_size, 0);
		}

		Buffer::Buffer(std::string str)
			: _cursor(0)
		{
			this->_size = str.size();
			this->_data = new char[this->_size + 1]();
			std::memcpy(this->_data, str.c_str(), this->_size);
		}

		Buffer::Buffer(const char *buff, size_t size)
			: _cursor(0)
		{
			this->_size = size;
			this->_data = new char[this->_size + 1]();
			std::memcpy(this->_data, buff, this->_size);
		}

		Buffer::Buffer(const Buffer & rhs)
			: _cursor(0)
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
				this->_size = rhs.size();
				this->_data = new char[this->_size + 1]();
				std::memcpy(this->_data, rhs.get_ptr(), this->_size);
			}
			return *this;
		}

		size_t Buffer::size() const
		{
			return this->_size - this->_cursor;
		}

		size_t Buffer::capacity() const
		{
			return this->_size;
		}

		char * Buffer::get_ptr()
		{
			return this->_data + this->_cursor;
		}

		const char * Buffer::get_ptr() const
		{
			return this->_data + this->_cursor;
		}

		std::string Buffer::to_string() const
		{
			return std::string(this->_data + this->_cursor, this->_size - this->_cursor);
		}

		Buffer&	Buffer::advance(size_t n)
		{
			this->_cursor += n;
			return *this;
		}


		void	Buffer::join(const Buffer& buff)
		{
			size_t	newSize = this->size() + buff.size();
			char*	tmp = new char[newSize]();

			std::memcpy(tmp, this->get_ptr(), this->size());
			std::memcpy(tmp + this->size(), buff.get_ptr(), buff.size());
			*this = Buffer(tmp, newSize);
		}
	}
}
