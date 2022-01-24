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
						shared::Log::info("connection closed by client");
						continue;
					}
					shared::Log::info("received data");
					if (this->_req_cache[it->con].update(opt.value()) == false)
					{
						const http::Req& req = this->_req_cache[it->con];
						shared::Log::info(this->_req_cache[it->con].body().to_string());
						shared::Log::info("completed request");
						std::cout << this->_req_cache[it->con].method() << std::endl;
						if (req.method() != UNDEF || req.path().empty())
						{
							resf.setStatus(STATUS400);
							resf.body().join(std::string("Bad Request\r\n"));
							this->_res_cache[it->con].push_back(resf);
						}
						else
						{
							const http::Res res = this->_router.process(
								this->_req_cache[it->con],
								std::make_pair(it->srv.get_host(),
								it->srv.get_port())
							);
							this->_res_cache[it->con].push_back(res);
						}
						this->_req_cache.erase(it->con);
					}
					else
						shared::Log::info("request not complete");
				}

				if (it->rwrite && !this->_res_cache[it->con].empty()) // ready to receive response if any response is in the cache
				{
					buff = this->_res_cache[it->con].front().get_res();
					if (!it->con.send(buff))
						this->_pool.close_con(it->con);
					this->_res_cache[it->con].erase(this->_res_cache[it->con].begin());
				}
			}
		}
	}
}
