#include "Ctx.hpp"

namespace ws
{
	namespace net
	{
		Ctx::Ctx()
			: con(), srv(), rread(false), rwrite(false)
		{}

		Ctx::Ctx(Connection con, Server srv, bool rread, bool rwrite)
			: con(con), srv(srv), rread(rread), rwrite(rwrite)
		{}

		Ctx::~Ctx()
		{}
	}
}
