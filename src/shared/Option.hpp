#ifndef WS_OPTION_SHARED_HPP
# define WS_OPTION_SHARED_HPP

#include <exception>

namespace ws
{
	namespace shared
	{
		template <typename T>
		class Option
		{
		public:

			class BadAccessException: public std::exception
			{
			public:
				virtual const char* what() const throw()
				{
					return "tried to access optional element that was null";
				}
			};

			Option()
				: _null(true), _value()
			{}

			Option(T & value)
				: _null(false), _value(value)
			{}

			Option(const Option & rhs)
			{
				*this = rhs;
			}

			Option & operator=(const Option & rhs)
			{
				if (this != &rhs)
				{
					this->_null = rhs._null;
					if (!rhs._null)
						this->_value = rhs._value;
				}
				return *this;
			}

			~Option()
			{}

			bool null()
			{
				return this->_null;
			}

			T & value() throw(BadAccessException)
			{
				if (this->_null)
					throw BadAccessException();
				else
					return this->_value;
			}

			void nullify()
			{
				this->_null = true;
			}

			void set(T & value)
			{
				this->_null = false;
				this->_value = value;
			}

		private:

			bool _null;
			T _value;

		};
	}
}

#endif
