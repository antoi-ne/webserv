# roadmap

## modules

### `shared`
Handy tools used in multiple modules.

### `conf`
Parse a config file into a manageable structure.

### `net`
Multiplexing abstraction handling multiple tcp servers.

### `http`
Deserializing HTTP requests and serializing HTTP responses.

### `core`
Main logic of the websever.
- `core::Controller`: creates and handles servers from conf into net, keeps track of current connections and parses them with `http`. Sends the requests to `core::Router`, and sends responses to the client trough `net`.
- `core::Router`: receives and process http request and returns http responses.

### `cgi`
Handles CGI requests.
