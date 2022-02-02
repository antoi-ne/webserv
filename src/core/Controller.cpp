#include "Controller.hpp"

namespace ws
{
	namespace core
	{

		Controller::Controller(const conf::Config& config)
			: _config(config), _srv(), _pool(), _req_cache(), _res_cache(), _router(config)
		{
			conf::server_map::const_iterator it;
			for (it = config.servers.begin(); it != config.servers.end(); it++)
				this->_srv.push_back(net::Server(it->first.first, it->first.second));
		}

		Controller::~Controller()
		{}

		void Controller::start()
		{
			std::list<net::Server>::iterator it;

			for (it = this->_srv.begin(); it != this->_srv.end(); it++)
				it->listen();

			this->_req_cache.clear();
			this->_res_cache.clear();

			this->_pool = net::Pool(this->_srv);

			for(;;)
				this->_loop();
		}

		void Controller::_loop()
		{
			std::list<net::Ctx> ctxs;
			std::list<net::Ctx>::iterator it;
			shared::Buffer buff;
			shared::Option<shared::Buffer> opt;
			http::Res resf;


			ctxs = this->_pool.probe();
			for (it = ctxs.begin(); it != ctxs.end(); it++)
			{
				if (it->rread) // new request fragment received
				{
					opt = it->con.recv(4096);
					if (opt.null())
					{
						this->_pool.close_con(it->con);
						this->_req_cache.erase(it->con);
						continue;
					}
					Req& req = this->_req_cache[it->con];
					if (req.update(opt.value()) == false)
					{
						shared::Log::info(this->_req_cache[it->con].body().to_string());
						std::cout << this->_req_cache[it->con].method() << std::endl;
						if (req.error())
						{
							resf.setStatus(STATUS400);
							resf.body().join(std::string("Bad Request\r\n"));
							this->_res_cache[it->con] = resf.get_res();
						}
						else
						{
							http::Res res = this->_router.process(
								this->_req_cache[it->con],
								std::make_pair(it->srv.get_host(),
								it->srv.get_port())
							);
							this->_res_cache[it->con] = res.get_res();
						}
						this->_req_cache.erase(it->con);
					}
				}

				if (it->rwrite && this->_res_cache[it->con].size() != 0) // ready to receive response if any response is in the cache
				{
					ssize_t rbytes = it->con.send(this->_res_cache[it->con]);
					if (rbytes == 0)
					{
						this->_res_cache[it->con] = shared::Buffer();
						this->_pool.close_con(it->con);
					}
					if (rbytes < 0)
						continue;
					this->_res_cache[it->con].advance(rbytes);
					if (this->_res_cache[it->con].size() == 0)
					{
						this->_res_cache[it->con] = shared::Buffer();
						this->_pool.close_con(it->con);
					}
				}
			}
		}
	}
}
