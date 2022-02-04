#include "Buffer.hpp"
#include <iostream>

namespace ws
{
	namespace shared
	{
		Buffer::Buffer()
			: _data(), _size(0), _cursor(0)
		{
			this->_data = new char[this->_size + 1]();
			std::memset(this->_data, 0, this->_size);
		}

		Buffer::Buffer(size_t size)
			: _data(), _size(size), _cursor(0)
		{
			this->_data = new char[this->_size + 1]();
			std::memset(this->_data, 0, this->_size);
		}

		Buffer::Buffer(const std::string& str)
			: _data(), _size(str.size()), _cursor(0)
		{
			this->_data = new char[this->_size + 1]();
			std::memcpy(this->_data, str.c_str(), this->_size);
		}

		Buffer::Buffer(const char *buff, size_t size)
			: _data(), _size(size), _cursor(0)
		{
			this->_data = new char[this->_size + 1]();
			std::memcpy(this->_data, buff, this->_size);
		}

		Buffer::Buffer(const Buffer & rhs)
			: _data(), _size(), _cursor(0)
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
				delete [] this->_data;
				this->_size = rhs._size;
				this->_cursor = rhs._cursor;
				this->_data = new char[this->_size + 1]();
				std::memcpy(this->_data, rhs._data, this->_size);
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

		Buffer&	Buffer::resetCursor(void)
		{
			this->_cursor = 0;
			return *this;
		}

		size_t	Buffer::find(const char c) const
		{
			const char*	ptr = this->get_ptr();
			
			for (size_t	i = 0; i < this->size(); ++i)
			{
				if (ptr[i] == c)
					return i;
			}
			return std::string::npos;
		}

		size_t	Buffer::find(const std::string& s) const
		{ return this->find(s.c_str()); }
		
		size_t	Buffer::find(const char* s) const
		{
			const char*	ptr = this->get_ptr();
			
			for (size_t	i = 0, j = 0; i < this->size(); ++i)
			{
				if (ptr[i] == s[j])
				{
					if (!s[++j])
						return (i - j) + 1;
				}
				else
					j = 0;
			}
			return std::string::npos;
		}

		size_t	Buffer::find(const char* s, size_t until) const
		{
			const char*	ptr = this->get_ptr();
			
			if (until <= this->size())
			{
				for (size_t	i = 0, j = 0; i < until; ++i)
				{
					if (ptr[i] == s[j])
					{
						if (!s[++j])
							return (i - j) + 1;
					}
					else
						j = 0;
				}
			}
			return std::string::npos;
		}

		// return first char of string s
		size_t	Buffer::find_last_of_from(const std::string& s, size_t from) const
		{
			const char*	ptr = this->get_ptr();
			
			for (size_t	i = from, j = s.size() - 1; i > 0; --i)
			{
				if (ptr[i] == s[j])
				{
					if (j == 0)
						return i;
					--j;
				}
				else
					j = s.size() - 1;
			}
			return std::string::npos;
		}

		void	Buffer::join(const std::string& s)
		{
			size_t	newSize = this->_size + s.size();
			char*	tmp = new char[newSize + 1]();

			std::memcpy(tmp, this->_data, this->_size);
			std::memcpy(tmp + this->_size, s.c_str(), s.size());
			tmp[newSize] = 0;
			delete [] this->_data;
			this->_data = tmp;
			this->_size = newSize;
		}

		void	Buffer::join(const char* buff, size_t size)
		{
			size_t	newSize = this->_size + size;
			char*	tmp = new char[newSize + 1]();

			std::memcpy(tmp, this->_data, this->_size);
			std::memcpy(tmp + this->_size, buff, size);
			tmp[newSize] = 0;
			delete [] this->_data;
			this->_data = tmp;
			this->_size = newSize;
		}

		void	Buffer::join(const Buffer& buff)
		{
			size_t	newSize = this->_size + buff.size();
			char*	tmp = new char[newSize + 1]();

			std::memcpy(tmp, this->_data, this->_size);
			std::memcpy(tmp + this->_size, buff.get_ptr(), buff.size());
			tmp[newSize] = 0;
			delete [] this->_data;
			this->_data = tmp;
			this->_size = newSize;
		}

		const char&	Buffer::operator[](size_t index) const
		{ return *(this->get_ptr() + index); }
	}
}
