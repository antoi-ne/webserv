from http.client import HTTPResponse
from constant import PORT
import time
import socket

def	connect_one(port = PORT, host = "localhost"):
	client = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
	client.connect((host, port))
	return client;

def connect_req(request):
	client = connect_one()
	return [req_one_res(client, request), client]

def req_one_res(client, request):
	client.send(request.encode())
	return get_res(client);

def get_res(client):
	time.sleep(1);
	response = HTTPResponse(client)
	response.begin()
	return response;

def one_req_div_send(client, request, div):
	size = len(request)
	char_by_send = int(size / div);
	last_seq_size = int(char_by_send + size % div);
	start = 0
	while size > start + last_seq_size:
		_req = request[start:start + char_by_send]
		client.send(_req.encode())
		start += char_by_send
	_req = request[start:start + last_seq_size]
	client.send(_req.encode())

def chunk_send_file(client, header, body, div):
	CRLF = "\r\n"
	client.send(header.encode())
	client.send(CRLF.encode())

	size = len(body)
	char_by_send = int(size / div);
	last_seq_size = int(char_by_send + size % div);
	start = 0
	chunk_size = hex(char_by_send)[2:] + CRLF
	while size > start + last_seq_size:
		_req = chunk_size;
		client.send(_req.encode())
		_req = body[start:start + char_by_send]
		client.send(_req)
		client.send(CRLF.encode())
		start += char_by_send
	_req = hex(last_seq_size)[2:] + CRLF
	client.send(_req.encode())
	_req = body[start:start + last_seq_size]
	client.send(_req)
	client.send(CRLF.encode())
	_req = ("0" + CRLF + CRLF).encode()
	client.send(_req)

def chunk_send(client, header, body, div):
	CRLF = "\r\n"
	client.send(header.encode())
	client.send(CRLF.encode())

	size = len(body)
	char_by_send = int(size / div);
	last_seq_size = int(char_by_send + size % div);
	start = 0
	chunk_size = hex(char_by_send)[2:] + CRLF
	while size > start + last_seq_size:
		_req = chunk_size;
		client.send(_req.encode())
		_req = body[start:start + char_by_send] + CRLF
		client.send(_req.encode())
		start += char_by_send
	_req = hex(last_seq_size)[2:] + CRLF
	client.send(_req.encode())
	_req = body[start:start + last_seq_size] + CRLF
	client.send(_req.encode())
	_req = ("0" + CRLF + CRLF).encode()
	client.send(_req)
