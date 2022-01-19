#ifndef WS_CORE_ROUTER_HPP
#define WS_CORE_ROUTER_HPP

namespace ws
{
	namespace core
	{
		class Router
		{
		public:
			Router();
			Router(void *config);
			~Router();

			void *process(void *request);

		private:
			void *_config;
		};
	}
}

#endif
