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
			std::map<net::Connection, Req>::iterator rqit;

			ctxs = this->_pool.probe();
			
			for (it = ctxs.begin(); it != ctxs.end(); it++)
			{
				if (it->rread) // new request fragment received
				{
					shared::Option<shared::Buffer> opt;

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
						http::Res res;
						if (req.error())
						{
							res.header()["Connection"] = "close";
							res.setStatus(STATUS400);
							res.body().join(std::string("Bad Request\r\n"));
						}
						else
						{
							res = this->_router.process(
								this->_req_cache[it->con],
								std::make_pair(it->srv.get_host(),
								it->srv.get_port())
							);
						}
						this->_res_cache[it->con] = std::make_pair(res.get_res(), res.header("connection") != "close");
						this->_req_cache.erase(it->con);
					}
				}

				if (it->rwrite && this->_res_cache.find(it->con) != this->_res_cache.end())
				{
					ssize_t rbytes = it->con.send(this->_res_cache[it->con].first);
					if (rbytes == 0)
					{
						this->_res_cache.erase(it->con);
						this->_pool.close_con(it->con);
						continue;
					}
					if (rbytes < 0)
						continue;
					this->_res_cache[it->con].first.advance(rbytes);
					if (this->_res_cache[it->con].first.size() == 0)
					{
						if (this->_res_cache[it->con].second == false)
							this->_pool.close_con(it->con);
						this->_res_cache.erase(it->con);
					}
				}
			}

			for (rqit = this->_req_cache.begin(); rqit != this->_req_cache.end(); rqit++)
			{
				if (rqit->second.getCreatedTime() + 60 > time(NULL))
				{
					this->_pool.close_con(rqit->first);
					this->_req_cache.erase(rqit);
				}
			}
		}
	}
}
