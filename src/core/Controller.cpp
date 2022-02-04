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
						if (req.error())
						{
							http::Res resf;

							resf.setStatus(STATUS400);
							resf.header()["Connection"] = "close";
							resf.body().join(std::string("Bad Request\r\n"));
							this->_res_cache[it->con] = std::make_pair(resf.get_res(), false);
						}
						else
						{
							http::Res res = this->_router.process(
								this->_req_cache[it->con],
								std::make_pair(it->srv.get_host(),
								it->srv.get_port())
							);
							this->_res_cache[it->con] = std::make_pair(res.get_res(), true);
							if (this->_req_cache[it->con].header("Connection") == "close")
								this->_res_cache[it->con].second = false;
						}
						this->_req_cache.erase(it->con);
					}
				}

				if (it->rwrite && this->_res_cache[it->con].first.size() != 0) // ready to receive response if any response is in the cache
				{
					ssize_t rbytes = it->con.send(this->_res_cache[it->con].first);
					if (rbytes == 0)
					{
						this->_res_cache.erase(it->con);
						this->_pool.close_con(it->con);
					}
					if (rbytes < 0)
						continue;
					this->_res_cache[it->con].first.advance(rbytes);
					if (this->_res_cache[it->con].first.size() == 0)
					{
						this->_res_cache.erase(it->con);
						if (this->_res_cache[it->con].second == false)
							this->_pool.close_con(it->con);
					}
				}
			}
		}
	}
}
