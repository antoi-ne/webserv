from utils import connect_one, get_res
from utils_test import common_check

def test_fail_not_finish_CR():
	client = connect_one()
	client.send("POST /post HTTP/1.1\r\nhost: x\r\ntransfer-encOding: chunked\r\n\r\n".encode())
	client.send("2\r\nok\r\n0\r\n\r\r\n".encode())
	res = get_res(client)
	common_check(res, client, 400)

def test_fail_no_chunk_size():
	client = connect_one()
	client.send("POST /post HTTP/1.1\r\nhost: x\r\ntransfer-encOding: chunked\r\n\r\n".encode())
	client.send("lala\r\nok\r\n0\r\n\r\r\n".encode())
	res = get_res(client)
	common_check(res, client, 400)

def test_success_nothing():
	client = connect_one()
	client.send("POST /post HTTP/1.1\r\nhost: x\r\ntransfer-encOding: chunked\r\n\r\n".encode())
	client.send("0\r\n\r\n".encode())
	res = get_res(client)
	common_check(res, client, 200) # or 201?