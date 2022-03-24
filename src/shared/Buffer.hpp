#ifndef WS_SHARED_BUFFER_HPP
# define WS_SHARED_BUFFER_HPP

# include <string>
# include <algorithm>
# include <cstring>

namespace ws
{
	namespace shared
	{
		class Buffer
		{
		public:

			Buffer();
			Buffer(size_t size);
			Buffer(const std::string& str);
			Buffer(const char *buff, size_t size);
			Buffer(const Buffer & rhs);
			~Buffer();

			Buffer & operator=(const Buffer & rhs);

			size_t	capacity() const;
			size_t	size() const;
			size_t	find(const char c) const;
			size_t	find(const char* s) const;
			size_t	find(const std::string& s) const;
			size_t	find(const char* c, size_t until) const;
			size_t	find(size_t from, const char* c) const;

			size_t	find_last_of_from(const std::string& s, size_t from) const;
			char * get_ptr();
			const char * get_ptr() const;

			std::string to_string() const;

			Buffer&	advance(size_t n);
			Buffer&	resetCursor(void);

			void	join(const Buffer& buff);
			void	join(const std::string& s);
			void	join(const char* buff, size_t size);

			const char&	operator[](size_t index) const;

		private:

			char * _data;
			size_t _size;
			size_t _cursor;

		};
	}
}

#endif
