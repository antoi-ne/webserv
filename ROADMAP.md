# roadmap

## modules

### `shared`
Handy tools used in multiple modules.

### `conf`
Parse a config file into a manageable structure.

### `net`
Multiplexing abstraction handling multiple tcp servers.
- `net::Server`: represents a single TCP server listening on a port.
- `net::Connection`: represents a connection between a server and a client.
- `net::Pool`: multiplexing server. keeps tracks of current connections on a given list of servers.
- `net::Ctx`:: represents a `net::Pool` connection ready to read/write.

### `http`
Deserializing HTTP requests and serializing HTTP responses.

### `core`
Main logic of the websever.
- `core::Controller`: creates and handles servers from conf into net, keeps track of current connections and parses them with `http`. Sends the requests to `core::Router`, and sends responses to the client trough `net`.
- `core::Router`: receives and process http request and returns http responses.

### `cgi`
Handles CGI requests.
