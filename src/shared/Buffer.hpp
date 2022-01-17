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

			Buffer(size_t size = 0);
			Buffer(std::string str);
			Buffer(char *buff, size_t size);
			~Buffer();

			size_t size();
			char * get_ptr();

			std::string to_string();

		private:

			char * _data;
			size_t _size;

		};
	}
}

#endif
